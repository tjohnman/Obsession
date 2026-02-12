#include "connectioncontroller.h"
#include "HotlineProtocol.h"
#include "DateTimeParser.h"
#include <QSettings>
#include <QStringList>
#include <QDir>
#include <QBrush>
#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QStringConverter>
#include "dialogprivatemessaging.h"

#include "transactionparameter.h"
#include "version.h"
#include "TextHelper.h"
#include "SettingsManager.h"

using namespace HotlineProtocol;

ConnectionController::ConnectionController()
{
    pSocket.open(QIODevice::ReadWrite);
    connect(&pSocket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(&pSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(&pTimeoutTimer, SIGNAL(timeout()), this, SLOT(onConnectionTimedOut()));
    
    // Connect UserManager signals
    connect(&m_userManager, &UserManager::userListChanged, this, &ConnectionController::userListChanged);

    m_serverInfo.agreement() = QString();

    // TODO: Preferences
    auto& settings = SettingsManager::instance();
    m_clientState.setNickname(settings.value(QString::fromUtf8("nick"), QString::fromUtf8("unnamed")).toString());
    m_clientState.setIconID((quint16) settings.value("icon", 25096).toString().toShort());

    m_clientState.setAFK(false);

    m_clientState.setClientVersion(190);

    pTaskIDCounter = 0;

    pReconnectionAttempts = 0;
    receivedTransaction = nullptr;

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
    m_serverInfo.address() = address;
    m_clientState.setPlainLogin(login);
    m_clientState.setPlainPassword(password);
    qint32 semicolonIndex = address.lastIndexOf(QString::fromUtf8(":"));
    QString addr;
    quint16 port;

    if(semicolonIndex != -1) {
        addr = address.left(semicolonIndex);
        port = address.right(address.length()-semicolonIndex-1).toInt();
    } else {
        addr = address;
        port = 5500;
    }

    QByteArray encodedLogin = TextHelper::EncodeText(login);
    for(qint32 i=0; i<encodedLogin.length(); i++) {
        encodedLogin[i] = 255 - encodedLogin[i];
    }
    m_clientState.setEncodedLogin(encodedLogin);

    QByteArray encodedPassword = TextHelper::EncodeText(password);
    for(qint32 i=0; i<encodedPassword.length(); i++) {
        encodedPassword[i] = 255 - encodedPassword[i];
    }
    m_clientState.setEncodedPassword(encodedPassword);

    pTimeoutTimer.start();
    pSocket.connectToHost(addr, port);

    serverIdent = addr;

    return 0;
}

void ConnectionController::broadcast(QString text)
{
    CTransaction * trans = createTransaction(355);
    QByteArray arr = TextHelper::EncodeText(text);
    trans->addParameter(toInt(Parameter::ChatMessage), arr.size(), arr.data());
    sendTransaction(trans);
}

void ConnectionController::requestAccount(QString login)
{
    QByteArray loginArr = TextHelper::EncodeText(login);
    CTransaction * t = createTransaction(352);
    t->addParameter(toInt(Parameter::PrivateChat), loginArr.size(), loginArr.data());
    sendTransaction(t, true);
}

void ConnectionController::sendTransaction(CTransaction * t, bool expectReply) {
    if(isConnected()) {
        pSocket.write(t->bytes(), t->length());
        if(expectReply) {
            m_transactionQueue.add(t);
        } else {
            delete t;
        }
    }
}

QString ConnectionController::serverName() {
    return QString(m_serverInfo.name().data());
}

void ConnectionController::sendChatText(QString text) {
    CTransaction * chatTransaction = new CTransaction(Transaction::SendChat, pTaskIDCounter++);
    chatTransaction->addParameter(toInt(Parameter::ChatMessage), TextHelper::EncodeText(text).size(), TextHelper::EncodeText(text).data());
    sendTransaction(chatTransaction);
}

void ConnectionController::sendEmote(QString text) {
    CTransaction * chatTransaction = new CTransaction(Transaction::SendChat, pTaskIDCounter++);
    chatTransaction->addParameter(toInt(Parameter::ChatMessage), TextHelper::EncodeText(text).size(), TextHelper::EncodeText(text).data());
    chatTransaction->addParameter(toInt(Parameter::ChatOptions), 1);
    sendTransaction(chatTransaction);
}

void ConnectionController::toggleAFK() {
    if(m_clientState.isAFK()) {
        auto& settings = SettingsManager::instance();
        m_clientState.setNickname(settings.value(QString::fromUtf8("nick"), QString::fromUtf8("unnamed")).toString());
        sendUserInfo();
        sendEmote(QString::fromUtf8("is back"));
        m_clientState.setAFK(false);
    } else {
        sendEmote(QString::fromUtf8("is AFK"));
        auto& settings = SettingsManager::instance();
        m_clientState.setNickname(settings.value(QString::fromUtf8("nick"), QString::fromUtf8("unnamed")).toString() + QString::fromUtf8(" (AFK)"));
        m_clientState.setAFK(true);
        sendUserInfo();
    }
}

bool ConnectionController::isAFK() {
    return m_clientState.isAFK();
}

void ConnectionController::sendUserInfo() {
    auto& settings = SettingsManager::instance();
    m_clientState.setNickname(settings.value(QString::fromUtf8("nick"), QString::fromUtf8("unnamed")).toString());
    m_clientState.setIconID(settings.value("icon", 25096).toString().toShort());
    CTransaction * uinfoTransaction = new CTransaction(Transaction::SetUserInfo, pTaskIDCounter++);
    uinfoTransaction->addParameter(toInt(Parameter::UserLogin), TextHelper::EncodeText(m_clientState.nickname()).size(), TextHelper::EncodeText(m_clientState.nickname()).data());
    uinfoTransaction->addParameter(toInt(Parameter::UserIconId), m_clientState.iconID());
    sendTransaction(uinfoTransaction);
}

HotlineUser * ConnectionController::getUserByUid(qint16 uid) {
    return m_userManager.getUserByUid(uid);
}

HotlineUser * ConnectionController::getUserByName(QString name) {
    return m_userManager.getUserByName(name);
}

std::vector<HotlineUser *> * ConnectionController::getUserList() {
    return m_userManager.getUserList();
}

QString ConnectionController::serverAgreement() {
    return m_serverInfo.agreement();
}

void ConnectionController::requestUserInfo(quint16 id)
{
    qint32 task = pTaskIDCounter++;
    m_UserInfoTaskMap[task] = id;
    CTransaction * PMTransaction = new CTransaction(Transaction::UserChange, task);
    PMTransaction->addParameter(toInt(Parameter::UserId), id);
    sendTransaction(PMTransaction, true);
}

void ConnectionController::sendPMToUser(quint16 uid, QString message, bool automatic) {
    CTransaction * PMTransaction = new CTransaction(Transaction::SendPrivateMessage, pTaskIDCounter++);
    PMTransaction->addParameter(toInt(Parameter::UserId), uid);
    if(automatic) {
        PMTransaction->addParameter(toInt(Parameter::ChatSubject), 4);
    } else {
        PMTransaction->addParameter(toInt(Parameter::ChatSubject), 1);
    }
    PMTransaction->addParameter(toInt(Parameter::ChatMessage), TextHelper::EncodeText(message).size(), TextHelper::EncodeText(message).data());
    sendTransaction(PMTransaction);
}

void ConnectionController::closeConnection(bool silent) {
    if(!silent && m_serverInfo.address() != QString::fromUtf8("") && !m_serverInfo.address().isEmpty())
    {
        emit gotChatMessage(QString::fromUtf8("                <b>Disconnected from %1</b>").arg(m_serverInfo.address()));
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

    ProtocolExtensions protocol_extensions = this->checkForProtocolExtensions();
    m_protocolExtensions = protocol_extensions;

    char clientMagicBytes[12] = {0x54, 0x52, 0x54, 0x50, 0x48, 0x4f, 0x54, 0x4c, 0x00, 0x01, 0x00, 0x02};
    char serverMagicBytes[8] = {0x54, 0x52, 0x54, 0x50, 0x00, 0x00, 0x00, 0x00};

    qint32 len = pSocket.read(serverMagicBytes, 8);

    if(len < 8 || memcmp(clientMagicBytes, serverMagicBytes, 4) != 0) {
        pSocket.close();
        // emit error
        return;
    }

    char errorCode[4];
    qint32 errorCodeLen = pSocket.read(errorCode, 4);

    if(errorCodeLen != 4 || errorCode[3] != 0) {
        pSocket.close();
        // emit error
        return;
    }

    quint16 serverVersion = ((quint16)serverMagicBytes[6] << 8) | (quint16)serverMagicBytes[7];

    CTransaction * loginTransaction = new CTransaction(Transaction::Login, pTaskIDCounter++);
    loginTransaction->addParameter(toInt(Parameter::PrivateChat), m_clientState.encodedLogin().length(), m_clientState.encodedLogin().data());
    if(m_clientState.encodedPassword().length() > 0) {
        loginTransaction->addParameter(toInt(Parameter::UserPassword), m_clientState.encodedPassword().length(), m_clientState.encodedPassword().data());
    }
    loginTransaction->addParameter(toInt(Parameter::UserLogin), TextHelper::EncodeText(m_clientState.nickname()).size(), TextHelper::EncodeText(m_clientState.nickname()).data());
    loginTransaction->addParameter(toInt(Parameter::UserIconId), m_clientState.iconID());

    quint16 ver = qToBigEndian(m_clientState.clientVersion());
    loginTransaction->addParameter(151, ver);

    if(protocol_extensions.pitbull()) {
        loginTransaction->addParameter(163, 4, (char *)"OBSE");
        loginTransaction->addParameter(164, VERSION_MAJOR*10 + VERSION_MINOR);
    }

    m_serverInfo.agreement() = QString::fromUtf8("");

    sendTransaction(loginTransaction, true);
}

ProtocolExtensions ConnectionController::checkForProtocolExtensions() {
    QTcpSocket * sock = new QTcpSocket();

    // Pitbull extension is disabled because it broke file downloads and it is not properly documented.

    // Check for Pitbull
    /*sock->connectToHost(pSocket.peerAddress(), pSocket.peerPort() + 1);
    sock->waitForConnected();

    sock->write("VERS\0\0\0\0\0\0\0\0\0\0\0\0", 16);
    sock->waitForBytesWritten();

    sock->waitForReadyRead();
    QByteArray response = sock->readAll().left(4);

    m_protocolExtensions.setPitbull(response == QString::fromUtf8("YES."));

    sock->disconnect();*/

    delete sock;
    return m_protocolExtensions;
}


void ConnectionController::onSocketError(QAbstractSocket::SocketError e) {
    qDebug() << "Error: " << e;
    QString string;
    switch(e) {
    case 0:
        string = QString::fromUtf8("Connection was refused.");
        break;
    case 1:
        string = QString::fromUtf8("The server closed the connection.");
        break;
    case 2:
        string = QString::fromUtf8("Host not found.");
        break;
    case 3:
        string = QString::fromUtf8("Socket access denied.");
        break;
    case 4:
        string = QString::fromUtf8("Not enough memory.");
        break;
    case 5:
        string = QString::fromUtf8("Connection timed out.");
        break;
    case 6:
        string = QString::fromUtf8("System datagram limit reached.");
        break;
    case 7:
        string = QString::fromUtf8("Network connection lost.");
        break;
    case 8:
        string = QString::fromUtf8("Address already in use.");
        break;
    case 9:
        string = QString::fromUtf8("Specified address not available.");
        break;
    case 10:
        string = QString::fromUtf8("Network operation not supported.");
        break;
    case 12:
        string = QString::fromUtf8("Proxy requires authentication.");
        break;
    case 13:
        string = QString::fromUtf8("SSL/TSL handshake failed.");
        break;
    case 11:
        string = QString::fromUtf8("Socket operation still in progress.");
        break;
    case 14:
        string = QString::fromUtf8("Connection to proxy denied.");
        break;
    case 15:
        string = QString::fromUtf8("Proxy disconnected unexpectedly.");
        break;
    case 16:
        string = QString::fromUtf8("Connection to proxy timed out.");
        break;
    case 17:
        string = QString::fromUtf8("Proxy address not found.");
        break;
    case 18:
        string = QString::fromUtf8("Could not identify proxy protocol.");
        break;
    default:
        string = QString::fromUtf8("Unknown socket error.");
    }


    auto& settings = SettingsManager::instance();
    if(e == 1 && settings.value("autoReconnect", false).toBool() && pReconnectionAttempts < 3)
    {
        emit socketError(string+ QString::fromUtf8("<br>Reconnecting..."));
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
    auto& settings = SettingsManager::instance();
    if(settings.value("autoReconnect", false).toBool() && pReconnectionAttempts < 3)
    {
        emit socketError(QString::fromUtf8("Connection timed out.<br>Reconnecting..."));
        closeConnection();
        QTimer::singleShot(2000, this, SLOT(reconnect()));
    } else
    {
        emit socketError(QString::fromUtf8("Connection timed out."));
        closeConnection();
    }
}

void ConnectionController::reconnect()
{
    connectToServer(m_serverInfo.address(),m_clientState.plainLogin(),m_clientState.plainPassword(), false);
}

void ConnectionController::onNameChanged() {
    sendUserInfo();
}

void ConnectionController::onSocketData() {
    if(receivedTransaction == nullptr) {
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
        parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ErrorCode));
        if(parameterBuffer) {
            emit serverError(parameterBuffer->toString());
        }
    }

    if(receivedTransaction->isReply()) {
        CTransaction* pendingTrans = m_transactionQueue.findByTaskId(receivedTransaction->taskID());
        if(pendingTrans) {
            switch(pendingTrans->transactionID()) {
                case Transaction::ServerMessage:
                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ChatMessage));
                    if(parameterBuffer) {
                        emit gotLinearNews(parameterBuffer->toString());
                    }
                    break;
                case Transaction::Login:
                    {
                    auto& settings = SettingsManager::instance();
                    m_clientState.setNickname(settings.value(QString::fromUtf8("nick"), QString::fromUtf8("unnamed")).toString());

                    sendUserInfo();

                    if(receivedTransaction->errorCode() == 0) {
                        parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ServerBanner));
                        if(parameterBuffer) {
                            if(parameterBuffer->type() == TYPE_STRING) {
                                m_serverInfo.name() = parameterBuffer->toString();
                                if(!m_serverInfo.name().isEmpty() && m_serverInfo.name() != QString::fromUtf8(" "))
                                {
                                    emit gotServerName();
                                    emit gotChatMessage(QString::fromUtf8("                <b>Connected to %1</b>").arg(m_serverInfo.name()));
                                }
                                else
                                {
                                    m_serverInfo.name() = QString::fromUtf8("");
                                    emit gotChatMessage(QString::fromUtf8("                <b>Connected to %1</b>").arg(m_serverInfo.address()));
                                }
                            }
                            else
                            {
                                m_serverInfo.name() = QString::fromUtf8("");
                                emit gotChatMessage(QString::fromUtf8("                <b>Connection established</b>"));
                            }
                        }
                        else
                        {
                            m_serverInfo.name() = QString::fromUtf8("");
                            emit gotChatMessage(QString::fromUtf8("                <b>Connection established</b>"));
                        }

                        requestUserList();
                    }
                    break;
                    }
                case Transaction::AgreementAccepted:
                    if(m_protocolExtensions.pitbull()) {
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
                case Transaction::GetFileListReply:
                    {
                    std::vector<HotlineFile *> fileList;
                    for(quint32 i=0; i<receivedTransaction->numberOfParameters(); i++) {
                        parameterBuffer = receivedTransaction->getParameter(i);
                        if(parameterBuffer) {
                            if(parameterBuffer->id() == 200) {
                                HotlineFile * file = new HotlineFile();

                                // Extract 4-byte type code
                                file->type = QString::fromLatin1(parameterBuffer->data(), 4);

                                // Extract size
                                memcpy(&file->size, parameterBuffer->data()+8, 4);
                                file->size = qFromBigEndian(file->size);

                                // Extract name
                                quint16 nameSize;
                                memcpy(&nameSize, parameterBuffer->data()+18, 2);
                                nameSize = qFromBigEndian(nameSize);
                                
                                file->name = QString::fromUtf8(parameterBuffer->data()+20, nameSize);

                                fileList.push_back(file);
                            }
                        }
                    }

                    emit gotFileList(fileList);
                    }
                    break;
                case Transaction::GetFileInfo:
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
                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ReferenceNumber));
                    if(parameterBuffer) {
                        referenceNumber = parameterBuffer->toInt();
                    }

                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::QueuePosition));
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
                case Transaction::DownloadFile:
                    {
                        parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ReferenceNumber));
                        if(parameterBuffer) {
                            emit gotUpload(parameterBuffer->toInt());
                        }
                    }
                    break;
                case Transaction::DeleteFile:
                    {
                        emit receivedFileDeleteResponse(receivedTransaction->errorCode());
                    }
                    break;
                case Transaction::GetUserInfo:
                    m_userManager.clearAllUsers();

                    for(quint32 i=0; i<receivedTransaction->numberOfParameters(); i++) {
                        parameterBuffer = receivedTransaction->getParameter(i);
                        if(parameterBuffer) {
                            if(parameterBuffer->id() == 300) {
                                HotlineUser * newUser = new HotlineUser();

                                memcpy(&newUser->id, parameterBuffer->data(), 2);
                                newUser->id = qFromBigEndian(newUser->id);

                                memcpy(&newUser->icon, parameterBuffer->data() + 2, 2);
                                newUser->icon = qFromBigEndian(newUser->icon);

                                newUser->doesCET = false;

                                newUser->iconPath = std::make_unique<QString>(QString::fromUtf8(":/icons/") + QString::number(newUser->icon) + QString::fromUtf8(".png"));

                                memcpy(&newUser->flags, parameterBuffer->data() + 4, 2);
                                newUser->flags = qFromBigEndian(newUser->flags);

                                quint16 nameLength;
                                memcpy(&nameLength, parameterBuffer->data() + 6, 2);
                                nameLength = qFromBigEndian(nameLength);

                                newUser->name = QString::fromUtf8(parameterBuffer->data() + 8, nameLength);

                                newUser->infoWindow = nullptr;

                                m_userManager.addUser(newUser);
                            }
                        }
                    }

                 break;
                case Transaction::UserChange:
                {
                    TransactionParameter * userNameParameter = receivedTransaction->getParameterById(toInt(Parameter::UserLogin));
                    TransactionParameter * userInfoParameter = receivedTransaction->getParameterById(toInt(Parameter::ChatMessage));

                    if(userNameParameter && userInfoParameter) {
                        emit gotUserInfo(userNameParameter->toString(), userInfoParameter->toString(), m_UserInfoTaskMap[receivedTransaction->taskID()]);
                    }
                }
                break;
                case Transaction::GetNewsCategoryListReply:
                {
                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserData));
                    TransactionParameter * loginParameter = receivedTransaction->getParameterById(toInt(Parameter::PrivateChat));
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

                        TransactionParameter * passwordParameter = receivedTransaction->getParameterById(toInt(Parameter::UserPassword));
                        if (passwordParameter) {
                            QString password = passwordParameter->toString();
                            emit gotPermissions(login, password, permissions[0], permissions[1], permissions[2], permissions[3], permissions[4], permissions[5], permissions[6], permissions[7]);
                        }
                    }
                }
                    break;
                 case 355:
                    {
                        parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ChatMessage));
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
                                    QString _name = QString::fromUtf8("");
                                    QString _poster = QString::fromUtf8("");

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
                                        _name = QString::fromUtf8(atitle);
                                        free(atitle);

                                        char psize;
                                        memcpy(&psize, parameterBuffer->data()+offset+23+tsize, 1);
                                        char * aposter = (char *) malloc(psize+1);
                                        memcpy(aposter, parameterBuffer->data()+offset+24+tsize, psize);
                                        aposter[(quint16)psize] = '\0';
                                        _poster = QString::fromUtf8(aposter);
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
                                    QString _name = QString::fromUtf8("");

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
                                        _name = QString::fromUtf8(buffer);
                                        free(buffer);
                                    }

                                    if(_typeshort == 3) { // Category
                                        unsigned char ns;
                                        memcpy(&ns, parameterBuffer->data()+28, 1);
                                        buffer = (char *) malloc(ns+1);
                                        memcpy(buffer, parameterBuffer->data()+29, ns);
                                        buffer[ns] = '\0';
                                        _name = QString::fromUtf8(buffer);
                                        free(buffer);
                                    }

                                    emit gotNewsCategory(_type, _name);
                                }
                            }
                        }
                    }
                break;
                case Transaction::UploadFileReply:
                    {
                        QString text, poster, timestamp;
                        char * t;

                        parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::NewsArticleData));
                        if(parameterBuffer) {
                            t = (char *) malloc(parameterBuffer->length()+1);
                            memcpy(t, parameterBuffer->data(), parameterBuffer->length());
                            t[parameterBuffer->length()] = '\0';
                            text = QString::fromUtf8(t);
                            free(t);
                        }
                        parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::NewsArticleDate));
                        if(parameterBuffer) {
                            t = (char *) malloc(parameterBuffer->length()+1);
                            memcpy(t, parameterBuffer->data(), parameterBuffer->length());
                            t[parameterBuffer->length()] = '\0';
                            poster = QString::fromUtf8(t);
                            free(t);
                        }
                        parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::NewsArticleParent));
                        if(parameterBuffer) {
                            timestamp = DateTimeParser::parseAndFormat(parameterBuffer->data());
                        }

                        emit gotNewsArticleText(text, poster, timestamp);
                    }
                    break;
                }
                m_transactionQueue.markComplete(receivedTransaction->taskID());
        }

        m_transactionQueue.removeCompleted();
    } else { // NOT A REPLY
        switch(receivedTransaction->transactionID()) {
        case 104:
            parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserId));
            if(parameterBuffer && receivedTransaction->getParameterById(toInt(Parameter::ChatMessage))) {
                quint16 uid = parameterBuffer->toShort();
                emit gotPM(receivedTransaction->getParameterById(toInt(Parameter::ChatMessage))->toString(), uid);
            } else {
                parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ChatMessage));
                if(parameterBuffer) {
                    emit serverError(parameterBuffer->toString());
                }
            }
            break;
        case 106:
            parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ChatMessage));
            if(parameterBuffer) {
                emit gotChatMessage(parameterBuffer->toString());
            }
            break;

        case 109:
            parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ChatMessage));
            if(parameterBuffer) {
                m_serverInfo.agreement() = parameterBuffer->toString();
            }
            break;

        case 113:
            parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserId));
            if(parameterBuffer) {
                quint16 uid = parameterBuffer->toShort();

                if(uid) {
                    sendPMToUser(uid, QString::fromUtf8("I'm sorry, this client does not support private chats yet. Please use private messages\0"), true);
                }
            }
            break;

        case 122:
            parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ServerBannerType));
            if (parameterBuffer) {
                quint32 bannerType = parameterBuffer->toInt();

                if (bannerType == 1) {
                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ServerBannerUrl));
                    m_serverInfo.bannerURL() = parameterBuffer->toString();
                    emit gotServerBannerURL(m_serverInfo.bannerURL());
                }
            }
            break;

        case 211:
            {
                quint32 referenceNumber = -1;
                parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::ReferenceNumber));
                if(parameterBuffer) {
                    referenceNumber = parameterBuffer->toInt();
                }

                parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::QueuePosition));
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
            if(receivedTransaction->getParameterById(toInt(Parameter::UserId))) {
                parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserId));

                if(!parameterBuffer) {
                    break;
                }

                quint16 uid = parameterBuffer->toShort();

                HotlineUser * user = getUserByUid(uid);
                if(user) { // Update user
                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserIconId));
                    if(parameterBuffer) {
                        quint16 newIcon = parameterBuffer->toShort();

                        if(user->icon == 3520 && newIcon != 3520) {
                            user->doesCET = true;
                            //sendCETIdentification(user);
                        } else {
                            user->doesCET = false;
                        }

                        user->icon = newIcon;
                        user->iconPath = std::make_unique<QString>(QString::fromUtf8(":/icons/") + QString::number(user->icon) + QString::fromUtf8(".png"));
                    }

                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserFlags));
                    if(parameterBuffer) {
                        user->flags = parameterBuffer->toShort();
                    }

                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserLogin));

                    if(parameterBuffer) {
                        QByteArray oldNameUtf8 = user->name.toUtf8();
                        QString oldName = TextHelper::DecodeText(oldNameUtf8.constData(), oldNameUtf8.length());

                        user->name = QString::fromUtf8(parameterBuffer->data(), parameterBuffer->length());

                        QByteArray newNameUtf8 = user->name.toUtf8();
                        QString newName = TextHelper::DecodeText(newNameUtf8.constData(), newNameUtf8.length());
                        QString message = QString::fromUtf8("                <b>%1 is now known as %2</b>").arg(oldName, newName);

                        if(oldName != newName)
                        {
                            emit userChangedName(oldName, newName);
                            emit gotChatMessage(message);
                        }
                    }

                } else { // New user
                    HotlineUser * newUser = new HotlineUser();
                    newUser->id = uid;

                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserIconId));
                    if(parameterBuffer) {
                        newUser->icon = parameterBuffer->toShort();
                        newUser->iconPath = std::make_unique<QString>(QString::fromUtf8("icons/") + QString::number(newUser->icon) + QString::fromUtf8(".png"));
                    }

                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserFlags));
                    if(parameterBuffer) {
                        newUser->flags = parameterBuffer->toShort();
                    }

                    parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserLogin));
                    if(parameterBuffer) {
                        newUser->name = QString::fromUtf8(parameterBuffer->data(), parameterBuffer->length());
                    }

                    m_userManager.addUser(newUser);
                    QByteArray nameUtf8 = newUser->name.toUtf8();
                    QString message = QString::fromUtf8("                <b>%1 has joined</b>").arg(TextHelper::DecodeText(nameUtf8.constData(), nameUtf8.length()));
                    emit gotChatMessage(message);
                }
            }
            break;
        case 302:
            parameterBuffer = receivedTransaction->getParameterById(toInt(Parameter::UserId));
            if(parameterBuffer) {
                quint16 uid = parameterBuffer->toShort();
                HotlineUser * user = getUserByUid(uid);

                if(!user) {
                    // Server reported user left, but it was never here to begin with.
                    break;
                }

                emit userLeft(user);

                QByteArray nameUtf8 = user->name.toUtf8();
                QString message = QString::fromUtf8("                <b>%1 has left</b>").arg(TextHelper::DecodeText(nameUtf8.constData(), nameUtf8.length()));
                emit gotChatMessage(message);
                m_userManager.removeUser(uid);
            }
            break;
        }
    }

    parameterBuffer = nullptr;
    delete receivedTransaction;
    receivedTransaction = nullptr;

    if(pSocket.bytesAvailable()) {
        onSocketData();
    }
}

std::string ConnectionController::getUserHash(HotlineUser * user) {
    return m_userManager.getUserHash(user);
}
