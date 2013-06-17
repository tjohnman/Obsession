#include "dialogtrackers.h"
#include "ui_dialogtrackers.h"
#include <QSettings>
#include <QtEndian>
#include <QTextCodec>
#include <QTextStream>
#include <QHostAddress>

DialogTrackers::DialogTrackers(ConnectionController * c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTrackers)
{
    ui->setupUi(this);
    connection = c;
    pSocket = new QTcpSocket(this);
    
    pTrackerNames.clear();
    pTrackerAddresses.clear();

    updateTrackerList();
    updateServerList(ui->comboBox->currentText());

    addTrackerDialog = new DialogAddTracker(this);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateServerList(QString)));
    connect(ui->pushButton, SIGNAL(clicked()), addTrackerDialog, SLOT(show()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(deleteTracker()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(openConnectionWindow()));
    connect(addTrackerDialog, SIGNAL(accepted()), this, SLOT(addTracker()));
    connect(ui->treeWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openConnectionWindow()));
    connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(ui->buttonRefresh, SIGNAL(clicked()), this, SLOT(updateCurrentList()));
}

void DialogTrackers::connectToServer(QModelIndex model) {
    QString address = ui->treeWidget->topLevelItem(model.row())->data(3, 0).toString();
    QString port = ui->treeWidget->topLevelItem(model.row())->data(4, 0).toString();
    qDebug() << "Double clicked: " << address << ":" << port;
    connection->closeConnection();

    if(!connection->connectToServer(address+":"+port, "", "")) {
        close();
    }
}

void DialogTrackers::socketError(QAbstractSocket::SocketError e) {
    qDebug() << "Socket error while retrieving server list: " << e;
}

void DialogTrackers::addTracker() {
    qDebug() << "Adding tracker";
    if(!addTrackerDialog->name().isEmpty() && !addTrackerDialog->address().isEmpty()) {
        pTrackerNames.push_back(addTrackerDialog->name());
        pTrackerAddresses.push_back(addTrackerDialog->address());
        saveTrackerList();
        updateTrackerList();
        ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
    } else {
        qDebug() << "Empty fields";
    }
}

void DialogTrackers::deleteTracker() {
    if(ui->comboBox->count() > 0) {
        for(quint32 i=0; i<pTrackerNames.size(); i++) {
            if(pTrackerNames[i] == ui->comboBox->currentText()) {
                pTrackerNames[i] = pTrackerNames.back();
                pTrackerNames.pop_back();

                pTrackerAddresses[i] = pTrackerAddresses.back();
                pTrackerAddresses.pop_back();
                break;
            }
        }
        saveTrackerList();
        updateTrackerList();
        if(ui->comboBox->count() > 0)
        {
            ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
        }
    }
}

void DialogTrackers::saveTrackerList() {
    QSettings settings("mir", "contra");
    settings.setValue("numtrackers", (qint32)pTrackerAddresses.size());
    qint32 numTrackers = settings.value("numtrackers").toInt();
    qDebug() << "Saving " << numTrackers << " trackers...";
    for(qint32 i=0; i<numTrackers; i++) {
        settings.setValue(QString("tracker")+QString::number(i), pTrackerNames[i]);
        settings.setValue(QString("trackeradd")+QString::number(i), pTrackerAddresses[i]);
    }
}

void DialogTrackers::updateTrackerList() {
    QSettings settings("mir", "contra");
    qint32 numTrackers = settings.value("numtrackers").toInt();
    qDebug() << "Loading " << numTrackers << " trackers...";
    pTrackerNames.clear();
    pTrackerAddresses.clear();
    ui->comboBox->clear();
    for(qint32 i=0; i<numTrackers; i++) {
        pTrackerNames.push_back(settings.value(QString("tracker")+QString::number(i)).toString());
        pTrackerAddresses.push_back(settings.value(QString("trackeradd")+QString::number(i)).toString());
        ui->comboBox->addItem(pTrackerNames.back());
    }
}

void DialogTrackers::updateCurrentList() {
    updateServerList(ui->comboBox->currentText());
}

void DialogTrackers::updateServerList(QString tracker) {
    ui->label->setText("loading...");
    qDebug() << "Updating server list for tracker " << tracker;
    if(ui->comboBox->count() == 0) {
        return;
    }
    QString address;

    ui->treeWidget->clear();

    for(quint32 i=0; i<pTrackerNames.size(); i++) {
        qDebug() << "Searching...";
        if(pTrackerNames[i] == tracker) {
            qDebug() << "Found address in list";
            address = pTrackerAddresses[i];
        }
    }

    pSocket->disconnect(this, SLOT(onSocketData()));
    while(pSocket->isOpen()) {pSocket->close();}
    delete pSocket;
    pSocket = new QTcpSocket();

    qDebug() << "Connecting...";
    connect(pSocket, SIGNAL(connected()), this, SLOT(sendRequest()));
    pSocket->connectToHost(address, 5498, QIODevice::ReadWrite);
}

void DialogTrackers::sendRequest() {
    qDebug() << "Connected to " << pSocket->peerAddress() << " on port " << pSocket->peerPort();

    QByteArray magic;
    magic.append("HTRK\x00\x01", 6);
    qDebug() << "Sending magic " << pSocket->write(magic.data(), 6) << "bytes";
    qDebug() << (qint8)magic[0] << " " << (qint8)magic[1] << " " << (qint8)magic[2] << " " << (qint8)magic[3] << " " << (qint8)magic[4] << " " << (qint8)magic[5];
    pSocket->waitForBytesWritten();
    qDebug() << "Sent magic";
    pSocket->waitForReadyRead(10000);
    char * response = pSocket->read(6).data();
    if(strncmp(magic, response, 6) != 0) {
        qDebug() << "Connecting to tracker failed";
        qDebug() << (quint8) response[0] << " " << (quint8) response[1] << " " << (quint8) response[2] << " " << (quint8) response[3] << " " << (quint8) response[4] << " " << (quint8) response[5];
        ui->label->setText("Connecting to tracker failed.");
        return;
    }
    qDebug() << "Handshake successful";
    gotHeader = false;
    connect(pSocket, SIGNAL(readyRead()), this, SLOT(onSocketData()));
    onSocketData();
}

void DialogTrackers::onSocketData() {
    qDebug() << "Getting header...";

    quint16 confirm;
    
    char * buffer;
    if(!gotHeader) {
        ui->label->setText("0 servers");
        numServers = 0;

        buffer = (char *) malloc(2);
        buffer = pSocket->read(2).data();
        memcpy(&confirm, buffer, 2);
        confirm = qFromBigEndian(confirm);
        qDebug() << "Server confirms: " << confirm;

        buffer = pSocket->read(2).data();
        memcpy(&dataLength, buffer, 2);
        dataLength = qFromBigEndian(dataLength);
        qDebug() << "Length of data: " << dataLength;

        buffer = pSocket->read(2).data();
        memcpy(&numberOfServers, buffer, 2);
        numberOfServers = qFromBigEndian(numberOfServers);
        qDebug() << "Number of servers: " << numberOfServers;
        gotHeader = true;
        lastPassBytes = 0;
        totalBytes = 0;
        pSocket->read(2);
    }

    totalBytes += pSocket->bytesAvailable() - lastPassBytes;
    lastPassBytes = pSocket->bytesAvailable();

    if(totalBytes < dataLength - 4) {
        qDebug() << "Waiting for all data to come in...";
        return;
    }

    while(pSocket->bytesAvailable()) {
        qDebug() << "--------------------------";
        qDebug() << pSocket->bytesAvailable() << " bytes available. Reading...";
        qDebug() << "--------------------------";

        QString name;
        QString description;
        QString address;
        quint16 port, users;

        unsigned char len;

        buffer = (char *) malloc(5);
        memset(buffer, 0, 5);

        quint16 a, b, c, d;
        QByteArray ipbuffer = pSocket->read(4).data();

        a = (unsigned char) ipbuffer[0];
        b = (unsigned char) ipbuffer[1];
        c = (unsigned char) ipbuffer[2];
        d = (unsigned char) ipbuffer[3];
        address = QString::number(a) + QString(".") +
                  QString::number(b) + QString(".") +
                  QString::number(c) + QString(".") +
                  QString::number(d);

        qDebug() << "Address: " << address;

        char * portbuffer = (char *) malloc(2);
        memset(portbuffer, 0, 2);

        portbuffer = pSocket->read(2).data();
        memcpy(&port, portbuffer, 2);
        port = qFromBigEndian(port);
        qDebug() << "Port: " << port;

        char * usersbuffer = (char *) malloc(2);
        usersbuffer = pSocket->read(2).data();
        memcpy(&users, usersbuffer, 2);
        users = qFromBigEndian(users);
        qDebug() << "Users: " << users;

        pSocket->read(2); // 2 bytes wasted in the protocol? WHY GOD WHY???

        buffer = (char *) malloc(1);
        memset(buffer, 0, 1);

        buffer = pSocket->read(1).data();
        len = 0;
        memcpy(&len, buffer, 1);
        qDebug() << "Name is " << (qint32)len << " bytes long";

        QTextStream stream(pSocket->read(len));
        stream.setCodec("Shift-JIS");
        name = stream.readAll();
        //name = QString::fromLocal8Bit(pSocket->read(len).data(), len);
        qDebug() << "Name: " << name;

        memset(buffer, 0, 1);

        buffer = pSocket->read(1).data();
        len = 0;
        memcpy(&len, buffer, 1);
        qDebug() << "Description is " << (qint32)len << " bytes long";

        QTextStream stream2(pSocket->read(len));
        stream2.setCodec("Shift-JIS");
        description = stream2.readAll();
        qDebug() << "Description: " << description;

        QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << name << QString::number(users) << description << address << QString::number(port));
        item->setToolTip(0, name);
        item->setToolTip(2, description);
        item->setTextAlignment(1, Qt::AlignHCenter);
        ui->treeWidget->addTopLevelItem(item);
        ui->label->setText(QString::number(ui->treeWidget->topLevelItemCount())+" servers");
    }
    numServers = ui->treeWidget->topLevelItemCount();
}

DialogTrackers::~DialogTrackers()
{
    delete ui;
    delete addTrackerDialog;
    pSocket->close();
    delete pSocket;
}

void DialogTrackers::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogTrackers::openConnectionWindow() {
    if(ui->treeWidget->selectedItems().count()) {
        QModelIndex model = ui->treeWidget->currentIndex();
        QString address = ui->treeWidget->topLevelItem(model.row())->data(3, 0).toString();
        QString port = ui->treeWidget->topLevelItem(model.row())->data(4, 0).toString();
        DialogOpenConnection * dialog = new DialogOpenConnection(0, connection);

        if(port != "5500") {
            dialog->setAddress(address+":"+port);
        } else {
            dialog->setAddress(address);
        }

        connect(dialog, SIGNAL(accepted()), this, SLOT(close()));

        dialog->show();
        dialog->setFocusOnLogin();
    }
}
