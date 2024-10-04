#include "cupload.h"
#include <QAbstractSocket>
#include <QIODevice>
#include <QHostAddress>
#include "TextHelper.h"

CUpload::CUpload()
{
    finished = false;
    uploadInProgress = false;
    referenceNumber = -1;
    fileSize = 0;
    currentName = "Waiting for name...";
    bytesSent = 0;
    dataSize = 0;
    bytesWritten = 0;
    lastSent = 0;
    uploadTimer = new QTimer();
    downloadSpeed = 0;
    queuePosition = -1;
    file = new QFile();
    pending = true;

    connect(uploadTimer, SIGNAL(timeout()), this, SLOT(updateSpeed()));
}

void CUpload::updateByteCount(qint64 bytes)
{
    bytesSent = bytes;
}

void CUpload::stopUpload() {
    if(!finished) {
        socket.close();
        socket.disconnect(this, SLOT(sentData(qint64)));
        file->close();
        uploadTimer->stop();
        if(uploadInProgress) {
            widget->infoLabel()->setText("Stopped");
        } else {
            widget->infoLabel()->setText("Cancelled");
        }
        uploadInProgress = false;
        finished = true;
    }
}

void CUpload::updateSpeed() {
    QString speedString;
    float speed;
    speed = bytesSent - lastSent;
    lastSent = bytesSent;
    float size = dataSize, read = bytesSent;
    QString sizeString, readString;
    if(size > 1024) {
        if(size > 1024*1024*1024) {
            sizeString = QString("%1 GB").arg(size/1024/1024/1024, 0, 'f', 2);
            readString = QString("%1 GB").arg(read/1024/1024/1024, 0, 'f', 2);
        } else {
            if(size > 1024*1024) {
                sizeString = QString("%1 MB").arg(size/1024/1024, 0, 'f', 2);
                readString = QString("%1 MB").arg(read/1024/1024, 0, 'f', 2);
            } else {
                sizeString = QString("%1 KB").arg(size/1024, 0, 'f', 2);
                readString = QString("%1 KB").arg(read/1024, 0, 'f', 2);
            }
        }
    } else {
        sizeString = QString("%1 bytes").arg(size, 0, 'f', 0);
        readString = QString("%1 bytes").arg(read, 0, 'f', 0);
    }

    if(speed > 1024) {
        if(speed > 1024*1024*1024) {
            speedString = QString("%1 GB/s").arg(speed/1024/1024/1024, 0, 'f', 2);
        } else {
            if(speed > 1024*1024) {
                speedString = QString("%1 MB/s").arg(speed/1024/1024, 0, 'f', 2);
            } else {
                speedString = QString("%1 KB/s").arg(speed/1024, 0, 'f', 2);
            }
        }
    } else {
        speedString = QString("%1 bytes/s").arg(speed, 0, 'f', 0);
    }

    widget->infoLabel()->setText(readString+" of "+sizeString+" ("+speedString+")");
    widget->progressBar()->setValue(bytesSent);
}

void CUpload::updateName() {
    widget->nameLabel()->setText(currentName);
}

qint32 CUpload::init() {
    uploadInProgress = false;
    dataSize = fileSize;
    widget->progressBar()->setMaximum(dataSize);
    widget->progressBar()->setValue(0);
    widget->stopButton()->setEnabled(true);

    startUploading();

    return 0;
}

void CUpload::startUploading() {
    if(connection->pSocket.isValid() && connection->pSocket.state() == QAbstractSocket::ConnectedState) {

        socket.connectToHost(connection->pSocket.peerAddress(), connection->pSocket.peerPort()+1, QIODevice::ReadWrite);
        socket.waitForConnected();
        if(socket.state() != QAbstractSocket::ConnectedState) {
            qDebug() << "Upload connection timed out.";
            threadFinished(4);
            return;
        }

        if(!file->open(QIODevice::ReadOnly)) {
            qDebug() << "Could not open file for reading.";
            threadFinished(3);
            return;
        }

        uploadInProgress = true;

        socket.write("HTXF");
        quint32 revRef = qToBigEndian(referenceNumber);
        socket.write((const char *)&revRef, 4);
        socket.write("\0\0\0\0\0\0\0\0", 8);
        socket.write("FILP\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\2INFO\0\0\0\0\0\0\0\0", 36);
        quint16 nameSize = TextHelper::EncodeText(currentName).size();
        quint32 infoForkSize = 72 + nameSize;
        quint32 revInfoForkSize = qToBigEndian(infoForkSize);
        socket.write((const char *)&revInfoForkSize, 4);
        socket.write("MWIN????????\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 70);
        quint16 revNameSize = qToBigEndian(nameSize);
        socket.write((const char *)&revNameSize, 2);

        socket.write(TextHelper::EncodeText(currentName).data(), nameSize);
        socket.write("DATA\0\0\0\0\0\0\0\0", 12);
        quint32 revDataSize = qToBigEndian(dataSize);
        socket.write((const char *)&revDataSize, 4);

        socket.waitForBytesWritten();

        if(socket.state() != QAbstractSocket::ConnectedState) {
            qDebug() << "Server closed the connection after receiving header.";
            threadFinished(2);
            return;
        }

        timeElapsed = 0;
        uploadTimer->start(1000);

        connect(&thread, SIGNAL(sentBytes(qint64)), this, SLOT(updateByteCount(qint64)));
        connect(&thread, SIGNAL(complete(int)), this, SLOT(threadFinished(int)));

        socket.moveToThread(&thread);
        thread.socket = &socket;
        thread.file = file;
        thread.bytesSent = 0;
        thread.dataSize = dataSize;
        thread.start();
    } else {
        qDebug() << "Warning: Must connect to upload files.";
        return;
    }
}

void CUpload::threadFinished(int code)
{
    switch(code)
    {
    case 0:
        bytesSent = 0;
        dataSize = 0;
        widget->progressBar()->setValue(10);
        widget->progressBar()->setMaximum(10);
        widget->infoLabel()->setText("Completed");
        uploadInProgress = false;
        file->close();
        uploadTimer->stop();
        finished = true;
        emit uploadFinished();
        break;
    default:
    case 1:
        qDebug() << "Data sending failed.";
        widget->infoLabel()->setText("Upload interrupted.");
        uploadTimer->stop();
        finished = true;
        uploadInProgress = false;
        break;
    case 2:
        widget->infoLabel()->setText("Server rejected the operation. Check encoding settings.");
        uploadTimer->stop();
        finished = true;
        uploadInProgress = false;
        break;
    case 3:
        widget->infoLabel()->setText("Error reading file.");
        uploadTimer->stop();
        finished = true;
        uploadInProgress = false;
        break;
    case 4:
        widget->infoLabel()->setText("Could not connect for transfer. Check encoding settings.");
        uploadTimer->stop();
        finished = true;
        uploadInProgress = false;
        break;
    }
}
