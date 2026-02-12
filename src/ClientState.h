#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H

#include <QString>
#include <QByteArray>

/**
 * @brief Encapsulates client identity and authentication state
 * 
 * Manages the client's nickname, icon, login credentials,
 * version information, and AFK status.
 */
class ClientState
{
public:
    ClientState() 
        : m_iconID(25096)
        , m_clientVersion(190)
        , m_isAFK(false)
    {}

    // Nickname
    QString nickname() const { return m_nickname; }
    void setNickname(const QString& nick) { m_nickname = nick; }

    // Icon
    quint16 iconID() const { return m_iconID; }
    void setIconID(quint16 id) { m_iconID = id; }

    // Client version
    qint16 clientVersion() const { return m_clientVersion; }
    void setClientVersion(qint16 version) { m_clientVersion = version; }

    // AFK status
    bool isAFK() const { return m_isAFK; }
    void setAFK(bool afk) { m_isAFK = afk; }
    void toggleAFK() { m_isAFK = !m_isAFK; }

    // Credentials (encoded)
    const QByteArray& encodedLogin() const { return m_encodedLogin; }
    void setEncodedLogin(const QByteArray& login) { m_encodedLogin = login; }

    const QByteArray& encodedPassword() const { return m_encodedPassword; }
    void setEncodedPassword(const QByteArray& password) { m_encodedPassword = password; }

    // Plain credentials (for reconnection)
    QString plainLogin() const { return m_plainLogin; }
    void setPlainLogin(const QString& login) { m_plainLogin = login; }

    QString plainPassword() const { return m_plainPassword; }
    void setPlainPassword(const QString& password) { m_plainPassword = password; }

    // Permissions
    quint64 permissionBitmap() const { return m_permissionBitmap; }
    void setPermissionBitmap(quint64 bitmap) { m_permissionBitmap = bitmap; }

private:
    QString m_nickname;
    quint16 m_iconID;
    qint16 m_clientVersion;
    bool m_isAFK;
    
    QByteArray m_encodedLogin;
    QByteArray m_encodedPassword;
    
    QString m_plainLogin;
    QString m_plainPassword;
    
    quint64 m_permissionBitmap = 0;
};

#endif // CLIENTSTATE_H
