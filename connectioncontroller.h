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

    qint32 connectToServer(QString address, QString login, QString password);
    void sendTransaction(CTransaction *, bool expectReply = false);
    void sendChatText(QString);
    void sendEmote(QString);
    void sendPMToUser(qint32, QString, bool automatic = false);

    bool isConnected();
    void toggleAFK();
    bool isAFK();
    QString serverAgreement();

    void closeConnection();

    s_user * getUserByUid(qint16 uid);
    s_user * getUserByName(QString name);
    std::vector<s_user *> * getUserList();

    CTransaction * createTransaction(qint16 id);

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
    quint16 pIconID;
    QString pNickname;
    qint16 pClientVersion;
    quint16 ownUserID;

    QString pServerAddress;
    quint16 pServerPort;
    QByteArray pServerName;
    qint16 pServerVersion;
    QString pServerAgreement;

    qint32 pTaskIDCounter;

    void sendUserInfo();

    CTransaction * receivedTransaction;

    //void sendCETIdentification(s_user *);

public slots:
    void onSocketConnected();
    void onSocketError(QAbstractSocket::SocketError);
    void onSocketData();
    void onNameChanged();

signals:
    void gotServerName();
    void gotChatMessage(QString str);
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
    void gotUpload(quint32);
    void receivedFileDeleteResponse(qint32 code);
};

#endif // CONNECTIONCONTROLLER_H
