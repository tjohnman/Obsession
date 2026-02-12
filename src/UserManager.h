#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <vector>
#include <QString>
#include <QObject>
#include "defines.h"

/**
 * @brief Manages the list of connected users
 * 
 * Responsibilities:
 * - Maintain list of currently connected users
 * - Add/remove/update users
 * - Lookup users by ID or name
 * - Generate user hash for identification
 * 
 * This class follows the Single Responsibility Principle by focusing
 * solely on user list management, extracted from ConnectionController.
 */
class UserManager : public QObject
{
    Q_OBJECT

public:
    UserManager(QObject* parent = nullptr) : QObject(parent) {}
    
    ~UserManager() {
        clearAllUsers();
    }

    /**
     * @brief Get user by unique ID
     * @param uid User ID
     * @return Pointer to user or nullptr if not found
     */
    HotlineUser* getUserByUid(qint16 uid) const {
        for (HotlineUser* user : m_users) {
            if (user->id == uid) {
                return user;
            }
        }
        return nullptr;
    }

    /**
     * @brief Get user by name (partial match)
     * @param name Username to search for
     * @return Pointer to user or nullptr if not found
     */
    HotlineUser* getUserByName(const QString& name) const {
        for (HotlineUser* user : m_users) {
            if (name.contains(user->name)) {
                return user;
            }
        }
        return nullptr;
    }

    /**
     * @brief Get the complete user list
     * @return Pointer to user vector
     */
    std::vector<HotlineUser*>* getUserList() {
        return &m_users;
    }

    /**
     * @brief Get user count
     * @return Number of users in the list
     */
    size_t getUserCount() const {
        return m_users.size();
    }

    /**
     * @brief Add a user to the list
     * @param user User to add (takes ownership)
     */
    void addUser(HotlineUser* user) {
        if (user) {
            m_users.push_back(user);
            emit userListChanged();
        }
    }

    /**
     * @brief Remove a user by ID
     * @param uid User ID to remove
     * @return true if user was found and removed
     */
    bool removeUser(qint16 uid) {
        for (auto it = m_users.begin(); it != m_users.end(); ++it) {
            if ((*it)->id == uid) {
                HotlineUser* user = *it;
                m_users.erase(it);
                delete user;
                emit userListChanged();
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Update user information
     * @param uid User ID
     * @param newName New username (nullptr to keep existing)
     * @param newIcon New icon ID (-1 to keep existing)
     * @return true if user was found and updated
     */
    bool updateUser(qint16 uid, const char* newName = nullptr, qint16 newIcon = -1) {
        HotlineUser* user = getUserByUid(uid);
        if (!user) {
            return false;
        }

        if (newName) {
            user->name = QString::fromUtf8(newName);
        }

        if (newIcon >= 0) {
            user->icon = newIcon;
        }

        emit userListChanged();
        return true;
    }

    /**
     * @brief Clear all users from the list
     */
    void clearAllUsers() {
        for (HotlineUser* user : m_users) {
            delete user;
        }
        m_users.clear();
        emit userListChanged();
    }

    /**
     * @brief Generate hash for user identification
     * @param user User to hash
     * @return Hash string
     */
    std::string getUserHash(HotlineUser* user) const {
        if (!user) {
            return "";
        }

        std::string hash;
        hash += std::to_string(user->id);
        hash += "_";
        hash += user->name.toStdString();
        hash += "_";
        hash += std::to_string(user->icon);

        return hash;
    }

signals:
    /**
     * @brief Emitted when the user list changes
     */
    void userListChanged();

private:
    std::vector<HotlineUser*> m_users;
};

#endif // USERMANAGER_H
