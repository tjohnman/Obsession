#include "cdownload.h"
#include <QAbstractSocket>
#include <QIODevice>
#include <QHostAddress>
#include <QStandardPaths>

#include "TextHelper.h"

CDownload::CDownload()
{
    finished = false;
    downloadInProgress = false;
    referenceNumber = -1;
    fileSize = 0;
    currentName = "Waiting for name...";
    bytesRead = 0;
    dataSize = 0;
    bytesWritten = 0;
    lastRead = 0;
    downloadTimer = new QTimer();
    connect(downloadTimer, SIGNAL(timeout()), this, SLOT(updateSpeed()));
    downloadSpeed = 0;
    queuePosition = -1;
    pathOnServer = "/";
    connection = NULL;
    file = new QFile();
    inited = false;
    forced = false;
    matched = false;
}

void CDownload::stopDownload() {
    if(inited) {
        if(!finished && connection->isConnected()) {
            socket.close();
            socket.disconnect(this, SLOT(gotData()));
            downloadInProgress = false;
            inited = false;
            file->close();
            downloadTimer->stop();
            if(downloadInProgress) {
                widget->infoLabel()->setText("Stopped");
            } else {
                widget->infoLabel()->setText("Cancelled");
            }
            finished = true;
            emit downloadFinished();
        }
    } else {
        widget->infoLabel()->setText("Cancelled");
        finished = true;
    }
    widget->goButton()->setEnabled(true);
    widget->stopButton()->setEnabled(false);
    widget->queueButton()->setEnabled(true);
    forced = false;
}

void CDownload::updateSpeed() {
    QString speedString;
    float speed = bytesRead - lastRead;
    lastRead = bytesRead;
    float size = fileSize, read = bytesRead;
    QString sizeString, readString, timeString;
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

    timeElapsed++;

    float averageSecondsPerByte = (float)timeElapsed/(float)bytesReadThisSession;
    quint32 secondsRemaining = (fileSize - bytesRead)*averageSecondsPerByte;

    if(secondsRemaining >= 60) {
        quint32 minutesRemaining = 0;
        while(secondsRemaining > 60) {
            minutesRemaining++;
            secondsRemaining -= 60;
        }
        if(minutesRemaining >= 60) {
            quint32 hoursRemaining = 0;
            while(minutesRemaining > 60) {
                hoursRemaining++;
                minutesRemaining -= 60;
            }
            if(hoursRemaining >= 24) {
                quint32 daysRemaining = 0;
                while(hoursRemaining > 24) {
                    daysRemaining++;
                    hoursRemaining -= 24;
                }
                if(daysRemaining > 0) {
                    timeString = QString::number(daysRemaining)+(daysRemaining == 1 ? " day " : " days ") +
                                 QString::number(hoursRemaining)+(hoursRemaining == 1 ? " hour " : " hours ") +
                                 QString::number(minutesRemaining)+(minutesRemaining == 1 ? " minute " : " minutes ") +
                                 QString::number(secondsRemaining)+(secondsRemaining == 1 ? " second " : " seconds ")+"remaining";
                } else {
                    timeString = QString::number(hoursRemaining)+(hoursRemaining == 1 ? " hour " : " hours ") +
                                 QString::number(minutesRemaining)+(minutesRemaining == 1 ? " minute " : " minutes ") +
                                 QString::number(secondsRemaining)+(secondsRemaining == 1 ? " second " : " seconds ")+"remaining";
                }
            } else {
                if(hoursRemaining > 0) {
                    timeString = QString::number(hoursRemaining)+(hoursRemaining == 1 ? " hour " : " hours ") +
                                 QString::number(minutesRemaining)+(minutesRemaining == 1 ? " minute " : " minutes ") +
                                 QString::number(secondsRemaining)+(secondsRemaining == 1 ? " second " : " seconds ")+"remaining";
                } else {
                    timeString = QString::number(minutesRemaining)+(minutesRemaining == 1 ? " minute " : " minutes ") +
                                 QString::number(secondsRemaining)+(secondsRemaining == 1 ? " second " : " seconds ")+"remaining";
                }
            }
        } else {
            if(minutesRemaining > 0) {
                timeString = QString::number(minutesRemaining)+(minutesRemaining == 1 ? " minute " : " minutes ") +
                             QString::number(secondsRemaining)+(secondsRemaining == 1 ? " second " : " seconds ")+"remaining";
            } else {
                timeString = QString::number(secondsRemaining)+(secondsRemaining == 1 ? " second " : " seconds ")+"remaining";
            }
        }
    } else {
        timeString = QString::number(secondsRemaining)+(secondsRemaining == 1 ? " second " : " seconds ")+"remaining";
    }

    widget->infoLabel()->setText(timeString+" - "+readString+" of "+sizeString+" ("+speedString+")");
}

void CDownload::updateName() {
    widget->nameLabel()->setText(currentName);
}

void CDownload::forceDownload() {
    forced = true;
    inited = false;
    finished = false;
    downloadInProgress = false;
    widget->goButton()->setEnabled(false);
    emit forcedDownload(this);
}

qint32 CDownload::init() {
    matched = true;
    timeElapsed = 0;
    bytesReadThisSession = 1;
    downloadInProgress = false;
    inited = true;

    widget->goButton()->setEnabled(false);
    widget->stopButton()->setEnabled(true);
    widget->queueButton()->setEnabled(true);
    widget->infoLabel()->setText("Preparing...");
    forced = false;

    if(queuePosition > 0) {
        widget->infoLabel()->setText("Queued by server at position "+QString::number(queuePosition));
        return 0;
    }

    startDownloading();

    return 0;
}

void CDownload::queueDownload() {
    if(inited) {
        if(!finished && connection->isConnected()) {
            socket.close();
            socket.disconnect(this, SLOT(gotData()));
            file->close();
            downloadTimer->stop();
        }
    }
    widget->goButton()->setEnabled(true);
    widget->stopButton()->setEnabled(true);
    widget->queueButton()->setEnabled(false);
    finished = false;
    downloadInProgress = false;
    inited = false;
    forced = false;
    widget->infoLabel()->setText("Queued");
}

void CDownload::startDownloading() {
    if(connection->pSocket.isValid() && connection->pSocket.state() == QAbstractSocket::ConnectedState) {
        socket.connectToHost(connection->pSocket.peerAddress(), connection->pSocket.peerPort()+1, QIODevice::ReadWrite);
        socket.waitForConnected();
        if(socket.state() == !QAbstractSocket::ConnectedState) {
            return;
        }

        gotForkSize = false;

        socket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
        connect(&socket, SIGNAL(readyRead()), this, SLOT(serverReady()));

        char magic[17];
        memcpy(magic, "HTXF", 4);
        quint32 ref = qToBigEndian(referenceNumber);
        memcpy(magic+4, &ref, 4);
        memset(magic+8, 0, 8);
        magic[16] = '\0';
        socket.write(magic, 16);
        socket.waitForBytesWritten();
    } else {
        return;
    }
}

void CDownload::serverReady() {
    if(!gotForkSize) {
        if(socket.bytesAvailable() < 40) {
            qDebug() << "Still need more bytes for header, waiting...";
            return;
        }
        downloadInProgress = true;
        disconnect(&socket, SIGNAL(readyRead()), this, SLOT(serverReady()));

        char * flatHeader = socket.read(24).data();

        if(strncmp(flatHeader, "FILP", 4)) {
            socket.close();
            downloadInProgress = false;
            emit gotError();
            return;
        }

        char * forkHeader = socket.read(16).data();

        if(strncmp(forkHeader, "INFO", 4)) {
            socket.close();
            downloadInProgress = false;
            emit gotError();
            return;
        }

        forkSize = 0;

        memcpy(&forkSize, forkHeader+12, 4);
        forkSize = qFromBigEndian(forkSize);

        gotForkSize = true;
    }

    if(socket.bytesAvailable() < forkSize) {
        return;
    }

    char * forkInfo = socket.read(forkSize).data();

    qint32 nameSize = forkSize - 72;

    bool hxd = false;

    char * reportedName = (char *)malloc(nameSize);
    memcpy(reportedName, forkInfo+72, nameSize);

    if(!strncmp(reportedName, "hxd", 3)) {
        qDebug() << "Server is hxd.";
        hxd = true;
    } else {
        qDebug() << "Server reports name: " << reportedName;
    }

    char * dataHeader = socket.read(16).data();

    if(strncmp(dataHeader, "DATA", 4)) {
        qDebug() << "DATA header was expected, but got something else (" << dataHeader << ").";
        socket.close();
        downloadInProgress = false;
        emit gotError();
        return;
    }

    memcpy(&fileSize, dataHeader+12, 4);
    fileSize = qFromBigEndian(fileSize);

    qDebug() << "Reported file size is " << fileSize;

    bytesRead = 0;
    widget->progressBar()->setMaximum(fileSize);
    widget->progressBar()->setValue(bytesRead);

    qDebug() << "Opening file for writing...";
    const QString downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    file->setFileName(downloadsFolder + "/" + currentName);
    if(file->exists()) {
        qDebug() << "Wil append to file";
        if(!file->open(QIODevice::Append)) {
            qDebug() << "Could not open file for writing!";
            return;
        }
    } else {
        if(!file->open(QIODevice::WriteOnly)) {
            qDebug() << "Could not open file for writing!";
            return;
        }
    }

    bytesWritten = 0;

    downloadTimer->start(1000);

    connect(&socket, SIGNAL(readyRead()), this, SLOT(gotData()));
}

void CDownload::gotData() {
    //qDebug() << "Got data (" << socket.bytesAvailable() << ")";
    qint32 read = socket.bytesAvailable();
    if(read <= 0) {
        // Still nothing to read.
        return;
    }
    char * data = socket.read(read).data();
    qint64 written = file->write(data, read);
    bytesWritten += written;
    if(written != read) {
        qDebug() << "Warning: some data could not be written!";
    }
    bytesRead += read;
    bytesReadThisSession += read;
    widget->progressBar()->setValue(bytesRead);

    if(bytesWritten >= fileSize) {
        qDebug() << "Download complete";
        qDebug() << "Wrote " << bytesWritten << " bytes";
        socket.close();
        socket.disconnect(this, SLOT(gotData()));
        bytesRead = 0;
        dataSize = 0;
        widget->progressBar()->setValue(10);
        widget->progressBar()->setMaximum(10);
        widget->infoLabel()->setText("Completed");
        downloadInProgress = false;
        file->flush();
        file->close();
        downloadTimer->stop();
        finished = true;
        widget->goButton()->setEnabled(false);
        widget->stopButton()->setEnabled(false);
        widget->queueButton()->setEnabled(false);
        emit downloadFinished();
    }
}
