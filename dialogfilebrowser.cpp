#include "dialogfilebrowser.h"
#include "ui_dialogfilebrowser.h"
#include "ctransaction.h"
#include <QTextStream>
#include <QMimeData>
#include <QFileDialog>
#include <QUrl>
#include "TextHelper.h"

DialogFileBrowser::DialogFileBrowser(ConnectionController * c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFileBrowser)
{
    ui->setupUi(this);
    connection = c;
    path = _m_RawPath = "/";
    connect(connection, SIGNAL(gotFileList(std::vector<s_hotlineFile *>)), this, SLOT(onGotFileList(std::vector<s_hotlineFile *>)));
    connect(ui->treeWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClick(QModelIndex)));
    connect(ui->treeWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectionChange()));

    connect(ui->buttonUp, SIGNAL(clicked()), this, SLOT(goDirectoryUp()));
    connect(ui->buttonDownload, SIGNAL(clicked()), this, SLOT(requestFile()));
    connect(ui->buttonUpload, SIGNAL(clicked()), this, SLOT(selectFileToUpload()));
    connect(ui->buttonDelete, SIGNAL(clicked()), this, SLOT(requestFileDelete()));
    connect(ui->buttonRefresh, SIGNAL(clicked()), this, SLOT(load()));

    ui->treeWidget->sortByColumn(0, Qt::AscendingOrder);


    ui->buttonDownload->setEnabled(false);
    ui->buttonDelete->setEnabled(false);

    setAcceptDrops(false);
    ui->treeWidget->setAcceptDrops(false);
}

void DialogFileBrowser::dragEnterEvent(QDragEnterEvent *event){
    event->acceptProposedAction();
}

void DialogFileBrowser::selectionChange()
{
    if(ui->treeWidget->selectedItems().size() > 0)
    {
        ui->buttonDownload->setEnabled(true);
        ui->buttonDelete->setEnabled(true);
    }
    else
    {
        ui->buttonDownload->setEnabled(false);
        ui->buttonDelete->setEnabled(false);
    }
}

void DialogFileBrowser::dropEvent(QDropEvent *event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
        return;

    uploadedFile.setFileName(fileName);
    requestUpload();
}

QString DialogFileBrowser::getExtension(QString name) {
    qint32 last = name.lastIndexOf(".");
    if(last == -1) {
        return "";
    }
    return name.right(name.length()-last-1).toLower();
}

void DialogFileBrowser::resetPath() {
    path = "/";
    ui->treeWidget->setEnabled(true);
}

void DialogFileBrowser::load() {
    if(!path.endsWith("/")) {
        path.append("/");
    }
    if(!_m_RawPath.endsWith("/"))
    {
        _m_RawPath.append("/");
    }
    ui->labelPath->setText(path);
    CTransaction * fileListTransaction = connection->createTransaction(200);

    if(path.length() > 1) {
        QStringList levels = path.split("/", Qt::SkipEmptyParts);
        quint16 directorylevels = levels.count();
        quint16 pathlen = 2 + directorylevels * 3;
        for(qint32 i=0; i<levels.count(); i++) {
            QString level = levels.at(i);
            pathlen += TextHelper::EncodeText(_m_RawNames[level]).length();
        }

        char * pathdata = (char *) malloc(sizeof(char)*pathlen);

        directorylevels = qToBigEndian(directorylevels);
        memcpy(pathdata, &directorylevels, 2);

        qint32 offset = 0;
        for(qint32 i=0; i<levels.count(); i++) {
            memset(pathdata+offset+2, 0, 2);
            QString level = levels.at(i);
            unsigned char len = (unsigned char)TextHelper::EncodeText(_m_RawNames[level]).length();
            memcpy(pathdata+offset+4, &len, 1);
            memcpy(pathdata+offset+5, TextHelper::EncodeText(_m_RawNames[level]).data(), len);
            offset += 3+len;
        }

        fileListTransaction->addParameter(202, pathlen, pathdata);
    }
    connection->sendTransaction(fileListTransaction, true);
    ui->label->setText("loading...");
    ui->treeWidget->setEnabled(false);
}

void DialogFileBrowser::onDoubleClick(QModelIndex model) {
    QTreeWidgetItem * item = ui->treeWidget->currentItem();
    if(item->data(2, 0).toString() == "fldr") {
        path = QString(path + item->data(0, 0).toString() + "/");
        _m_RawPath = QString(_m_RawPath + _m_RawNames[item->data(0,0).toString()] + "/");
        load();
    } else {
        requestFile();
    }
}

void DialogFileBrowser::onGotFileList(std::vector<s_hotlineFile *> list) {
    ui->treeWidget->clear();

    for(quint32 i=0; i<list.size(); i++) {
        QTreeWidgetItem * item = new QTreeWidgetItem();
        QString _n1 = TextHelper::DecodeText(list[i]->name, list[i]->nameSize);
        QString _n = TextHelper::DecodeTextAutoUTF8(list[i]->name, list[i]->nameSize);
        _m_RawNames[_n] = _n1;
        item->setData(0, 0, _n);
        qint32 size = list[i]->size;
        if(!strncmp(list[i]->type, "fldr", 4)) {
            item->setData(1, 0, QString::number(size) + (size == 1 ? " item" : " items"));
        } else {
            if(size > 1024) {
                if(size > 1024*1024*1024) {
                    item->setData(1, 0, QString::number(size/1024/1024/1024) + " GB");
                } else {
                    if(size > 1024*1024) {
                        item->setData(1, 0, QString::number(size/1024/1024) + " MB");
                    } else {
                        item->setData(1, 0, QString::number(size/1024) + " KB");
                    }
                }
            } else {
                if(size < 0) {
                    item->setData(1, 0, "more than 2GB");
                } else {
                    if(list[i]->size <= 0) {
                        item->setData(1, 0, "0 bytes");
                    } else {
                        item->setData(1, 0, QString::number(size) + " bytes");
                    }
                }
            }
        }
        item->setData(3, 0, size);
        item->setTextAlignment(1, Qt::AlignRight);

        if(!strncmp(list[i]->type, "fldr", 4)) {
            item->setIcon(0, QIcon(":/files/interfaceIcons/filesFolder.png"));
        } else {
            item->setIcon(0, QIcon(":/files/interfaceIcons/filesUnknown.png"));

            if(getExtension(_n) == "sitx") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "sit") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "zip") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "dmg") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "rar") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "7zip") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "7z") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "z") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "tar") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "hqx") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "gzip") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "gz") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(getExtension(_n) == "exe") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesExe.png"));
            }
            if(getExtension(_n) == "jpg") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesImage.png"));
            }
            if(getExtension(_n) == "jpeg") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesImage.png"));
            }
            if(getExtension(_n) == "bmp") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesImage.png"));
            }
            if(getExtension(_n) == "txt") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesText.png"));
            }
            if(getExtension(_n) == "srt") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesText.png"));
            }
            if(getExtension(_n) == "lit") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesText.png"));
            }
            if(getExtension(_n) == "doc") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesDocument.png"));
            }
            if(getExtension(_n) == "pdf") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesDocument.png"));
            }
            if(getExtension(_n) == "rtf") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesDocument.png"));
            }
            if(getExtension(_n) == "mov") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesVideo.png"));
            }
            if(getExtension(_n) == "avi") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesVideo.png"));
            }
            if(getExtension(_n) == "wmv") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesVideo.png"));
            }
            if(getExtension(_n) == "mp4") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesVideo.png"));
            }
            if(getExtension(_n) == "mkv") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesVideo.png"));
            }
            if(getExtension(_n) == "mpg") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesVideo.png"));
            }
            if(getExtension(_n) == "mpeg") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesVideo.png"));
            }
            if(getExtension(_n) == "ogm") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesVideo.png"));
            }
            if(getExtension(_n) == "mp3") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesAudio.png"));
            }
            if(getExtension(_n) == "wav") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesAudio.png"));
            }
            if(getExtension(_n) == "aif") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesAudio.png"));
            }
            if(getExtension(_n) == "aiff") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesAudio.png"));
            }
            if(getExtension(_n) == "wma") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesAudio.png"));
            }
            if(getExtension(_n) == "ogg") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesAudio.png"));
            }
            if(getExtension(_n) == "iso") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesIso.png"));
            }
            if(getExtension(_n) == "nrg") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesIso.png"));
            }

            if(!strncmp(list[i]->type, "JPEG", 4)) {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesImage.png"));
            }
            if(!strncmp(list[i]->type, "PNGf", 4)) {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesImage.png"));
            }
            if(!strncmp(list[i]->type, "BMP ", 4)) {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesImage.png"));
            }
            if(!strncmp(list[i]->type, "ZIP ", 4)) {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(!strncmp(list[i]->type, "SITD", 4)) {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesArchive.png"));
            }
            if(!strncmp(list[i]->type, "DEXE", 4)) {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesExe.png"));
            }

            if(getExtension(_n) == "hpf") {
                item->setIcon(0, QIcon(":/files/interfaceIcons/filesPartial.png"));
            }
        }

        item->setData(2, 0, QString(list[i]->type));

        ui->treeWidget->addTopLevelItem(item);
    }
    ui->label->setText(QString::number(list.size())+" items");
    ui->treeWidget->setEnabled(true);
    for(qint32 i=0; i<ui->treeWidget->columnCount(); ++i)
    {
        ui->treeWidget->resizeColumnToContents(i);
    }

    selectionChange();
}

void DialogFileBrowser::goDirectoryUp() {
    if(path.isEmpty() || path == "/") {
        path = "/";
        return;
    }
    QStringList levels = path.split("/", QString::SkipEmptyParts);
    levels.pop_back();
    path = levels.join("/");
    load();
}

DialogFileBrowser::~DialogFileBrowser()
{
    delete ui;
}

void DialogFileBrowser::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogFileBrowser::requestFile() {
    if(ui->treeWidget->currentItem() == NULL) return;
    emit requestedFile(ui->treeWidget->currentItem()->data(0, 0).toString(), ui->treeWidget->currentItem()->data(3, 0).toInt(), path);
}

void DialogFileBrowser::requestFileDelete()
{
    CTransaction * transaction = connection->createTransaction(204);
    transaction->addParameter(201, TextHelper::EncodeText(ui->treeWidget->currentItem()->data(0, 0).toString()).size(), TextHelper::EncodeText(ui->treeWidget->currentItem()->data(0, 0).toString()).data());

    QStringList levels = path.split("/", Qt::SkipEmptyParts);
    quint16 directorylevels = levels.count();
    quint16 pathlen = 2 + directorylevels * 3;
    for(qint32 i=0; i<levels.count(); i++) {
        QString level = levels.at(i);
        pathlen += TextHelper::EncodeText(_m_RawNames[level]).length();
    }

    char * pathdata = (char *) malloc(sizeof(char)*pathlen);

    directorylevels = qToBigEndian(directorylevels);
    memcpy(pathdata, &directorylevels, 2);

    qint32 offset = 0;
    for(qint32 i=0; i<levels.count(); i++) {
        memset(pathdata+offset+2, 0, 2);
        QString level = levels.at(i);
        unsigned char len = (unsigned char)TextHelper::EncodeText(_m_RawNames[level]).length();

        memcpy(pathdata+offset+4, &len, 1);
        memcpy(pathdata+offset+5, TextHelper::EncodeText(_m_RawNames[level]).data(), len);
        offset += 3+len;
    }

    transaction->addParameter(202, pathlen, pathdata);

    connect(connection, SIGNAL(receivedFileDeleteResponse(qint32)), this, SLOT(gotFileDeleteResponse(qint32)));

    connection->sendTransaction(transaction, true);
}

void DialogFileBrowser::gotFileDeleteResponse(qint32 code)
{
    if(code == 0)
    {
        disconnect(connection, SIGNAL(receivedFileDeleteResponse(qint32)), this, SLOT(gotFileDeleteResponse(qint32)));
        load();
    }
}

void DialogFileBrowser::selectFileToUpload() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    if(dialog.exec() == QDialog::Accepted) {
        QStringList selectedFiles = dialog.selectedFiles();
        uploadedFile.setFileName(selectedFiles.at(0));
        requestUpload();
    }
}

void DialogFileBrowser::requestUpload() {
    CTransaction * uploadRequest = connection->createTransaction(203);
    uploadRequest->addParameter(201, TextHelper::EncodeText(uploadedFile.fileName().split("/").last()).size(), TextHelper::EncodeText(uploadedFile.fileName().split("/").last()).data());

    if(!path.endsWith("/")) {
        path.append("/");
    }

    QStringList levels = path.split("/", Qt::SkipEmptyParts);
    quint16 directorylevels = levels.count();
    quint16 pathlen = 2 + directorylevels * 3;
    for(qint32 i=0; i<levels.count(); i++) {
        QString level = levels.at(i);
        pathlen += TextHelper::EncodeText(_m_RawNames[level]).length();
    }

    char * pathdata = (char *) malloc(sizeof(char)*pathlen);

    directorylevels = qToBigEndian(directorylevels);
    memcpy(pathdata, &directorylevels, 2);

    qint32 offset = 0;
    for(qint32 i=0; i<levels.count(); i++) {
        memset(pathdata+offset+2, 0, 2);
        QString level = levels.at(i);
        unsigned char len = (unsigned char)TextHelper::EncodeText(_m_RawNames[level]).length();

        memcpy(pathdata+offset+4, &len, 1);
        qDebug() << TextHelper::EncodeText(_m_RawNames[level]);
        memcpy(pathdata+offset+5, TextHelper::EncodeText(_m_RawNames[level]).data(), len);
        offset += 3+len;
    }
    uploadRequest->addParameter(202, pathlen, pathdata);
    uploadRequest->addParameter(108, (quint32)uploadedFile.size());
    connection->sendTransaction(uploadRequest, true);

    emit requestedUpload(uploadedFile.fileName(), uploadedFile.size());
}
