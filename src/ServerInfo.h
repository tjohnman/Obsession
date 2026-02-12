#ifndef SERVERINFO_H
#define SERVERINFO_H

#include <QString>

/**
 * @brief Encapsulates server connection information
 * 
 * Stores server metadata including name, address, version,
 * agreement text, banner URL, and connection port.
 */
class ServerInfo
{
public:
    ServerInfo() : m_port(5500), m_version(0) {}

    // Getters
    QString address() const { return m_address; }
    quint16 port() const { return m_port; }
    QString name() const { return m_name; }
    qint16 version() const { return m_version; }
    QString agreement() const { return m_agreement; }
    QString bannerURL() const { return m_bannerURL; }
    QString identifier() const { return m_identifier; }

    // Setters
    void setAddress(const QString& addr) { m_address = addr; }
    void setPort(quint16 p) { m_port = p; }
    void setName(const QString& n) { m_name = n; }
    void setVersion(qint16 v) { m_version = v; }
    void setAgreement(const QString& a) { m_agreement = a; }
    void setBannerURL(const QString& url) { m_bannerURL = url; }
    void setIdentifier(const QString& id) { m_identifier = id; }

    // Utility
    void clear() {
        m_address.clear();
        m_port = 5500;
        m_name.clear();
        m_version = 0;
        m_agreement.clear();
        m_bannerURL.clear();
        m_identifier.clear();
    }

private:
    QString m_address;
    quint16 m_port = 5500;
    QString m_name;
    qint16 m_version = 0;
    QString m_agreement;
    QString m_bannerURL;
    QString m_identifier;  // Server display identifier
};

#endif // SERVERINFO_H
