#ifndef CONNECTIONCONTROLLER_H
#define CONNECTIONCONTROLLER_H

#include <QTcpSocket>
#include <QByteArray>
#include <vector>
#include "ctransaction.h"
#include <QtEndian>
#include <QDate>
#include <QTimer>
#include <map>

class ConnectionController : public QObject
{
    Q_OBJECT
public:
    ConnectionController();

    bool pAFK;
    QString serverName();

    qint32 connectToServer(QString address, QString login, QString password, bool resetAutoReconnect = true);
    void sendTransaction(CTransaction *, bool expectReply = false);
    void sendChatText(QString);
    void sendEmote(QString);
    void sendPMToUser(quint16, QString, bool automatic = false);
    void requestUserInfo(quint16 id);
    void requestAccount(QString login);
    void broadcast(QString text);

    bool isConnected();
    void toggleAFK();
    bool isAFK();
    QString serverAgreement();

    void closeConnection(bool silent = false);

    s_user * getUserByUid(qint16 uid);
    s_user * getUserByName(QString name);
    std::vector<s_user *> * getUserList();

    CTransaction * createTransaction(qint16 id);

    std::string getUserHash(s_user *);

    QString serverIdent;

    QTcpSocket pSocket;

    quint64 pPermissionBitmap;

    QByteArray pLogin;

private:
    std::vector<CTransaction *> pPendingTransactions;
    std::vector<s_user *> pUsers;

    QByteArray pPassword;
    quint16 pIconID;
    QString pNickname;
    qint16 pClientVersion;

    QString pServerAddress;
    quint16 pServerPort;
    QString pServerName;
    qint16 pServerVersion;
    QString pServerAgreement;

    QString pPlainLogin, pPlainPassword;
    QTimer pTimeoutTimer;

    qint32 pTaskIDCounter;

    quint8 pReconnectionAttempts;

    void sendUserInfo();

    CTransaction * receivedTransaction;

    std::map<qint32,quint16> m_UserInfoTaskMap;

    typedef struct {
        bool pitbull = false;
        bool encryption = false;
    } t_protocolExtensions;

    t_protocolExtensions pServerProtocolExtensions;

    t_protocolExtensions checkForProtocolExtensions();

    //void sendCETIdentification(s_user *);

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
    void gotFileList(std::vector<s_hotlineFile *>);
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
    void userLeft(s_user * user);
};

#endif // CONNECTIONCONTROLLER_H
