#include "threadiconloader.h"

#include <QDir>
#include <QFileInfoList>
#include <QDebug>

void ThreadIconLoader::run() {
    QDir iconsFolder = QDir(":/icons");
    QFileInfoList fileList = iconsFolder.entryInfoList();
    emit startIconCount(iconsFolder.count()-2);

    for(quint32 i=2; i<iconsFolder.count(); i++) {
        QListWidgetItem * item = new QListWidgetItem();
        item->setSizeHint(QSize(232, 18));
        item->setIcon(QIcon(QPixmap::fromImage(QImage(fileList[i].absoluteFilePath()))));
        item->setData(Qt::DisplayRole, fileList[i].fileName());

        emit loadedItem(i, item);
    }
}
