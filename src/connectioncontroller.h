#ifndef CONNECTIONCONTROLLER_H
#define CONNECTIONCONTROLLER_H

#include <QTcpSocket>
#include <QByteArray>
#include <vector>
#include <memory>
#include "ctransaction.h"
#include "UserManager.h"
#include "ServerInfo.h"
#include "TransactionQueue.h"
#include "ClientState.h"
#include "ProtocolExtensions.h"
#include "ConnectionState.h"
#include "TaskIdGenerator.h"
#include "NewsProtocolParser.h"
#include "UserInfoTracker.h"
#include <QtEndian>
#include <QDate>
#include <QTimer>
#include <map>

class ConnectionController : public QObject
{
    Q_OBJECT
public:
    ConnectionController();

    QString serverName() const;
    QString serverIdentifier() const { return m_serverInfo.identifier(); }

    qint32 connectToServer(QString address, QString login, QString password, bool resetAutoReconnect = true);
    void sendTransaction(CTransaction *, bool expectReply = false);
    void sendChatText(QString);
    void sendEmote(QString);
    void sendPMToUser(quint16, QString, bool automatic = false);
    void requestUserInfo(quint16 id);
    void requestAccount(QString login);
    void broadcast(QString text);

    bool isConnected() const;
    void toggleAFK();
    bool isAFK() const;
    QString serverAgreement() const;

    void closeConnection(bool silent = false);

    HotlineUser * getUserByUid(qint16 uid) const;
    HotlineUser * getUserByName(QString name) const;
    std::vector<HotlineUser *> * getUserList() const;

    UserManager* userManager() { return &m_userManager; }

    CTransaction * createTransaction(qint16 id);

    std::string getUserHash(HotlineUser *);

    QTcpSocket pSocket;

private:
    TransactionQueue m_transactionQueue;
    UserManager m_userManager;
    ServerInfo m_serverInfo;
    ClientState m_clientState;
    ProtocolExtensions m_protocolExtensions;
    ConnectionState m_connectionState;
    TaskIdGenerator m_taskIdGenerator;
    UserInfoTracker m_userInfoTracker;

    void sendUserInfo();

    std::unique_ptr<CTransaction> m_receivedTransaction;

    ProtocolExtensions checkForProtocolExtensions();

    // Transaction reply handlers
    void handleServerMessageReply(TransactionParameter*& parameterBuffer);
    void handleDeleteFileReply();
    void handleDownloadFileReply(TransactionParameter*& parameterBuffer);
    void handleGetFileInfoReply(TransactionParameter*& parameterBuffer);
    void handleLoginReply(TransactionParameter*& parameterBuffer);
    void handleAgreementAcceptedReply();
    void handleGetFileListReply(TransactionParameter*& parameterBuffer);
    void handleGetUserInfoReply(TransactionParameter*& parameterBuffer);
    void handleUserChangeReply(TransactionParameter*& parameterBuffer);
    void handleGetNewsCategoryListReply(TransactionParameter*& parameterBuffer);
    void handleUploadFileReply(TransactionParameter*& parameterBuffer);
    void handleBroadcastReply(TransactionParameter*& parameterBuffer);
    void handleNewsItemsReply(TransactionParameter*& parameterBuffer);

    // Non-reply transaction handlers
    void handlePrivateMessage(TransactionParameter*& parameterBuffer);
    void handleChatMessage(TransactionParameter*& parameterBuffer);
    void handleServerAgreement(TransactionParameter*& parameterBuffer);
    void handlePrivateChatRequest(TransactionParameter*& parameterBuffer);
    void handleServerBanner(TransactionParameter*& parameterBuffer);
    void handleQueueUpdate(TransactionParameter*& parameterBuffer);
    void handleUserDisconnected(TransactionParameter*& parameterBuffer);
    void handleUserJoinOrUpdate(TransactionParameter*& parameterBuffer);

public slots:
    void reconnect();
    void onSocketConnected();
    void onSocketError(QAbstractSocket::SocketError);
    void onSocketData();
    void onNameChanged();
    void requestUserList();
    void onConnectionTimedOut();

signals:
    void gotServerName();
    void gotChatMessage(QString str);
    void gotBroadcast(QString str);
    void userListChanged();
    void gotPM(QString, qint16);
    void gotFileList(std::vector<HotlineFile *>);
    void serverError(QString);
    void socketError(QString);
    void connected();
    void gotLinearNews(QString);
    void connecting();
    void gotFile(quint32, quint32, quint32);
    void serverUpdatedQueue(quint32, quint32);
    void gotNewsCategory(unsigned char, QString);
    void gotNewsItem(QString, quint32 id, quint32 pid);
    void gotNewsArticleText(QString, QString, QString);
    void gotUserInfo(QString username, QString info, quint16);
    void gotUpload(quint32);
    void receivedFileDeleteResponse(qint32 code);
    void gotPermissions(QString login, QString password, quint8 p1, quint8 p2, quint8 p3, quint8 p4, quint8, quint8, quint8, quint8);
    void userChangedName(QString old_name, QString new_name);
    void userLeft(HotlineUser * user);
    void gotServerBannerURL(const QString url);
};

#endif // CONNECTIONCONTROLLER_H
