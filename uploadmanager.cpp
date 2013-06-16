#include "uploadmanager.h"

UploadManager::UploadManager(ConnectionController * c)
{
    connection = c;
    downloadInProgress = false;
    fileSize = 0;
    downloadsTimer = new QTimer(this);
    bytesRead = 0;
    lastRead = 0;
    downloadSpeed = 0;
}

uint UploadManager::cleanIdle() {
    quint32 deleted = 1;
    CUpload * upload = NULL;
    while(deleted) {
        deleted = 0;
        for(quint32 i=0; i<uploads.size(); i++) {
            upload = uploads[i];
            if(upload->finished) {
                listWidget->removeItemWidget(upload->itemPlaceholder);
                listWidget->takeItem(listWidget->row(upload->itemPlaceholder));
                if(upload->widget) {
                    delete upload->widget;
                }
                if(upload->itemPlaceholder) {
                    delete upload->itemPlaceholder;
                }
                uploads[i] = uploads.back();
                uploads.pop_back();
                delete upload;
                deleted++;
            }
        }
    }
    return deleted;
}

void UploadManager::onRequestedFile(QString name, qint32 size) {
    CUpload * upload = new CUpload();
    upload->fileSize = size;
    qDebug() << "Got new item. Will use size "<<size<<" for matching";
    upload->file->setFileName(name);
    upload->currentName = name.split("/").last();

    qDebug() << "Creating item widget";
    QListWidgetItem * item = new QListWidgetItem();
    WidgetDownloadItem * customItem = new WidgetDownloadItem();
    item->setSizeHint(customItem->sizeHint());
    listWidget->addItem(item);
    listWidget->setItemWidget(item, customItem);
    upload->widget = customItem;
    upload->itemPlaceholder = item;
    connect(customItem->stopButton(), SIGNAL(clicked()), upload, SLOT(stopUpload()));

    upload->updateName();
    qDebug() << "Saving item";
    uploads.push_back(upload);
}

void UploadManager::addUpload(quint32 ref) {
    qDebug() << "Got signal from connection controller";
    CUpload * upload = NULL;
    qDebug() << "Trying to find pending";
    for(quint32 i=0; i<uploads.size(); i++) {
        if(uploads[i]->pending) {
            upload = uploads[i];
            upload->pending = false;
        }
    }

    if(upload == NULL) {
        qDebug() << "Error. Item was not in upload list";
    }

    upload->referenceNumber = ref;
    upload->connection = connection;
    qDebug() << "Initializing upload item...";
    upload->init();
}
