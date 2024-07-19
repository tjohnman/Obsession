#include "downloadmanager.h"
#include <QSettings>
#include <QDesktopServices>
#include "dialogerror.h"
#include "TextHelper.h"

DownloadManager::DownloadManager(ConnectionController * c)
{
    connection = c;
    downloadInProgress = false;
    fileSize = 0;
    downloadsTimer = new QTimer(this);
    bytesRead = 0;
    lastRead = 0;
    downloadSpeed = 0;
    connect(connection, SIGNAL(serverUpdatedQueue(quint32, quint32)), this, SLOT(onQueueUpdate(quint32,quint32)));
}

uint DownloadManager::cleanIdle() {
    quint32 deleted = 1;
    CDownload * download = NULL;
    while(deleted) {
        deleted = 0;
        for(quint32 i=0; i<downloads.size(); i++) {
            download = downloads[i];
            if(download->finished) {
                listWidget->removeItemWidget(download->itemPlaceholder);
                listWidget->takeItem(listWidget->row(download->itemPlaceholder));
                if(download->widget) {
                    delete download->widget;
                }
                if(download->itemPlaceholder) {
                    delete download->itemPlaceholder;
                }
                downloads[i] = downloads.back();
                downloads.pop_back();
                delete download;
                deleted++;
            }
        }
    }
    return deleted;
}

void DownloadManager::onDownloadError(QString errorString)
{
    if (errorString.length() > 0)
    {
        DialogError * dialog = new DialogError(errorString, 0);
        dialog->show();
        connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
    }
}

void DownloadManager::onDownloadFinished() {
    quint32 active = 0;

    for(quint32 i=0; i<downloads.size(); i++) {
        CDownload * download;
        download = downloads[i];
        if(download->inited && !download->finished) {
            active++;
        }
    }

    QSettings settings("mir", "Contra");
    qint32 dlqueue = settings.value("dlqueue", 1).toInt();

    qint32 free;
    if(dlqueue <= 0) {
        for(quint32 i=0; i<downloads.size(); i++) {
            CDownload * download;
            download = downloads[i];
            if(download->widget->infoLabel()->text() == "Queued") {
                sendDownloadRequestToServer(download);
            }
        }
    } else {
        free = dlqueue - active;
        for(qint32 d=0; d<free; d++) {
            for(quint32 i=0; i<downloads.size(); i++) {
                CDownload * download;
                download = downloads[i];
                if(download->widget->infoLabel()->text() == "Queued") {
                    sendDownloadRequestToServer(download);
                    break;
                }
            }
        }
    }
}

void DownloadManager::onForcedDownload(CDownload * download) {
    sendDownloadRequestToServer(download);
}

void DownloadManager::sendDownloadRequestToServer(CDownload * download) {
    QString path = download->pathOnServer;
    download->matched = false;
    download->widget->infoLabel()->setText("Waiting for server...");
    CTransaction * fileRequest = connection->createTransaction(202);
    fileRequest->addParameter(201, TextHelper::EncodeText(download->currentName).size(), TextHelper::EncodeText(download->currentName).data());

    if(!path.endsWith("/")) {
        path.append("/");
    }

    QStringList levels = path.split("/", QString::SkipEmptyParts);
    quint16 directorylevels = levels.count();
    quint16 pathlen = 2 + directorylevels * 3;
    for(qint32 i=0; i<levels.count(); i++) {
        pathlen += TextHelper::EncodeText(levels.at(i)).size();
    }

    char * pathdata = (char *) malloc(sizeof(char)*pathlen);

    directorylevels = qToBigEndian(directorylevels);
    memcpy(pathdata, &directorylevels, 2);

    qint32 offset = 0;
    for(qint32 i=0; i<levels.count(); i++) {
        memset(pathdata+offset+2, 0, 2);
        QString level = levels.at(i);
        unsigned char len = TextHelper::EncodeText(level).size();

        memcpy(pathdata+offset+4, &len, 1);
        memcpy(pathdata+offset+5, TextHelper::EncodeText(level).data(), len);
        offset += 3+len;
    }
    fileRequest->addParameter(202, pathlen, pathdata);

    // Look for existing data

    QFile preFile;
    preFile.setFileName(DownloadManager::GetDownloadsDirectoryPath()+"/"+QString(download->currentName));
    if(preFile.exists()) {
        quint32 preSize = preFile.size();

        preSize = qToBigEndian(preSize);
        char resumeData[74];
        memcpy(resumeData, "RFLT\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\2DATA", 46);
        memcpy(resumeData+46, &preSize, 4);
        memcpy(resumeData+50, "\0\0\0\0\0\0\0\0MACR\0\0\0\0\0\0\0\0\0\0\0\0", 24);
        fileRequest->addParameter(203, 74, resumeData);
    }

    //

    connection->sendTransaction(fileRequest, true);
}

void DownloadManager::onQueueUpdate(quint32 ref, quint32 pos) {
    CDownload * download = NULL;
    for(quint32 i=0; i<downloads.size(); i++) {
        if(downloads[i]->referenceNumber == ref) {
            download = downloads[i];

            download->queuePosition = pos;
            download->init();
            return;
        }
    }
}

void DownloadManager::onRequestedFile(QString name, qint32 size, QString path) {
    if(size < 0) {
        DialogError error("The Hotline protocol does not support files over 2GB in size.", 0);
        error.show();
        return;
    }
    CDownload * newDownload = new CDownload();

    newDownload->currentName = name;
    newDownload->pathOnServer = path;
    newDownload->fileSize = size;

    QFile preFile;
    preFile.setFileName(DownloadManager::GetDownloadsDirectoryPath()+"/"+QString(newDownload->currentName));
    if(preFile.exists()) {
        quint32 preSize = preFile.size();
        newDownload->bytesRead = preSize;
        newDownload->dataSize = size - preSize;
    } else {
        newDownload->dataSize = size;
    }

    QListWidgetItem * item = new QListWidgetItem();
    WidgetDownloadItem * customItem = new WidgetDownloadItem();
    item->setSizeHint(customItem->sizeHint());
    listWidget->addItem(item);
    listWidget->setItemWidget(item, customItem);
    customItem->infoLabel()->setText("Queued");
    newDownload->widget = customItem;
    newDownload->itemPlaceholder = item;

    newDownload->widget->goButton()->setEnabled(true);
    newDownload->widget->queueButton()->setEnabled(false);
    newDownload->widget->stopButton()->setEnabled(true);

    connect(customItem->stopButton(), SIGNAL(clicked()), newDownload, SLOT(stopDownload()));
    connect(customItem->goButton(), SIGNAL(clicked()), newDownload, SLOT(forceDownload()));
    connect(customItem->queueButton(), SIGNAL(clicked()), newDownload, SLOT(queueDownload()));

    newDownload->updateName();
    connect(newDownload, SIGNAL(downloadFinished()), this, SLOT(onDownloadFinished()));
    connect(newDownload, SIGNAL(forcedDownload(CDownload *)), this, SLOT(onForcedDownload(CDownload*)));
    connect(newDownload, SIGNAL(gotError(QString)), this, SLOT(onDownloadError(QString)));

    downloads.push_back(newDownload);
    onDownloadFinished();
}

void DownloadManager::addDownload(quint32 ref, quint32 size, quint32 queuepos) {
    CDownload * newDownload = NULL;
    for(quint32 i=0; i<downloads.size(); i++) {
        if(downloads[i]->fileSize == size) {
            newDownload = downloads[i];
        }
    }

    if(newDownload == NULL) {
        for(quint32 i=0; i<downloads.size(); i++) {
            if(!downloads[i]->matched) {
                newDownload = downloads[i];
                break;
            }
        }
        if(newDownload == NULL) {
            qDebug() << "Could not match download to file.";
            return;
        }
    }

    newDownload->referenceNumber = ref;
    newDownload->queuePosition = queuepos;
    newDownload->connection = connection;

    newDownload->init();
}

QString DownloadManager::GetDownloadsDirectoryPath()
{
    // Qt4 does not offer a standard "Downloads" location. We could define one in the preferences dialog.
    return QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
}
