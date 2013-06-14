#include "cupload.h"
#include <QAbstractSocket>
#include <QIODevice>
#include <QHostAddress>

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
    connect(uploadTimer, SIGNAL(timeout()), this, SLOT(updateSpeed()));
    downloadSpeed = 0;
    queuePosition = -1;
    file = new QFile();
    pending = true;
    packetSize = 204800;
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
    float speed = (bytesSent - lastSent)/(uploadTimer->interval()/1000.f);
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

    // Actual upload

    if(socket.state() == !QAbstractSocket::ConnectedState) {
        qDebug() << "Server closed the connection";
        widget->infoLabel()->setText("Error: the server closed the connection.");
        uploadTimer->stop();
        finished = true;
        uploadInProgress = false;
        return;
    }

    if(bytesSent >= dataSize) {
        qDebug() << "Upload complete";
        qDebug() << "Sent " << bytesSent << " bytes";
        socket.close();
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
        return;
    } else {
        unsigned int sent;

        if(dataSize - bytesSent < 4096) {
            sent = socket.write(file->read((dataSize - bytesSent)), (dataSize - bytesSent));
        } else {
            if((dataSize - bytesSent) < packetSize) {
                sent = socket.write(file->read((dataSize - bytesSent)), (dataSize - bytesSent));
            } else {
                sent = socket.write(file->read(packetSize), packetSize);
            }
        }

        socket.waitForBytesWritten(30000);
        bytesSent += sent;
    }

    widget->progressBar()->setValue(bytesSent);
    qDebug() << bytesSent << "/" << dataSize;
}

void CUpload::updateName() {
    widget->nameLabel()->setText(currentName);
}

int CUpload::init() {
    uploadInProgress = false;
    dataSize = fileSize;
    widget->progressBar()->setMaximum(dataSize);
    widget->progressBar()->setValue(0);

    startUploading();

    return 0;
}

void CUpload::startUploading() {
    if(connection->pSocket.isValid() && connection->pSocket.state() == QAbstractSocket::ConnectedState) {
        qDebug() << "Connecting...";
        socket.connectToHost(connection->pSocket.peerAddress(), connection->pSocket.peerPort()+1, QIODevice::ReadWrite);
        socket.waitForConnected();
        if(socket.state() == !QAbstractSocket::ConnectedState) {
            qDebug() << "DownloaderThread connection timed out";
            return;
        }

        if(!file->open(QIODevice::ReadOnly)) {
            qDebug() << "Could not open file for reading";
            return;
        }

        uploadInProgress = true;
        qDebug() << "Sending magic...";
        char magic[17] = {0x48, 0x54, 0x58, 0x46};
        qDebug() << "Reference is "<<referenceNumber;
        unsigned int ref = qToBigEndian(referenceNumber);
        memcpy(magic+4, &ref, 4);
        qDebug() << "Size is "<<fileSize;
        unsigned int s = qToBigEndian(fileSize);
        memcpy(magic+8, &s, 4);
        memset(magic+12, 0, 4);
        magic[16] = '\0';
        bytesSent = 0;
        socket.write(magic, 16);

        char flatheader[24];
        memcpy(flatheader, "FILP", 4);
        qint16 version = 1;
        version = qToBigEndian(version);
        qint16 forkcount = 2;
        forkcount = qToBigEndian(forkcount);
        memcpy(flatheader+4, &version, 2);
        memset(flatheader+6, 0, 16);
        memcpy(flatheader+22, &forkcount, 2);

        socket.write(flatheader, 24);

        unsigned short forksize = 72 + currentName.toAscii().length();  // We need to get the number of bytes, not the
                                                                        // number of characters. For Japanese chars.
        forksize = qToBigEndian(forksize);

        char forkheader[16];
        memcpy(forkheader, "INFO", 4);
        memset(forkheader+4, 0, 8);
        memcpy(forkheader+12, &forksize, 4);
        socket.write(forkheader, 16);

        char * infork = (char *) malloc(72 + currentName.toAscii().length());
        memcpy(infork, "MWIN", 4);
        memset(infork+4, '?', 8);
        memset(infork+12, 0, 58);
        unsigned short namesize = currentName.toAscii().length();
        namesize = qToBigEndian(namesize);
        memcpy(infork+70, &namesize, 2);
        memcpy(infork+72, currentName.toAscii().data(), currentName.toAscii().length());
        socket.write(infork, 72 + currentName.toAscii().length());
        socket.waitForBytesWritten();
        free(infork);

        char header[16] = {0x44, 0x41, 0x54, 0x41};
        memset(header+4, 0, 8);
        qDebug() << "Data size : " << dataSize;
        unsigned int siz = qToBigEndian(dataSize);
        memcpy(header+12, &siz, 4);
        socket.write(header, 16);
        socket.flush();

        qDebug() << "Starting actual upload";

        //uploadTimer->start(1);

    } else {
        qDebug() << "Warning: Must connect to upload files";
        return;
    }
}
