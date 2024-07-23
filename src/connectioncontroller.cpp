#include "connectioncontroller.h"
#include <QSettings>
#include <QStringList>
#include <QDir>
#include <QBrush>
#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QTextCodec>
#include "dialogprivatemessaging.h"

#include "transactionparameter.h"
#include "version.h"
#include "TextHelper.h"

ConnectionController::ConnectionController()
{
    pSocket.open(QIODevice::ReadWrite);
    connect(&pSocket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(&pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(&pTimeoutTimer, SIGNAL(timeout()), this, SLOT(onConnectionTimedOut()));

    pServerAgreement = QString();

    // TODO: Preferences
    QSettings settings("mir", "Contra");
    pNickname = settings.value("nick", "unnamed").toString();
    pIconID = (quint16) settings.value("icon", 25096).toString().toShort();

    pAFK = false;

    pClientVersion = 190;

    pTaskIDCounter = 0;

    pReconnectionAttempts = 0;
    receivedTransaction = NULL;

    pTimeoutTimer.setSingleShot(true);
    pTimeoutTimer.setInterval(10000);
}

bool ConnectionController::isConnected() {
    if(pSocket.state() == QAbstractSocket::ConnectedState) {
        return true;
    }
    return false;
}

qint32 ConnectionController::connectToServer(QString address, QString login, QString password, bool resetAutoReconnect) {

    if(resetAutoReconnect)
    {
        pReconnectionAttempts = 0;
    }
    else
    {
        ++pReconnectionAttempts;
    }

    emit connecting();
    pServerAddress = address;
    pPlainLogin = login;
    pPlainPassword = password;
    qint32 semicolonIndex = address.lastIndexOf(":");
    QString addr;
    quint16 port;

    if(semicolonIndex != -1) {
        addr = address.left(semicolonIndex);
        port = address.right(address.length()-semicolonIndex-1).toInt();
    } else {
        addr = address;
        port = 5500;
    }

    pLogin = TextHelper::EncodeText(login);

    for(qint32 i=0; i<pLogin.length(); i++) {
        pLogin[i] = 255 - pLogin[i];
    }

    pPassword = TextHelper::EncodeText(password);

    for(qint32 i=0; i<pPassword.length(); i++) {
        pPassword[i] = 255 - pPassword[i];
    }

    pTimeoutTimer.start();
    pSocket.connectToHost(addr, port);

    serverIdent = addr;

    return 0;
}

void ConnectionController::broadcast(QString text)
{
    CTransaction * trans = createTransaction(355);
    QByteArray arr = TextHelper::EncodeText(text);
    trans->addParameter(101, arr.size(), arr.data());
    sendTransaction(trans);
}

void ConnectionController::requestAccount(QString login)
{
    QByteArray loginArr = TextHelper::EncodeText(login);
    CTransaction * t = createTransaction(352);
    t->addParameter(105, loginArr.size(), loginArr.data());
    sendTransaction(t, true);
}

void ConnectionController::sendTransaction(CTransaction * t, bool expectReply) {
    if(isConnected()) {
        pSocket.write(t->bytes(), t->length());
        if(expectReply) {
            pPendingTransactions.push_back(t);
        } else {
            delete t;
        }
    }
}

QString ConnectionController::serverName() {
    return QString(pServerName.data());
}

void ConnectionController::sendChatText(QString text) {
    CTransaction * chatTransaction = new CTransaction(105, pTaskIDCounter++);
    chatTransaction->addParameter(101, TextHelper::EncodeText(text).size(), TextHelper::EncodeText(text).data());
    sendTransaction(chatTransaction);
}

void ConnectionController::sendEmote(QString text) {
    CTransaction * chatTransaction = new CTransaction(105, pTaskIDCounter++);
    chatTransaction->addParameter(101, TextHelper::EncodeText(text).size(), TextHelper::EncodeText(text).data());
    chatTransaction->addParameter(109, 1);
    sendTransaction(chatTransaction);
}

void ConnectionController::toggleAFK() {
    if(pAFK) {
        QSettings settings("mir", "Contra");
        pNickname = settings.value(QString("nick"), "unnamed").toString();
        sendUserInfo();
        sendEmote(QString("is back"));
        pAFK = false;
    } else {
        sendEmote(QString("is AFK"));
        QSettings settings("mir", "Contra");
        pNickname = settings.value(QString("nick"), "unnamed").toString() + QString(" (AFK)");
        pAFK = true;
        sendUserInfo();
    }
}

bool ConnectionController::isAFK() {
    return pAFK;
}

void ConnectionController::sendUserInfo() {
    QSettings settings("mir", "Contra");
    pNickname = settings.value(QString("nick"), "unnamed").toString();
    pIconID = settings.value("icon", 25096).toString().toShort();
    CTransaction * uinfoTransaction = new CTransaction(304, pTaskIDCounter++);
    uinfoTransaction->addParameter(102, TextHelper::EncodeText(pNickname).size(), TextHelper::EncodeText(pNickname).data());
    uinfoTransaction->addParameter(104, pIconID);
    sendTransaction(uinfoTransaction);
}

s_user * ConnectionController::getUserByUid(qint16 uid) {
    for(quint32 i=0; i<pUsers.size(); i++) {
        if(pUsers[i]->id == uid) {
            return pUsers[i];
        }
    }
    return NULL;
}

s_user * ConnectionController::getUserByName(QString name) {
    for(quint32 i=0; i<pUsers.size(); i++) {
        if(name.contains(QString(pUsers[i]->name))) {
            return pUsers[i];
        }
    }
    return NULL;
}

std::vector<s_user *> * ConnectionController::getUserList() {
    return &pUsers;
}

QString ConnectionController::serverAgreement() {
    return pServerAgreement;
}

void ConnectionController::requestUserInfo(quint16 id)
{
    qint32 task = pTaskIDCounter++;
    m_UserInfoTaskMap[task] = id;
    CTransaction * PMTransaction = new CTransaction(303, task);
    PMTransaction->addParameter(103, id);
    sendTransaction(PMTransaction, true);
}

void ConnectionController::sendPMToUser(quint16 uid, QString message, bool automatic) {
    CTransaction * PMTransaction = new CTransaction(108, pTaskIDCounter++);
    PMTransaction->addParameter(103, uid);
    if(automatic) {
        PMTransaction->addParameter(113, 4);
    } else {
        PMTransaction->addParameter(113, 1);
    }
    PMTransaction->addParameter(101, TextHelper::EncodeText(message).size(), TextHelper::EncodeText(message).data());
    sendTransaction(PMTransaction);
}

void ConnectionController::closeConnection(bool silent) {
    if(!silent && pServerAddress != "" && !pServerAddress.isEmpty())
    {
        emit gotChatMessage(QString("                <b>Disconnected from %1</b>").arg(pServerAddress));
    }
    pSocket.close();
    if(pSocket.state() != QAbstractSocket::UnconnectedState) {
        pSocket.waitForDisconnected(3000);
    }
    pSocket.disconnect(SIGNAL(readyRead()));
}

CTransaction * ConnectionController::createTransaction(qint16 id) {
    return new CTransaction(id, pTaskIDCounter++);
}

/************
    SLOTS
*************/

void ConnectionController::requestUserList()
{
    CTransaction * requestUserListTransaction = new CTransaction(300, pTaskIDCounter++);
    sendTransaction(requestUserListTransaction, true);
}

void ConnectionController::onSocketConnected() {
    pSocket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    pTimeoutTimer.stop();
    emit connected();

    t_protocolExtensions protocol_extensions = this->checkForProtocolExtensions();

    char clientMagicBytes[12] = {0x54, 0x52, 0x54, 0x50, 0x48, 0x4f, 0x54, 0x4c, 0x00, 0x01, 0x00, 0x02};
    char serverMagicBytes[8] = {0x54, 0x52, 0x54, 0x50, 0x00, 0x00, 0x00, 0x00};

    pSocket.write(clientMagicBytes, 12);
    pSocket.waitForReadyRead(30000);
    QByteArray response = pSocket.readAll();

    for(quint32 i=0; i<8; i++) {
        if(response.data()[i] != serverMagicBytes[i]) {
            qDebug() << "Handshake failed";
            return;
        }
    }

    if(pSocket.bytesAvailable()) {
        pSocket.readAll();
    }
    connect(&pSocket, SIGNAL(readyRead()), this, SLOT(onSocketData()));

    CTransaction * loginTransaction = new CTransaction(107, pTaskIDCounter++);
    loginTransaction->addParameter(105, pLogin.length(), pLogin.data());
    if(pPassword.length() > 0) {
        loginTransaction->addParameter(106, pPassword.length(), pPassword.data());
    }
    loginTransaction->addParameter(102, TextHelper::EncodeText(pNickname).size(), TextHelper::EncodeText(pNickname).data());

    quint16 iconID = qToBigEndian((quint16)3520);
    quint16 ver = qToBigEndian(pClientVersion);

    loginTransaction->addParameter(104, iconID);
    loginTransaction->addParameter(160, ver);

    if(protocol_extensions.pitbull) {
        loginTransaction->addParameter(163, 4, (char *)"OBSE");
        loginTransaction->addParameter(164, VERSION_MAJOR*10 + VERSION_MINOR);
    }

    pServerAgreement = "";

    sendTransaction(loginTransaction, true);
}

ConnectionController::t_protocolExtensions ConnectionController::checkForProtocolExtensions() {
    QTcpSocket * sock = new QTcpSocket();

    // Pitbull extension is disabled because it broke file downloads and it is not properly documented.

    // Check for Pitbull
    /*sock->connectToHost(pSocket.peerAddress(), pSocket.peerPort() + 1);
    sock->waitForConnected();

    sock->write("VERS\0\0\0\0\0\0\0\0\0\0\0\0", 16);
    sock->waitForBytesWritten();

    sock->waitForReadyRead();
    QByteArray response = sock->readAll().left(4);

    pServerProtocolExtensions.pitbull = response == "YES.";

    sock->disconnect();*/

    delete sock;
    return pServerProtocolExtensions;
}


void ConnectionController::onSocketError(QAbstractSocket::SocketError e) {
    qDebug() << "Error: " << e;
    QString string;
    switch(e) {
    case 0:
        string = "Connection was refused.";
        break;
    case 1:
        string = "The server closed the connection.";
        break;
    case 2:
        string = "Host not found.";
        break;
    case 3:
        string = "Socket access denied.";
        break;
    case 4:
        string = "Not enough memory.";
        break;
    case 5:
        string = "Connection timed out.";
        break;
    case 6:
        string = "System datagram limit reached.";
        break;
    case 7:
        string = "Network connection lost.";
        break;
    case 8:
        string = "Address already in use.";
        break;
    case 9:
        string = "Specified address not available.";
        break;
    case 10:
        string = "Network operation not supported.";
        break;
    case 12:
        string = "Proxy requires authentication.";
        break;
    case 13:
        string = "SSL/TSL handshake failed.";
        break;
    case 11:
        string = "Socket operation still in progress.";
        break;
    case 14:
        string = "Connection to proxy denied.";
        break;
    case 15:
        string = "Proxy disconnected unexpectedly.";
        break;
    case 16:
        string = "Connection to proxy timed out.";
        break;
    case 17:
        string = "Proxy address not found.";
        break;
    case 18:
        string = "Could not identify proxy protocol.";
        break;
    default:
        string = "Unknown socket error.";
    }


    QSettings settings("mir", "Contra");
    if(e == 1 && settings.value("autoReconnect", false).toBool() && pReconnectionAttempts < 3)
    {
        emit socketError(string+"<br>Reconnecting...");
        closeConnection();
        QTimer::singleShot(2000, this, SLOT(reconnect()));
    } else
    {
        emit socketError(string);
        closeConnection();
    }
}

void ConnectionController::onConnectionTimedOut()
{
    QSettings settings("mir", "Contra");
    if(settings.value("autoReconnect", false).toBool() && pReconnectionAttempts < 3)
    {
        emit socketError("Connection timed out.<br>Reconnecting...");
        closeConnection();
        QTimer::singleShot(2000, this, SLOT(reconnect()));
    } else
    {
        emit socketError("Connection timed out.");
        closeConnection();
    }
}

void ConnectionController::reconnect()
{
    connectToServer(pServerAddress,pPlainLogin,pPlainPassword, false);
}

void ConnectionController::onNameChanged() {
    sendUserInfo();
}

void ConnectionController::onSocketData() {
    if(receivedTransaction == NULL) {
        QByteArray dataArray = pSocket.read(22);

        receivedTransaction = new CTransaction(dataArray.constData());

        if(receivedTransaction->errorCode() == -1) {
            qDebug() << "Transaction error.";
            return;
        }
    }

    if(pSocket.bytesAvailable() < receivedTransaction->dataLength()-2 && receivedTransaction->dataLength() > 0) {
        return;
    }

    QByteArray bytes = pSocket.read(receivedTransaction->dataLength()-2);
    receivedTransaction->addData(bytes.constData());

    TransactionParameter * parameterBuffer;

    if(receivedTransaction->errorCode() != 0) {
        parameterBuffer = receivedTransaction->getParameterById(100);
        if(parameterBuffer) {
            emit serverError(parameterBuffer->toString());
        }
    }

    if(receivedTransaction->isReply()) {
        for(quint32 i=0; i<pPendingTransactions.size(); i++) {
            if(receivedTransaction->taskID() == pPendingTransactions[i]->taskID()) {
                switch(pPendingTransactions[i]->transactionID()) {
                case 101:
                    parameterBuffer = receivedTransaction->getParameterById(101);
                    if(parameterBuffer) {
                        emit gotLinearNews(parameterBuffer->toString());
                    }
                    break;
                case 107:
                    {
                    QSettings settings("mir", "Contra");
                    pNickname = settings.value(QString("nick"), "unnamed").toString();

                    sendUserInfo();

                    if(receivedTransaction->errorCode() == 0) {
                        parameterBuffer = receivedTransaction->getParameterById(162);
                        if(parameterBuffer) {
                            if(parameterBuffer->type() == TYPE_STRING) {
                                pServerName = parameterBuffer->toString();
                                if(!pServerName.isEmpty() && pServerName != " ")
                                {
                                    emit gotServerName();
                                    emit gotChatMessage(QString("                <b>Connected to %1</b>").arg(pServerName));
                                }
                                else
                                {
                                    pServerName = "";
                                    emit gotChatMessage(QString("                <b>Connected to %1</b>").arg(pServerAddress));
                                }
                            }
                            else
                            {
                                pServerName = "";
                                emit gotChatMessage("                <b>Connection established</b>");
                            }
                        }
                        else
                        {
                            pServerName = "";
                            emit gotChatMessage("                <b>Connection established</b>");
                        }

                        requestUserList();
                    }
                    break;
                    }
                case 126:
                    if(pServerProtocolExtensions.pitbull) {
                    /*
                    125 is sending an image
                    126 is recieving one
                    ￼
                    // Set Parameters
                    Parameters.AddString(HFieldType.FileName, fileName);
                    Parameters.AddBinary(HFieldType.Data, HUtils.ImageToByteArray(image, image.RawFormat));
                    if (chatId != 0)
                    Parameters.AddUInt32(HFieldType.ChatId, chatId);
                    if (opts != 0)
                    Parameters.AddUInt32(HFieldType.Options, opts);
                    ￼
                    fields for sending
                    ￼
                    p.AddBinary(HFieldType.UserStamp, client.ToUserStamp().ToBinary());
                    p.AddString(HFieldType.FileName, fileName);
                    p.AddBinary(HFieldType.Data, data);
                    if (opts != 0)
                    p.AddUInt32(HFieldType.Options, opts);
                    ￼
                    fields for recieving
                    ￼
                    UserStamp is a special type pitbull uses
                    ￼
                    it bascialy contains a username, id, info etc etc
                    */

                    /*
                    theres a seperate transaction to send an image to private chat
                    ￼
                    SendImg = 123, // Client [Pitbull]
                    ServerImg = 124, // Server [Pitbull]
                    */
                    }
                    break;
                case 200:
                    {
                    std::vector<s_hotlineFile *> fileList;
                    for(quint32 i=0; i<receivedTransaction->numberOfParameters(); i++) {
                        parameterBuffer = receivedTransaction->getParameter(i);
                        if(parameterBuffer) {
                            if(parameterBuffer->id() == 200) {
                                s_hotlineFile * file = (s_hotlineFile *) malloc(sizeof(s_hotlineFile));

                                file->type = (char *) malloc(5);

                                memcpy(file->type, parameterBuffer->data(), 4);
                                file->type[4] = '\0';

                                memcpy(&file->size, parameterBuffer->data()+8, 4);
                                file->size = qFromBigEndian(file->size);

                                memcpy(&file->nameSize, parameterBuffer->data()+18, 2);
                                file->nameSize = qFromBigEndian(file->nameSize);

                                file->name = (char *) malloc(sizeof(char)*file->nameSize+1);
                                memcpy(file->name, parameterBuffer->data()+20, file->nameSize);
                                file->name[file->nameSize] = '\0';

                                fileList.push_back(file);
                            }
                        }
                    }

                    emit gotFileList(fileList);
                    }
                    break;
                case 202:
                    {
                    quint32 transferSize = 0;
                    quint32 referenceNumber = 0;

                    parameterBuffer = receivedTransaction->getParameterById(207);
                    if(parameterBuffer) {
                        transferSize = parameterBuffer->toInt();
                    } else {
                        parameterBuffer = receivedTransaction->getParameterById(108);
                        if(parameterBuffer) {
                            transferSize = parameterBuffer->toInt();
                        }
                    }
                    parameterBuffer = receivedTransaction->getParameterById(107);
                    if(parameterBuffer) {
                        referenceNumber = parameterBuffer->toInt();
                    }

                    parameterBuffer = receivedTransaction->getParameterById(116);
                    quint32 queuePosition = 0;
                    if(parameterBuffer) {
                        quint16 squeue = 0;
                        quint32 iqueue = 0;
                        if(parameterBuffer->type() == TYPE_SHORT) {
                            squeue = parameterBuffer->toShort();
                        } else {
                            iqueue = parameterBuffer->toInt();
                        }
                        if(squeue > 0) {
                            queuePosition = squeue;
                        }
                        if(iqueue > 0) {
                            queuePosition = iqueue;
                        }
                    }


                    emit gotFile(referenceNumber, transferSize, queuePosition);
                    break;
                    }
                case 203:
                    {
                        parameterBuffer = receivedTransaction->getParameterById(107);
                        if(parameterBuffer) {
                            emit gotUpload(parameterBuffer->toInt());
                        }
                    }
                    break;
                case 204:
                    {
                        emit receivedFileDeleteResponse(receivedTransaction->errorCode());
                    }
                    break;
                case 300:
                    while(pUsers.size() > 0) {
                        s_user * u = pUsers.back();
                        pUsers.pop_back();
                        free(u);
                    }

                    for(quint32 i=0; i<receivedTransaction->numberOfParameters(); i++) {
                        parameterBuffer = receivedTransaction->getParameter(i);
                        if(parameterBuffer) {
                            if(parameterBuffer->id() == 300) {
                                s_user * newUser = (s_user *) malloc(sizeof(s_user));

                                memcpy(&newUser->id, parameterBuffer->data(), 2);
                                newUser->id = qFromBigEndian(newUser->id);

                                memcpy(&newUser->icon, parameterBuffer->data() + 2, 2);
                                newUser->icon = qFromBigEndian(newUser->icon);

                                newUser->doesCET = false;

                                newUser->iconPath = new QString(QString(":/icons/") + QString::number(newUser->icon) + QString(".png"));

                                memcpy(&newUser->flags, parameterBuffer->data() + 4, 2);
                                newUser->flags = qFromBigEndian(newUser->flags);

                                memcpy(&newUser->nameLength, parameterBuffer->data() + 6, 2);
                                newUser->nameLength = qFromBigEndian(newUser->nameLength);

                                newUser->name = (char *) malloc(sizeof(char)*newUser->nameLength+1);
                                memcpy(newUser->name, parameterBuffer->data() + 8, newUser->nameLength);
                                newUser->name[newUser->nameLength] = '\0';

                                newUser->infoWindow = NULL;

                                pUsers.push_back(newUser);

                                emit userListChanged();
                            }
                        }
                    }

                 break;
                case 303:
                {
                    TransactionParameter * userNameParameter = receivedTransaction->getParameterById(102);
                    TransactionParameter * userInfoParameter = receivedTransaction->getParameterById(101);

                    if(userNameParameter && userInfoParameter) {
                        emit gotUserInfo(userNameParameter->toString(), userInfoParameter->toString(), m_UserInfoTaskMap[receivedTransaction->taskID()]);
                    }
                }
                break;
                case 352:
                {
                    parameterBuffer = receivedTransaction->getParameterById(110);
                    TransactionParameter * loginParameter = receivedTransaction->getParameterById(105);
                    if(parameterBuffer && loginParameter)
                    {
                        for(int i=0; i<loginParameter->length(); ++i)
                        {
                            loginParameter->setData(i, 255 - loginParameter->data()[i]);
                        }
                        QString login = TextHelper::DecodeText(loginParameter->data(), loginParameter->length());

                        quint8 permissions[8];
                        memset(permissions, 0, 8);
                        memcpy(&permissions[0], parameterBuffer->data(), 1);
                        memcpy(&permissions[1], parameterBuffer->data()+1, 1);
                        memcpy(&permissions[2], parameterBuffer->data()+2, 1);
                        memcpy(&permissions[3], parameterBuffer->data()+3, 1);
                        memcpy(&permissions[4], parameterBuffer->data()+4, 1);
                        memcpy(&permissions[5], parameterBuffer->data()+5, 1);
                        memcpy(&permissions[6], parameterBuffer->data()+6, 1);
                        memcpy(&permissions[7], parameterBuffer->data()+7, 1);

                        TransactionParameter * passwordParameter = receivedTransaction->getParameterById(106);
                        if (passwordParameter) {
                            QString password = passwordParameter->toString();
                            emit gotPermissions(login, password, permissions[0], permissions[1], permissions[2], permissions[3], permissions[4], permissions[5], permissions[6], permissions[7]);
                        }
                    }
                }
                    break;
                 case 355:
                    {
                        parameterBuffer = receivedTransaction->getParameterById(101);
                        if(parameterBuffer) {
                            emit gotBroadcast(parameterBuffer->toString());
                        }
                    }
                    break;
                 case 370:
                 case 371:
                    {
                        for(quint32 i=0; i<receivedTransaction->numberOfParameters(); i++) {
                            parameterBuffer = receivedTransaction->getParameter(i);
                            if(parameterBuffer) {
                                if(parameterBuffer->id() == 320) {
                                    qDebug() << "Warning: Ignoring old-style threaded news!";
                                }
                                if(parameterBuffer->id() == 321) { // Got news items
                                    QString _name = "";
                                    QString _poster = "";

                                    quint32 count;
                                    memcpy(&count, parameterBuffer->data()+4, 4);
                                    count = qFromBigEndian(count);

                                    char size;
                                    memcpy(&size, parameterBuffer->data()+8, 1);

                                    char size2;
                                    memcpy(&size2, parameterBuffer->data()+9+size, 1);

                                    qint32 offset = 10+size+size2;
                                    for(quint32 j=0; j<count; j++) {
                                        quint32 articleID;
                                        memcpy(&articleID, parameterBuffer->data()+offset, 4);
                                        articleID = qFromBigEndian(articleID);

                                        quint32 parentArticleID;
                                        memcpy(&parentArticleID, parameterBuffer->data()+offset+12, 4);
                                        parentArticleID = qFromBigEndian(parentArticleID);

                                        quint16 fcount;
                                        memcpy(&fcount, parameterBuffer->data()+offset+20, 2);
                                        fcount = qFromBigEndian(fcount);

                                        char tsize;
                                        memcpy(&tsize, parameterBuffer->data()+offset+22, 1);
                                        char * atitle = (char *) malloc(tsize+1);
                                        memcpy(atitle, parameterBuffer->data()+offset+23, tsize);
                                        atitle[(quint16)tsize] = '\0';
                                        _name = QString(atitle);
                                        free(atitle);

                                        char psize;
                                        memcpy(&psize, parameterBuffer->data()+offset+23+tsize, 1);
                                        char * aposter = (char *) malloc(psize+1);
                                        memcpy(aposter, parameterBuffer->data()+offset+24+tsize, psize);
                                        aposter[(quint16)psize] = '\0';
                                        _poster = QString(aposter);
                                        free(aposter);

                                        qint32 offset2 = offset+24+tsize+psize;

                                        for(quint32 f=0; f<fcount; f++) {
                                            char fsize;
                                            memcpy(&fsize, parameterBuffer->data()+offset2, 1);
                                            offset2 += 1;
                                            // char * mime = (char *) malloc(fsize+1);
                                            // memcpy(mime, parameterBuffer->data()+offset2, fsize);
                                            // mime[(quint16)fsize] = '\0';
                                            // free(mime);

                                            offset2+= fsize;
                                            quint16 asize;
                                            memcpy(&asize, parameterBuffer->data()+offset2, 2);
                                            asize = qFromBigEndian(asize);

                                            offset2 += 2;
                                        }

                                        offset = offset2;

                                        emit gotNewsItem(_name, articleID, parentArticleID);
                                    }
                                }
                                if(parameterBuffer->id() == 323) { // Got news categories/bundles
                                    unsigned char _type = 0;
                                    QString _name = "";

                                    quint16 _typeshort;
                                    memcpy(&_typeshort, parameterBuffer->data(), 2);
                                    _typeshort = qFromBigEndian(_typeshort);
                                    _type = (unsigned char) _typeshort;

                                    char * buffer;

                                    if(_typeshort == 2) { // Bundle
                                        unsigned char ns;
                                        memcpy(&ns, parameterBuffer->data()+4, 1);
                                        buffer = (char *) malloc(ns+1);
                                        memcpy(buffer, parameterBuffer->data()+5, ns);
                                        buffer[ns] = '\0';
                                        _name = QString(buffer);
                                        free(buffer);
                                    }

                                    if(_typeshort == 3) { // Category
                                        unsigned char ns;
                                        memcpy(&ns, parameterBuffer->data()+28, 1);
                                        buffer = (char *) malloc(ns+1);
                                        memcpy(buffer, parameterBuffer->data()+29, ns);
                                        buffer[ns] = '\0';
                                        _name = QString(buffer);
                                        free(buffer);
                                    }

                                    emit gotNewsCategory(_type, _name);
                                }
                            }
                        }
                    }
                break;
                case 400:
                    {
                        QString text, poster, timestamp;
                        char * t;

                        parameterBuffer = receivedTransaction->getParameterById(333);
                        if(parameterBuffer) {
                            t = (char *) malloc(parameterBuffer->length()+1);
                            memcpy(t, parameterBuffer->data(), parameterBuffer->length());
                            t[parameterBuffer->length()] = '\0';
                            text = QString(t);
                            free(t);
                        }
                        parameterBuffer = receivedTransaction->getParameterById(329);
                        if(parameterBuffer) {
                            t = (char *) malloc(parameterBuffer->length()+1);
                            memcpy(t, parameterBuffer->data(), parameterBuffer->length());
                            t[parameterBuffer->length()] = '\0';
                            poster = QString(t);
                            free(t);
                        }
                        parameterBuffer = receivedTransaction->getParameterById(330);
                        if(parameterBuffer) {
                            quint16 year;
                            memcpy(&year, parameterBuffer->data(), 2);
                            year = qFromBigEndian(year);
                            quint16 millis;
                            memcpy(&millis, parameterBuffer->data()+2, 2);
                            millis = qFromBigEndian(millis);
                            quint32 seconds;
                            memcpy(&seconds, parameterBuffer->data()+4, 4);
                            seconds = qFromBigEndian(seconds);

                            quint32 d = 1, M = 1, h = 0, m = 0, s = 0;
                            s = seconds + millis/1000;

                            quint32 secondsInDay = 86400;

                            while(s > secondsInDay) {
                                d++;
                                s -= secondsInDay;
                            }

                            bool isLeap;

                            if(year%4 == 0) {
                                if(year%100 == 0 && year%400 != 0) {
                                    isLeap = false;
                                } else {
                                    isLeap = true;
                                }
                            } else {
                                isLeap = false;
                            }

                            quint32 daysInMonth;

                            for(qint32 i=0; i<12 ; i++) {
                                if(i==0 || i==2 || i==4 || i==6 || i==7 || i==9 || i==11) {
                                    daysInMonth = 31;
                                } else {
                                    if(i==1) {
                                        if(isLeap) {
                                            daysInMonth = 29;
                                        } else {
                                            daysInMonth = 28;
                                        }
                                    } else {
                                        daysInMonth = 30;
                                    }
                                }

                                if(d > daysInMonth) {
                                    M++;
                                    d -= daysInMonth;
                                } else {
                                    break;
                                }
                            }

                            timestamp = QString::number(d)+" ";

                            switch(M) {
                            case 1:
                                timestamp += "January ";
                                break;
                            case 2:
                                timestamp += "February ";
                                break;
                            case 3:
                                timestamp += "March ";
                                break;
                            case 4:
                                timestamp += "April ";
                                break;
                            case 5:
                                timestamp += "May ";
                                break;
                            case 6:
                                timestamp += "June ";
                                break;
                            case 7:
                                timestamp += "July ";
                                break;
                            case 8:
                                timestamp += "August ";
                                break;
                            case 9:
                                timestamp += "September ";
                                break;
                            case 10:
                                timestamp += "October ";
                                break;
                            case 11:
                                timestamp += "November ";
                                break;
                            case 12:
                                timestamp += "December ";
                                break;
                            }

                            timestamp += QString::number(year);
                            timestamp += " at ";

                            while(s > 3600) {
                                h++;
                                s -= 3600;
                            }
                            while(s > 60) {
                                m++;
                                s -= 60;
                            }

                            if(h < 10) {
                                timestamp += "0"+QString::number(h)+":";
                            } else {
                                timestamp += QString::number(h)+":";
                            }
                            if(m < 10) {
                                timestamp += "0"+QString::number(m)+":";
                            } else {
                                timestamp += QString::number(m)+":";
                            }
                            if(s < 10) {
                                timestamp += "0"+QString::number(s);
                            } else {
                                timestamp += QString::number(s);
                            }

                            if(d > 31) {
                                timestamp = "Unknown";
                            }
                        }

                        emit gotNewsArticleText(text, poster, timestamp);
                    }
                    break;
                }
                pPendingTransactions[i]->done = true;
            }
        }

        for(quint32 i=0; i<pPendingTransactions.size(); i++) {
            if(pPendingTransactions[i]->done) {
                CTransaction * t = pPendingTransactions[i];
                pPendingTransactions[i] = pPendingTransactions.back();
                pPendingTransactions.pop_back();
                delete t;
            }
        }
    } else { // NOT A REPLY
        switch(receivedTransaction->transactionID()) {
        case 104:
            parameterBuffer = receivedTransaction->getParameterById(103);
            if(parameterBuffer && receivedTransaction->getParameterById(101)) {
                quint16 uid = parameterBuffer->toShort();
                emit gotPM(receivedTransaction->getParameterById(101)->toString(), uid);
            } else {
                parameterBuffer = receivedTransaction->getParameterById(101);
                if(parameterBuffer) {
                    emit serverError(parameterBuffer->toString());
                }
            }
            break;
        case 106:
            parameterBuffer = receivedTransaction->getParameterById(101);
            if(parameterBuffer) {
                emit gotChatMessage(parameterBuffer->toString());
            }
            break;

        case 109:
            parameterBuffer = receivedTransaction->getParameterById(101);
            if(parameterBuffer) {
                pServerAgreement = parameterBuffer->toString();
            }
            break;

        case 113:
            parameterBuffer = receivedTransaction->getParameterById(103);
            if(parameterBuffer) {
                quint16 uid = parameterBuffer->toShort();

                if(uid) {
                    sendPMToUser(uid, QString("I'm sorry, this client does not support private chats yet. Please use private messages\0"), true);
                }
            }
            break;

        case 122:
            parameterBuffer = receivedTransaction->getParameterById(152);
            if (parameterBuffer) {
                quint32 bannerType = parameterBuffer->toInt();

                if (bannerType == 1) {
                    parameterBuffer = receivedTransaction->getParameterById(153);
                    pServerBannerURL = parameterBuffer->toString();
                    emit gotServerBannerURL(pServerBannerURL);
                }
            }
            break;

        case 211:
            {
                quint32 referenceNumber = -1;
                parameterBuffer = receivedTransaction->getParameterById(107);
                if(parameterBuffer) {
                    referenceNumber = parameterBuffer->toInt();
                }

                parameterBuffer = receivedTransaction->getParameterById(116);
                quint32 queuePosition = 0;
                if(parameterBuffer) {
                    quint16 squeue = 0;
                    quint32 iqueue = 0;
                    if(parameterBuffer->type() == TYPE_SHORT) {
                        squeue = parameterBuffer->toShort();
                    } else {
                        iqueue = parameterBuffer->toInt();
                    }
                    if(squeue > 0) {
                        queuePosition = squeue;
                    }
                    if(iqueue > 0) {
                        queuePosition = iqueue;
                    }
                }

                emit serverUpdatedQueue(referenceNumber, queuePosition);
                break;
            }

        case 301:
            if(receivedTransaction->getParameterById(103)) {
                parameterBuffer = receivedTransaction->getParameterById(103);

                if(!parameterBuffer) {
                    break;
                }

                quint16 uid = parameterBuffer->toShort();

                s_user * user = getUserByUid(uid);
                if(user) { // Update user
                    parameterBuffer = receivedTransaction->getParameterById(104);
                    if(parameterBuffer) {
                        quint16 newIcon = parameterBuffer->toShort();

                        if(user->icon == 3520 && newIcon != 3520) {
                            user->doesCET = true;
                            //sendCETIdentification(user);
                        } else {
                            user->doesCET = false;
                        }

                        user->icon = newIcon;
                        user->iconPath = new QString(QString(":/icons/") + QString::number(user->icon) + QString(".png"));
                    }

                    parameterBuffer = receivedTransaction->getParameterById(112);
                    if(parameterBuffer) {
                        user->flags = parameterBuffer->toShort();
                    }

                    parameterBuffer = receivedTransaction->getParameterById(102);

                    if(parameterBuffer) {
                        QString oldName = TextHelper::DecodeText(user->name, user->nameLength);

                        user->name = (char *) malloc(sizeof(char)*parameterBuffer->length());
                        memcpy(user->name, parameterBuffer->data(), parameterBuffer->length());
                        user->nameLength = parameterBuffer->length();

                        QString newName = TextHelper::DecodeText(user->name, parameterBuffer->length());
                        QString message = QString("                <b>%1 is now known as %2</b>").arg(oldName, newName);

                        if(oldName != newName)
                        {
                            emit userChangedName(oldName, newName);
                            emit gotChatMessage(message);
                        }
                    }

                } else { // New user
                    s_user * newUser = (s_user *) malloc(sizeof(s_user));
                    newUser->id = uid;

                    parameterBuffer = receivedTransaction->getParameterById(104);
                    if(parameterBuffer) {
                        newUser->icon = parameterBuffer->toShort();
                        newUser->iconPath = new QString(QString("icons/") + QString::number(newUser->icon) + QString(".png"));
                    }

                    parameterBuffer = receivedTransaction->getParameterById(112);
                    if(parameterBuffer) {
                        newUser->flags = parameterBuffer->toShort();
                    }

                    parameterBuffer = receivedTransaction->getParameterById(102);
                    if(parameterBuffer) {
                        newUser->name = (char *) malloc(sizeof(char)*parameterBuffer->length());
                        memcpy(newUser->name, parameterBuffer->data(), parameterBuffer->length());
                        newUser->nameLength = parameterBuffer->length();
                    }

                    pUsers.push_back(newUser);
                    QString message = QString("                <b>%1 has joined</b>").arg(TextHelper::DecodeText(newUser->name, newUser->nameLength));
                    emit gotChatMessage(message);
                }
                emit userListChanged();
            }
            break;
        case 302:
            parameterBuffer = receivedTransaction->getParameterById(103);
            if(parameterBuffer) {
                quint16 uid = parameterBuffer->toShort();
                s_user * user = getUserByUid(uid);

                if(!user) {
                    // Server reported user left, but it was never here to begin with.
                    break;
                }

                emit userLeft(user);

                QString message = QString("                <b>%1 has left</b>").arg(TextHelper::DecodeText(user->name, user->nameLength));
                emit gotChatMessage(message);
                for(quint32 i=0; i<pUsers.size(); i++) {
                    if(pUsers[i]->id == uid) {
                        pUsers[i] = pUsers.back();
                        pUsers.pop_back();
                        break;
                    }
                }

                delete user;

                emit userListChanged();
            }
            break;
        }
    }

    parameterBuffer = NULL;
    delete receivedTransaction;
    receivedTransaction = NULL;

    if(pSocket.bytesAvailable()) {
        onSocketData();
    }
}

std::string ConnectionController::getUserHash(s_user * user) {
    QString string;
    string.append(user->name);
    return string.toStdString();
}
