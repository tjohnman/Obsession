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
        qDebug() << "Upload stopped";
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
    if(speed == 0) return;
    lastSent = bytesSent;
    float size = dataSize, read = bytesSent;
    QString sizeString, readString;
    if(size > 1024) {
        if(size > 1024*1024*1024) {
            sizeString = sizeString.sprintf("%.2f GB", size/1024/1024/1024);
            readString = readString.sprintf("%.2f GB", read/1024/1024/1024);
        } else {
            if(size > 1024*1024) {
                sizeString = sizeString.sprintf("%.2f MB", size/1024/1024);
                readString = readString.sprintf("%.2f MB", read/1024/1024);
            } else {
                sizeString = sizeString.sprintf("%.2f KB", size/1024);
                readString = readString.sprintf("%.2f KB", read/1024);
            }
        }
    } else {
        sizeString = sizeString.sprintf("%.0f bytes", size);
        readString = readString.sprintf("%.0f bytes", read);
    }

    if(speed > 1024) {
        if(speed > 1024*1024*1024) {
            speedString = speedString.sprintf("%.2f GB/s", speed/1024/1024/1024); // Would love to live to see this :P
        } else {
            if(speed > 1024*1024) {
                speedString = speedString.sprintf("%.2f MB/s", speed/1024/1024);
            } else {
                speedString = speedString.sprintf("%.2f KB/s", speed/1024);
            }
        }
    } else {
        speedString = speedString.sprintf("%.0f bytes/s", speed);
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
        qDebug() << "Connecting...";
        socket.connectToHost(connection->pSocket.peerAddress(), connection->pSocket.peerPort()+1, QIODevice::ReadWrite);
        socket.waitForConnected();
        if(socket.state() != QAbstractSocket::ConnectedState) {
            qDebug() << "Upload connection timed out";
            return;
        }

        if(!file->open(QIODevice::ReadOnly)) {
            qDebug() << "Could not open file for reading";
            return;
        }

        uploadInProgress = true;
        qDebug() << "Sending magic...";

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
        qDebug() << "Name size is " << nameSize;
        socket.write(TextHelper::EncodeText(currentName).data(), nameSize);
        socket.write("DATA\0\0\0\0\0\0\0\0", 12);
        quint32 revDataSize = qToBigEndian(dataSize);
        socket.write((const char *)&revDataSize, 4);
        qDebug() << "Data size is " << dataSize;

        socket.waitForBytesWritten();

        if(socket.state() != QAbstractSocket::ConnectedState) {
            qDebug() << "Server closed the connection after receiving header";
            return;
        }

        qDebug() << "Starting actual upload";

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

        qDebug() << "Upload thread started.";

    } else {
        qDebug() << "Warning: Must connect to upload files";
        return;
    }
}

void CUpload::threadFinished(int code)
{
    switch(code)
    {
    case 0:
        qDebug() << "Upload complete";
        qDebug() << "Sent " << bytesSent << " bytes";
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
    case 1:
        qDebug() << "Data sending failed. Bailing out.";
        widget->infoLabel()->setText("Error: transfer failed.");
        uploadTimer->stop();
        finished = true;
        uploadInProgress = false;
        break;
    }
}
