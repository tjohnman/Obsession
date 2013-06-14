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

#include "CUpload.h"
#include "widgetdownloaditem.h"
#include "ConnectionController.h"
#include "defines.h"

class UploadManager : public QObject
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

    std::vector<CUpload *> uploads;

    int init(unsigned int reference, unsigned int size);

    UploadManager(ConnectionController * c);

private slots:

public slots:
    void addUpload(unsigned int);
    void onRequestedFile(QString name, int size);
    uint cleanIdle();

signals:
    void newItem(QListWidgetItem *);
};

#endif // UPLOADMANAGER_H
