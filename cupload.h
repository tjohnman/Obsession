#ifndef CUPLOAD_H
#define CUPLOAD_H

#include <QTcpSocket>
#include <QProgressBar>
#include <QFile>
#include <QString>
#include <QTimer>
#include "connectioncontroller.h"
#include "widgetdownloaditem.h"
#include <QListWidgetItem>
#include "threaduploader.h"

class CUpload : public QObject
{
    Q_OBJECT
public:
    bool uploadInProgress;
    bool finished;
    quint32 referenceNumber;
    quint32 fileSize;
    quint32 packetSize;
    QTcpSocket socket;
    QFile * file;
    QString currentName;
    quint32 bytesSent;
    quint32 dataSize;
    qint32 bytesWritten;
    ConnectionController * connection;
    quint32 lastSent;
    QTimer * uploadTimer;
    quint32 downloadSpeed;
    quint32 queuePosition;
    WidgetDownloadItem * widget;
    QListWidgetItem * itemPlaceholder;
    bool pending;
    int timeElapsed;
    ThreadUploader thread;

    qint32 init();

    CUpload();

public:
    void startUploading();
    void updateName();

public slots:
    void updateSpeed();
    void updateByteCount(qint64 bytes);
    void stopUpload();
    void threadFinished(int code);

signals:
    void gotError();
    void uploadFinished();
};

#endif // CUPLOAD_H
