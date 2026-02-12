#ifndef HOTLINEPROTOCOL_H
#define HOTLINEPROTOCOL_H

#include <QtTypes>

/**
 * @brief Hotline Protocol Constants
 * 
 * Defines all protocol transaction IDs, parameter IDs, and error codes
 * used in the Hotline client-server communication protocol.
 * 
 * This replaces scattered magic numbers throughout the codebase with
 * named constants for better readability and maintainability.
 */
namespace HotlineProtocol {

/**
 * @brief Transaction types (client → server and server → client)
 * 
 * NOTE: The Hotline protocol reuses transaction IDs in different contexts.
 * For example, ID 200 is used for both GetFileList (request) and the reply.
 * ID 202 is used for GetFileInfo (download request) and UploadFile.
 * ID 203 is used for SetFileInfo, DownloadFile, and upload notifications.
 * 
 * These constants use descriptive names to clarify their usage context.
 */
namespace Transaction {
    // Client to Server Transactions
    constexpr quint16 Login = 107;
    constexpr quint16 SendPrivateMessage = 108;
    constexpr quint16 GetMessages = 109;
    constexpr quint16 NewUser = 113;
    constexpr quint16 DeleteUser = 114;
    constexpr quint16 SendChat = 105;
    constexpr quint16 SendPrivateChat = 106;
    
    // User Operations
    constexpr quint16 GetUserInfo = 300;
    constexpr quint16 SetUserInfo = 304;        // Send updated user info (nickname, icon)
    constexpr quint16 UserChange = 303;         // Request user info or user change notification
    constexpr quint16 GetUserList = 348;
    
    // File Operations (note: some IDs are reused!)
    constexpr quint16 GetFileList = 200;        // Request file list
    constexpr quint16 GetFileListReply = 200;   // Server reply with file list (same ID)
    constexpr quint16 GetFileInfo = 202;        // Request file info for download
    constexpr quint16 UploadFile = 202;         // Upload file / News article data (same ID)
    constexpr quint16 SetFileInfo = 203;        // Set file information
    constexpr quint16 DownloadFile = 203;       // Download file / Upload notification (same ID)
    constexpr quint16 DeleteFile = 204;         // Delete file
    constexpr quint16 NewFolder = 201;          // Create new folder
    constexpr quint16 UploadFileReply = 400;    // Server reply for upload
    
    // News Operations
    constexpr quint16 GetNews = 370;
    constexpr quint16 PostNews = 371;
    constexpr quint16 DeleteNewsArticle = 380;
    constexpr quint16 GetNewsCategoryList = 352;
    constexpr quint16 GetNewsCategoryListReply = 352; // Same ID as request
    constexpr quint16 GetNewsArticleList = 355;
    constexpr quint16 GetNewsArticleListReply = 355;  // Same ID as request
    
    // Connection Management
    constexpr quint16 KeepAlive = 122;
    
    // Server to Client Notifications  
    constexpr quint16 ServerMessage = 101;
    constexpr quint16 UserAccess = 104;
    constexpr quint16 UserListUpdate = 354;
    constexpr quint16 ChatMessage = 106;
    constexpr quint16 PrivateMessage = 105;
    constexpr quint16 UserLeave = 304;
    
    // Agreement/Login related
    constexpr quint16 ShowAgreement = 108;
    constexpr quint16 AgreementAccepted = 126;
    constexpr quint16 SendBroadcast = 330;
    
    // Tracker related
    constexpr quint16 GetTrackerList = 211;
}

/**
 * @brief Parameter IDs used in transactions
 */
enum class Parameter : quint16 {
    // Error parameters
    ErrorCode = 100,
    
    // User parameters
    Data = 101,              // Generic data/text (chat messages, etc.)
    UserLogin = 102,
    UserId = 103,
    UserIconId = 104,
    PrivateChat = 105,
    UserPassword = 106,
    UserName = 102,          // Alias for UserLogin
    UserData = 110,          // User information data
    UserFlags = 112,
    
    // Chat parameters
    ChatMessage = 101,       // Alias for Data
    ChatOptions = 109,
    ChatSubject = 114,
    
    // File transfer parameters
    ReferenceNumber = 107,   // File transfer reference
    FileTransferSize = 207,  // Transfer size (alternate)
    FileSize = 108,          // File size
    QueuePosition = 116,     // Wait in queue position
    ResumeData = 211,        // Resume transfer data
    
    // File parameters
    FileName = 201,
    FilePath = 202,
    FileType = 204,
    FileCreator = 205,
    FileComment = 210,
    
    // Server parameters
    ServerBannerType = 152,  // Server banner type
    ServerBannerUrl = 153,   // Server banner URL
    ServerName = 160,
    ServerDescription = 161,
    ServerBanner = 162,
    
    // News parameters
    NewsPath = 325,
    NewsArticleId = 326,
    NewsArticleTitle = 327,
    NewsArticlePoster = 328,
    NewsArticleDate = 329,
    NewsArticleParent = 330,
    NewsArticleFlags = 331,
    NewsArticleData = 333,   // Note: 332 might exist but not used
    NewsCategoryName = 335,
    NewsCategoryId = 336,
    
    // Protocol parameters
    ProtocolVersion = 160,
    ClientVersion = 160,
};

/**
 * @brief Socket error codes (Qt error codes)
 */
enum class SocketError : qint32 {
    ConnectionRefused = 0,
    RemoteHostClosed = 1,
    HostNotFound = 2,
    SocketAccessError = 3,
    SocketResourceError = 4,
    SocketTimeout = 5,
    DatagramTooLarge = 6,
    NetworkError = 7,
    AddressInUse = 8,
    AddressNotAvailable = 9,
    UnsupportedOperation = 10,
    UnfinishedOperation = 11,
    ProxyAuthenticationRequired = 12,
    SslHandshakeFailed = 13,
    ProxyConnectionRefused = 14,
    ProxyConnectionClosed = 15,
    ProxyConnectionTimeout = 16,
    ProxyNotFound = 17,
    ProxyProtocolError = 18,
    OperationError = 19,
    SslInternalError = 20,
    SslInvalidUserData = 21,
    TemporaryError = 22,
    UnknownError = -1,
};

/**
 * @brief User flags/permissions
 */
enum class UserFlags : quint16 {
    Normal = 0,
    Away = 1,
    Admin = 2,
    Special = 3,
};

/**
 * @brief File type codes (4-character codes)
 */
namespace FileType {
    constexpr const char* Folder = "fldr";
    constexpr const char* JPEG = "JPEG";
    constexpr const char* PNG = "PNGf";
    constexpr const char* BMP = "BMP ";
    constexpr const char* ZIP = "ZIP ";
    constexpr const char* SIT = "SITD";
    constexpr const char* Executable = "DEXE";
}

/**
 * @brief Protocol constants
 */
namespace Constants {
    constexpr quint16 ProtocolVersion = 123;
    constexpr quint16 DefaultPort = 5500;
    constexpr qint32 KeepAliveInterval = 10000; // 10 seconds
    constexpr qint32 MaxChatMessageLength = 32000;
    constexpr qint32 MaxUsernameLength = 255;
}

/**
 * @brief Helper function to convert Parameter enum to quint16
 */
inline constexpr quint16 toInt(Parameter p) {
    return static_cast<quint16>(p);
}

/**
 * @brief Helper function to convert SocketError enum to qint32
 */
inline constexpr qint32 toInt(SocketError e) {
    return static_cast<qint32>(e);
}

} // namespace HotlineProtocol

#endif // HOTLINEPROTOCOL_H
