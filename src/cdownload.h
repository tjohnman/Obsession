#ifndef CDOWNLOAD_H
#define CDOWNLOAD_H
#include <QTcpSocket>
#include <QProgressBar>
#include <QFile>
#include <QString>
#include <QTimer>
#include "connectioncontroller.h"
#include "widgetdownloaditem.h"
#include <QListWidgetItem>

class CDownload : public QObject
{
    Q_OBJECT
public:
    bool downloadInProgress;
    bool finished;
    quint32 referenceNumber;
    quint32 fileSize;
    quint32 timeElapsed;
    QTcpSocket socket;
    QFile * file;
    QString currentName;
    QString pathOnServer;
    quint32 bytesRead;
    quint32 bytesReadThisSession;
    qint32 dataSize;
    quint32 bytesWritten;
    ConnectionController * connection;
    quint32 lastRead;
    QTimer * downloadTimer;
    quint32 downloadSpeed;
    quint32 queuePosition;
    WidgetDownloadItem * widget;
    QListWidgetItem * itemPlaceholder;
    bool inited;
    bool forced;
    bool matched;

    bool gotForkSize;
    qint32 forkSize;

    qint32 init();

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
    void gotError(QString);
    void downloadFinished();
    void forcedDownload(CDownload *);
};

#endif // CDOWNLOAD_H
