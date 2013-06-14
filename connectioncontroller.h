#ifndef CONNECTIONCONTROLLER_H
#define CONNECTIONCONTROLLER_H

#include <qTcpSocket>
#include <QByteArray>
#include <vector>
#include "CTransaction.h"
#include <qtEndian>
#include <QDate>
#include <QTimer>

class ConnectionController : public QObject
{
    Q_OBJECT
public:
    ConnectionController();

    bool pAFK;
    QString serverName();

    int connectToServer(QString address, QString login, QString password);
    void sendTransaction(CTransaction *, bool expectReply = false);
    void sendChatText(QString);
    void sendEmote(QString);
    void sendPMToUser(int, QString, bool automatic = false);

    bool isConnected();
    void toggleAFK();
    bool isAFK();
    QString serverAgreement();

    void closeConnection();

    s_user * getUserByUid(short uid);
    s_user * getUserByName(QString name);
    std::vector<s_user *> * getUserList();

    CTransaction * createTransaction(short id);

    QString serverIdent;

    QTcpSocket pSocket;

    bool pPermissionDeleteFile;
    bool pPermissionUploadFile;
    bool pPermissionDownloadFile;
    bool pPermissionRenameFile;
    bool pPermissionMoveFile;
    bool pPermissionCreateFolder;
    bool pPermissionDeleteFolder;
    bool pPermissionRenameFolder;
    bool pPermissionMoveFolder;
    bool pPermissionReadChat;
    bool pPermissionSendChat;
    bool pPermissionOpenChat;
    bool pPermissionCloseChat;
    bool pPermissionShowInList;
    bool pPermissionCreateUser;
    bool pPermissionDeleteUser;
    bool pPermissionOpenUser;
    bool pPermissionModifyUser;
    bool pPermissionChangeOwnPassword;
    bool pPermissionSendPrivateMessage;
    bool pPermissionNewsReadArticle;
    bool pPermissionNewsPostArticle;
    bool pPermissionDisconnectUser;
    bool pPermissionCannotBeDisconnected;
    bool pPermissionGetClientInfo;
    bool pPermissionUploadAnywhere;
    bool pPermissionAnyName;
    bool pPermissionNoAgreement;
    bool pPermissionSetFileComment;
    bool pPermissionViewDropBoxes;
    bool pPermissionMakeAlias;
    bool pPermissionBroadcast;
    bool pPermissionNewsDeleteArticle;
    bool pPermissionNewsCreateCategory;
    bool pPermissionNewsDeleteCategory;
    bool pPermissionNewsCreateFolder;
    bool pPermissionNewsDeleteFolder;

private:
    std::vector<CTransaction *> pPendingTransactions;
    std::vector<s_user *> pUsers;

    QByteArray pLogin;
    QByteArray pPassword;
    unsigned short pIconID;
    QString pNickname;
    short pClientVersion;
    unsigned short ownUserID;

    QString pServerAddress;
    quint16 pServerPort;
    QByteArray pServerName;
    short pServerVersion;
    QString pServerAgreement;

    int pTaskIDCounter;

    void sendUserInfo();

    CTransaction * receivedTransaction;

    void sendCETIdentification(s_user *);

public slots:
    void onSocketConnected();
    void onSocketError(QAbstractSocket::SocketError);
    void onSocketData();
    void onNameChanged();

signals:
    void gotServerName();
    void gotChatMessage(char * t, short l);
    void userListChanged();
    void gotPM(short, QString);
    void gotFileList(std::vector<s_hotlineFile *>);
    void serverError(QString);
    void socketError(QString);
    void connected();
    void gotLinearNews(QString);
    void connecting();
    void gotFile(unsigned int, unsigned int, unsigned int);
    void serverUpdatedQueue(unsigned int, unsigned int);
    void gotNewsCategory(unsigned char, QString);
    void gotNewsItem(QString, unsigned int id, unsigned int pid);
    void gotNewsArticleText(QString, QString, QString);
    void gotUpload(unsigned int);
};

#endif // CONNECTIONCONTROLLER_H
