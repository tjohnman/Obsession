#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include <QString>
#include <QVariant>
#include <memory>

/**
 * @brief Centralized settings management singleton
 * 
 * Replaces the scattered QSettings instantiations throughout the codebase.
 * Provides a single point of access to application settings with type-safe
 * helper methods.
 * 
 * Usage:
 *   auto& settings = SettingsManager::instance();
 *   settings.setValue("key", value);
 *   auto value = settings.value("key", defaultValue);
 */
class SettingsManager
{
public:
    // Deleted copy/move constructors and assignment operators
    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;
    SettingsManager(SettingsManager&&) = delete;
    SettingsManager& operator=(SettingsManager&&) = delete;
    
    /**
     * @brief Get the singleton instance
     * @return Reference to the SettingsManager instance
     */
    static SettingsManager& instance()
    {
        static SettingsManager instance;
        return instance;
    }
    
    /**
     * @brief Get access to the underlying QSettings object
     * @return Reference to QSettings
     */
    QSettings& settings()
    {
        return m_settings;
    }
    
    /**
     * @brief Set a value in settings
     * @param key The setting key
     * @param value The value to set
     */
    void setValue(const QString& key, const QVariant& value)
    {
        m_settings.setValue(key, value);
    }
    
    /**
     * @brief Set a value in settings (overload for string literals)
     * @param key The setting key as C string
     * @param value The value to set
     */
    void setValue(const char* key, const QVariant& value)
    {
        m_settings.setValue(QString::fromUtf8(key), value);
    }
    
    /**
     * @brief Get a value from settings
     * @param key The setting key
     * @param defaultValue Default value if key doesn't exist
     * @return The setting value or default
     */
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const
    {
        return m_settings.value(key, defaultValue);
    }
    
    /**
     * @brief Get a value from settings (overload for string literals)
     * @param key The setting key as C string
     * @param defaultValue Default value if key doesn't exist
     * @return The setting value or default
     */
    QVariant value(const char* key, const QVariant& defaultValue = QVariant()) const
    {
        return m_settings.value(QString::fromUtf8(key), defaultValue);
    }
    
    /**
     * @brief Check if a key exists in settings
     * @param key The setting key to check
     * @return true if key exists
     */
    bool contains(const QString& key) const
    {
        return m_settings.contains(key);
    }
    
    /**
     * @brief Check if a key exists in settings (overload for string literals)
     * @param key The setting key to check as C string
     * @return true if key exists
     */
    bool contains(const char* key) const
    {
        return m_settings.contains(QString::fromUtf8(key));
    }
    
    /**
     * @brief Remove a key from settings
     * @param key The setting key to remove
     */
    void remove(const QString& key)
    {
        m_settings.remove(key);
    }
    
    /**
     * @brief Remove a key from settings (overload for string literals)
     * @param key The setting key to remove as C string
     */
    void remove(const char* key)
    {
        m_settings.remove(QString::fromUtf8(key));
    }
    
    /**
     * @brief Sync settings to persistent storage
     */
    void sync()
    {
        m_settings.sync();
    }
    
    /**
     * @brief Begin a settings group
     * @param prefix The group prefix
     */
    void beginGroup(const QString& prefix)
    {
        m_settings.beginGroup(prefix);
    }
    
    /**
     * @brief End the current settings group
     */
    void endGroup()
    {
        m_settings.endGroup();
    }
    
    /**
     * @brief Get all keys in current group
     * @return List of keys
     */
    QStringList allKeys() const
    {
        return m_settings.allKeys();
    }
    
    /**
     * @brief Get child groups in current group
     * @return List of child groups
     */
    QStringList childGroups() const
    {
        return m_settings.childGroups();
    }

private:
    /**
     * @brief Private constructor - singleton pattern
     */
    SettingsManager() 
        : m_settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"))
    {
    }
    
    /**
     * @brief Private destructor
     */
    ~SettingsManager() = default;
    
    QSettings m_settings;
};

#endif // SETTINGSMANAGER_H
