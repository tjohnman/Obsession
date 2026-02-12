#ifndef PROTOCOLEXTENSIONS_H
#define PROTOCOLEXTENSIONS_H

/**
 * @brief Tracks server protocol extension support
 * 
 * Manages flags for optional protocol extensions like
 * Pitbull (image sharing) and encryption support.
 */
class ProtocolExtensions
{
public:
    ProtocolExtensions() 
        : m_pitbull(false)
        , m_encryption(false)
    {}

    bool pitbull() const { return m_pitbull; }
    void setPitbull(bool enabled) { m_pitbull = enabled; }

    bool encryption() const { return m_encryption; }
    void setEncryption(bool enabled) { m_encryption = enabled; }

    void clear() {
        m_pitbull = false;
        m_encryption = false;
    }

private:
    bool m_pitbull;
    bool m_encryption;
};

#endif // PROTOCOLEXTENSIONS_H
