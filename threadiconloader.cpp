#include "threadiconloader.h"

#include <QDir>
#include <QFileInfoList>
#include <QDebug>

void ThreadIconLoader::run() {
    QDir iconsFolder = QDir("icons");
    qDebug() << QDir::currentPath();
    qDebug() << "Loading" << iconsFolder.count() << "icons...";
    QFileInfoList fileList = iconsFolder.entryInfoList();
    emit startIconCount(iconsFolder.count()-2);

    for(unsigned int i=2; i<iconsFolder.count(); i++) {
        QListWidgetItem * item = new QListWidgetItem();
        item->setSizeHint(QSize(232, 18));
        item->setIcon(QIcon(QPixmap::fromImage(QImage(fileList[i].absoluteFilePath()))));
        item->setData(Qt::DisplayRole, fileList[i].fileName());

        emit loadedItem(i, item);
    }
}
