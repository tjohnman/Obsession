#ifndef UPLOADMANAGER_H
#define UPLOADMANAGER_H

#include <QTcpSocket>
#include <QProgressBar>
#include <QHostAddress>
#include <QtEndian>
#include <QFile>
#include <QObject>
#include <vector>
#include <QTimer>
#include <QListWidgetItem>
#include <QListWidget>

#include "cupload.h"
#include "widgetdownloaditem.h"
#include "connectioncontroller.h"
#include "defines.h"

class UploadManager : public QObject
{
    Q_OBJECT
public:
    bool downloadInProgress;
    quint32 referenceNumber;
    quint32 fileSize;
    QTcpSocket socket;
    QProgressBar * progressBar;
    QFile * file;
    QString currentName;
    quint32 bytesRead;
    quint32 dataSize;
    qint32 bytesWritten;
    ConnectionController * connection;
    quint32 lastRead;
    QTimer * downloadsTimer;
    quint32 downloadSpeed;
    QListWidget * listWidget;

    std::vector<CUpload *> uploads;

    qint32 init(quint32 reference, quint32 size);

    UploadManager(ConnectionController * c);

private slots:

public slots:
    void addUpload(quint32);
    void onRequestedFile(QString name, qint32 size);
    uint cleanIdle();

signals:
    void newItem(QListWidgetItem *);
};

#endif // UPLOADMANAGER_H
