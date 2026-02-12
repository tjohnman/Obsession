#ifndef CONNECTIONSTATE_H
#define CONNECTIONSTATE_H

#include <QTimer>

/**
 * @brief Manages connection lifecycle state
 * 
 * Handles reconnection attempts, connection timeout tracking,
 * and connection state management.
 */
class ConnectionState
{
public:
    ConnectionState() 
        : m_reconnectionAttempts(0)
    {
        m_timeoutTimer.setSingleShot(true);
        m_timeoutTimer.setInterval(10000); // 10 seconds
    }

    // Reconnection management
    quint8 reconnectionAttempts() const { return m_reconnectionAttempts; }
    void resetReconnectionAttempts() { m_reconnectionAttempts = 0; }
    void incrementReconnectionAttempts() { ++m_reconnectionAttempts; }

    // Timeout management
    QTimer& timeoutTimer() { return m_timeoutTimer; }
    const QTimer& timeoutTimer() const { return m_timeoutTimer; }
    
    void startTimeout() { m_timeoutTimer.start(); }
    void stopTimeout() { m_timeoutTimer.stop(); }

private:
    quint8 m_reconnectionAttempts;
    QTimer m_timeoutTimer;
};

#endif // CONNECTIONSTATE_H
