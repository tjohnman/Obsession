#ifndef USERINFOTRACKER_H
#define USERINFOTRACKER_H

#include <map>
#include <QtGlobal>

/**
 * @brief Tracks pending user info requests
 * 
 * Maps transaction task IDs to user IDs for user info requests.
 * Allows retrieving the user ID when the transaction reply is received.
 */
class UserInfoTracker
{
public:
    /**
     * @brief Register a user info request
     * @param taskId Transaction task ID
     * @param userId User ID being requested
     */
    void registerRequest(qint32 taskId, quint16 userId) {
        m_taskToUserMap[taskId] = userId;
    }

    /**
     * @brief Get user ID for a completed transaction
     * @param taskId Transaction task ID
     * @return User ID, or 0 if not found
     */
    quint16 getUserIdForTask(qint32 taskId) const {
        auto it = m_taskToUserMap.find(taskId);
        if (it != m_taskToUserMap.end()) {
            return it->second;
        }
        return 0;
    }

    /**
     * @brief Remove a completed request
     * @param taskId Transaction task ID
     */
    void removeRequest(qint32 taskId) {
        m_taskToUserMap.erase(taskId);
    }

    /**
     * @brief Clear all pending requests
     */
    void clear() {
        m_taskToUserMap.clear();
    }

private:
    std::map<qint32, quint16> m_taskToUserMap;
};

#endif // USERINFOTRACKER_H
