#ifndef CDOWNLOAD_H
#define CDOWNLOAD_H
#include <QTcpSocket>
#include <QProgressBar>
#include <QFile>
#include <QString>
#include <QTimer>
#include "ConnectionController.h"
#include "widgetdownloaditem.h"
#include <QListWidgetItem>

class CDownload : public QObject
{
    Q_OBJECT
public:
    bool downloadInProgress;
    bool finished;
    unsigned int referenceNumber;
    unsigned int fileSize;
    unsigned int timeElapsed;
    QTcpSocket socket;
    QFile * file;
    QString currentName;
    QString pathOnServer;
    unsigned int bytesRead;
    unsigned int bytesReadThisSession;
    int dataSize;
    unsigned int bytesWritten;
    ConnectionController * connection;
    unsigned int lastRead;
    QTimer * downloadTimer;
    unsigned int downloadSpeed;
    unsigned int queuePosition;
    WidgetDownloadItem * widget;
    QListWidgetItem * itemPlaceholder;
    bool inited;
    bool forced;
    bool matched;

    bool gotForkSize;
    qint32 forkSize;

    int init();

    CDownload();

public:
    void startDownloading();
    void updateName();

public slots:
    void serverReady();
    void gotData();
    void updateSpeed();
    void stopDownload();
    void forceDownload();
    void queueDownload();

signals:
    void gotError();
    void downloadFinished();
    void forcedDownload(CDownload *);
};

#endif // CDOWNLOAD_H
