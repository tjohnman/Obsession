#ifndef CUPLOAD_H
#define CUPLOAD_H

#include <QTcpSocket>
#include <QProgressBar>
#include <QFile>
#include <QString>
#include <QTimer>
#include "ConnectionController.h"
#include "widgetdownloaditem.h"
#include <QListWidgetItem>

class CUpload : public QObject
{
    Q_OBJECT
public:
    bool uploadInProgress;
    bool finished;
    unsigned int referenceNumber;
    unsigned int fileSize;
    unsigned int packetSize;
    QTcpSocket socket;
    QFile * file;
    QString currentName;
    unsigned int bytesSent;
    unsigned int dataSize;
    int bytesWritten;
    ConnectionController * connection;
    unsigned int lastSent;
    QTimer * uploadTimer;
    unsigned int downloadSpeed;
    unsigned int queuePosition;
    WidgetDownloadItem * widget;
    QListWidgetItem * itemPlaceholder;
    bool pending;

    int init();

    CUpload();

public:
    void startUploading();
    void updateName();

public slots:
    void updateSpeed();
    void stopUpload();

signals:
    void gotError();
    void uploadFinished();
};

#endif // CUPLOAD_H
