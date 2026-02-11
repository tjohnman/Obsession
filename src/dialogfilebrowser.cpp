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
    path = _m_RawPath = QString::fromUtf8("/");
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
    qint32 last = name.lastIndexOf(QString::fromUtf8("."));
    if(last == -1) {
        return QString::fromUtf8("");
    }
    return name.right(name.length()-last-1).toLower();
}

void DialogFileBrowser::resetPath() {
    path = QString::fromUtf8("/");
    ui->treeWidget->setEnabled(true);
}

void DialogFileBrowser::load() {
    if(!path.endsWith(QString::fromUtf8("/"))) {
        path.append(QString::fromUtf8("/"));
    }
    if(!_m_RawPath.endsWith(QString::fromUtf8("/")))
    {
        _m_RawPath.append(QString::fromUtf8("/"));
    }
    ui->labelPath->setText(path);
    CTransaction * fileListTransaction = connection->createTransaction(200);

    if(path.length() > 1) {
        QStringList levels = path.split(QString::fromUtf8("/"), Qt::SkipEmptyParts);
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
    ui->label->setText(QString::fromUtf8("loading..."));
    ui->treeWidget->setEnabled(false);
}

void DialogFileBrowser::onDoubleClick(QModelIndex model) {
    QTreeWidgetItem * item = ui->treeWidget->currentItem();
    if(item->data(2, 0).toString() == QString::fromUtf8("fldr")) {
        path = QString(path + item->data(0, 0).toString() + QString::fromUtf8("/"));
        _m_RawPath = QString(_m_RawPath + _m_RawNames[item->data(0,0).toString()] + QString::fromUtf8("/"));
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
            item->setData(1, 0, QString::number(size) + (size == 1 ? QString::fromUtf8(" item") : QString::fromUtf8(" items")));
        } else {
            if(size > 1024) {
                if(size > 1024*1024*1024) {
                    item->setData(1, 0, QString::number(size/1024/1024/1024) + QString::fromUtf8(" GB"));
                } else {
                    if(size > 1024*1024) {
                        item->setData(1, 0, QString::number(size/1024/1024) + QString::fromUtf8(" MB"));
                    } else {
                        item->setData(1, 0, QString::number(size/1024) + QString::fromUtf8(" KB"));
                    }
                }
            } else {
                if(size < 0) {
                    item->setData(1, 0, QVariant(QString::fromUtf8("more than 2GB")));
                } else {
                    if(list[i]->size <= 0) {
                        item->setData(1, 0, QVariant(QString::fromUtf8("0 bytes")));
                    } else {
                        item->setData(1, 0, QString::number(size) + QString::fromUtf8(" bytes"));
                    }
                }
            }
        }
        item->setData(3, 0, size);
        item->setTextAlignment(1, Qt::AlignRight);

        if(!strncmp(list[i]->type, "fldr", 4)) {
            item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesFolder.png")));
        } else {
            item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesUnknown.png")));

            if(getExtension(_n) == QString::fromUtf8("sitx")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("sit")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("zip")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("dmg")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("rar")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("7zip")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("7z")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("z")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("tar")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("hqx")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("gzip")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("gz")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("exe")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesExe.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("jpg")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesImage.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("jpeg")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesImage.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("bmp")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesImage.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("txt")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesText.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("srt")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesText.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("lit")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesText.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("doc")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesDocument.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("pdf")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesDocument.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("rtf")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesDocument.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("mov")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesVideo.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("avi")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesVideo.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("wmv")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesVideo.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("mp4")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesVideo.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("mkv")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesVideo.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("mpg")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesVideo.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("mpeg")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesVideo.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("ogm")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesVideo.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("mp3")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesAudio.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("wav")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesAudio.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("aif")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesAudio.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("aiff")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesAudio.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("wma")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesAudio.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("ogg")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesAudio.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("iso")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesIso.png")));
            }
            if(getExtension(_n) == QString::fromUtf8("nrg")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesIso.png")));
            }

            if(!strncmp(list[i]->type, "JPEG", 4)) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesImage.png")));
            }
            if(!strncmp(list[i]->type, "PNGf", 4)) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesImage.png")));
            }
            if(!strncmp(list[i]->type, "BMP ", 4)) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesImage.png")));
            }
            if(!strncmp(list[i]->type, "ZIP ", 4)) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(!strncmp(list[i]->type, "SITD", 4)) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesArchive.png")));
            }
            if(!strncmp(list[i]->type, "DEXE", 4)) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesExe.png")));
            }

            if(getExtension(_n) == QString::fromUtf8("hpf")) {
                item->setIcon(0, QIcon(QString::fromUtf8(":/files/interfaceIcons/filesPartial.png")));
            }
        }

        item->setData(2, 0, QString::fromUtf8(list[i]->type));

        ui->treeWidget->addTopLevelItem(item);
    }
    ui->label->setText(QString::number(list.size())+ QString::fromUtf8(" items"));
    ui->treeWidget->setEnabled(true);
    for(qint32 i=0; i<ui->treeWidget->columnCount(); ++i)
    {
        ui->treeWidget->resizeColumnToContents(i);
    }

    selectionChange();
}

void DialogFileBrowser::goDirectoryUp() {
    if(path.isEmpty() || path == QString::fromUtf8("/")) {
        path = QString::fromUtf8("/");
        return;
    }
    QStringList levels = path.split(QString::fromUtf8("/"), Qt::SkipEmptyParts);
    levels.pop_back();
    path = levels.join(QString::fromUtf8("/"));
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

    QStringList levels = path.split(QString::fromUtf8("/"), Qt::SkipEmptyParts);
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
    uploadRequest->addParameter(201, TextHelper::EncodeText(uploadedFile.fileName().split(QString::fromUtf8("/")).last()).size(), TextHelper::EncodeText(uploadedFile.fileName().split(QString::fromUtf8("/")).last()).data());

    if(!path.endsWith(QString::fromUtf8("/"))) {
        path.append(QString::fromUtf8("/"));
    }

    QStringList levels = path.split(QString::fromUtf8("/"), Qt::SkipEmptyParts);
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
