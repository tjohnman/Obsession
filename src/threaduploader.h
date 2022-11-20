#ifndef THREADUPLOADER_H
#define THREADUPLOADER_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>

class ThreadUploader : public QThread
{
    Q_OBJECT
public:
    explicit ThreadUploader(QObject *parent = 0);
    QTcpSocket * socket;
    QFile * file;

    qint32 bytesSent, dataSize;
    
signals:
    void sentBytes(qint64 bytes);
    void complete(int code);

private:
    QTimer * timer;

private slots:

protected:
    void run();
    
public slots:
    
};

#endif // THREADUPLOADER_H
