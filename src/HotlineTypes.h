#ifndef HOTLINETYPES_H
#define HOTLINETYPES_H

#include <QString>
#include <memory>
#include <vector>
#include <cstring>

// Forward declarations
class DialogPrivateMessaging;
class DialogUserInfo;

/**
 * @brief Type constants for transaction parameters
 */
enum class ParameterType : qint32 {
    Int = 1,
    Short = 2,
    String = 3,
    Binary = 4
};

// Legacy defines for compatibility
#define TYPE_INT 1
#define TYPE_SHORT 2
#define TYPE_STRING 3
#define TYPE_BINARY 4

/**
 * @brief Modern C++ class for Hotline user information
 * 
 * Replaces the legacy s_user struct with RAII memory management
 */
class HotlineUser {
public:
    quint16 id{0};
    quint16 icon{0};
    std::unique_ptr<QString> iconPath;
    quint16 flags{0};
    QString name;
    bool doesCET{false};
    DialogUserInfo* infoWindow{nullptr};
    quint32 orderInList{0};
    
    HotlineUser() = default;
    
    /**
     * @brief Construct from C-style name buffer
     */
    HotlineUser(quint16 uid, quint16 iconId, const char* nameBuf, quint16 nameLen) 
        : id(uid), icon(iconId) {
        if (nameBuf && nameLen > 0) {
            name = QString::fromUtf8(nameBuf, nameLen);
        }
    }
    
    /**
     * @brief Get name as UTF-8 encoded byte array
     */
    QByteArray nameAsUtf8() const {
        return name.toUtf8();
    }
    
    /**
     * @brief Get name length in bytes
     */
    quint16 nameLength() const {
        return static_cast<quint16>(nameAsUtf8().length());
    }
};

/**
 * @brief Modern C++ class for Hotline file information
 * 
 * Replaces the legacy s_hotlineFile struct with RAII memory management
 */
class HotlineFile {
public:
    QString type;        // 4-character file type code
    quint32 size{0};
    QString name;
    
    HotlineFile() = default;
    
    /**
     * @brief Construct from C-style buffers
     */
    HotlineFile(const char* typeBuf, quint32 fileSize, const char* nameBuf, quint16 nameLen)
        : size(fileSize) {
        if (typeBuf) {
            type = QString::fromLatin1(typeBuf, 4);
        }
        if (nameBuf && nameLen > 0) {
            name = QString::fromUtf8(nameBuf, nameLen);
        }
    }
    
    /**
     * @brief Get type as 4-character buffer
     */
    QByteArray typeAs4Chars() const {
        QByteArray result = type.toLatin1();
        result.resize(4);
        return result;
    }
    
    /**
     * @brief Get name as UTF-8 encoded byte array
     */
    QByteArray nameAsUtf8() const {
        return name.toUtf8();
    }
    
    /**
     * @brief Get name length in bytes
     */
    quint16 nameSize() const {
        return static_cast<quint16>(nameAsUtf8().length());
    }
};

/**
 * @brief Modern C++ class for transaction parameters
 * 
 * Replaces the legacy s_parameter struct with RAII memory management
 */
class HotlineParameter {
public:
    quint16 id{0};
    quint16 length{0};
    qint32 type{TYPE_STRING};
    std::vector<char> data;
    quint16 shortValue{0};
    quint32 intValue{0};
    
    HotlineParameter() = default;
    
    /**
     * @brief Construct with data buffer
     */
    HotlineParameter(quint16 paramId, qint32 paramType, const char* buffer, quint16 len)
        : id(paramId), length(len), type(paramType) {
        if (buffer && len > 0) {
            data.resize(len);
            memcpy(data.data(), buffer, len);
        }
    }
    
    /**
     * @brief Get data pointer (for compatibility)
     */
    char* dataPtr() {
        return data.empty() ? nullptr : data.data();
    }
    
    const char* dataPtr() const {
        return data.empty() ? nullptr : data.data();
    }
};

/**
 * @brief Legacy s_item class (already properly implemented)
 */
class s_item {
public:
    quint32 reference;
    quint32 size;
    QString name;

    s_item(quint32 s, QString str) {
        reference = 0;
        size = s;
        name = str;
    }
};

// Legacy type aliases for backward compatibility
// These will allow gradual migration
typedef struct {
    quint16 id;
    quint16 length;
    qint32 type;
    char * data;
    quint16 shortValue;
    quint32 intValue;
} s_parameter;

typedef struct {
    quint16 id;
    quint16 icon;
    QString * iconPath;
    quint16 flags;
    quint16 nameLength;
    char * name;
    bool doesCET;
    DialogUserInfo * infoWindow;
    quint32 orderInList;
} s_user;

typedef struct {
    char * type;
    quint32 size;
    quint16 nameSize;
    char * name;
} s_hotlineFile;

#endif // HOTLINETYPES_H
