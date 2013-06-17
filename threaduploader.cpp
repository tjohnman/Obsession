#include "threaduploader.h"

ThreadUploader::ThreadUploader(QObject *parent) :
    QThread(parent)
{

}

void ThreadUploader::run()
{
    int packetSize = 1024;
    qint64 sent;
    QByteArray readData;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1000);

    while(dataSize > bytesSent)
    {
        if(dataSize - bytesSent < packetSize) {
            readData = file->read(dataSize - bytesSent);
            sent = socket->write(readData, readData.size());
        } else {
            readData = file->read(packetSize);
            sent = socket->write(readData, readData.size());
        }

        if(!socket->waitForBytesWritten() || sent == -1)
        {
            socket->close();
            emit complete(1);
            return;
        }

        bytesSent += sent;
        if(bytesSent % 102400 == 0)
        {
            emit sentBytes(bytesSent);
        }
    }

    socket->close();
    emit complete(0);
    return;
}

void ThreadUploader::timerUpdate()
{
    emit sentBytes(bytesSent);
}
