#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

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

#include "cdownload.h"
#include "widgetdownloaditem.h"
#include "connectioncontroller.h"
#include "defines.h"

class DownloadManager : public QObject
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

    std::vector<CDownload *> downloads;

    qint32 init(quint32 reference, quint32 size);

    void sendDownloadRequestToServer(CDownload *);

    DownloadManager(ConnectionController * c);

private slots:

public slots:
    void addDownload(quint32, quint32, quint32);
    void onRequestedFile(QString name, qint32 size, QString path);
    void onQueueUpdate(quint32, quint32);
    uint cleanIdle();
    void onDownloadFinished();
    void onForcedDownload(CDownload *);

signals:
    void newItem(QListWidgetItem *);
};

#endif // DOWNLOADMANAGER_H
