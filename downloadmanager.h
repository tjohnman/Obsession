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

#include "CDownload.h"
#include "widgetdownloaditem.h"
#include "ConnectionController.h"
#include "defines.h"

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    bool downloadInProgress;
    unsigned int referenceNumber;
    unsigned int fileSize;
    QTcpSocket socket;
    QProgressBar * progressBar;
    QFile * file;
    QString currentName;
    unsigned int bytesRead;
    unsigned int dataSize;
    int bytesWritten;
    ConnectionController * connection;
    unsigned int lastRead;
    QTimer * downloadsTimer;
    unsigned int downloadSpeed;
    QListWidget * listWidget;

    std::vector<CDownload *> downloads;

    int init(unsigned int reference, unsigned int size);

    void sendDownloadRequestToServer(CDownload *);

    DownloadManager(ConnectionController * c);

private slots:

public slots:
    void addDownload(unsigned int, unsigned int, unsigned int);
    void onRequestedFile(QString name, int size, QString path);
    void onQueueUpdate(unsigned int, unsigned int);
    uint cleanIdle();
    void onDownloadFinished();
    void onForcedDownload(CDownload *);

signals:
    void newItem(QListWidgetItem *);
};

#endif // DOWNLOADMANAGER_H
