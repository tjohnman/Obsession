#ifndef ACCESSTRANSLATOR_H
#define ACCESSTRANSLATOR_H

#include <QtCore>
#include <QDebug>
#include "connectioncontroller.h"

class AccessTranslator
{
public:
    enum
    {
        DELETE_FILE = Q_UINT64_C(1),
        UPLOAD_FILE = Q_UINT64_C(2),
        DOWNLOAD_FILE = Q_UINT64_C(4),
        RENAME_FILE = Q_UINT64_C(8),
        MOVE_FILE = Q_UINT64_C(16),
        CREATE_FOLDER = Q_UINT64_C(32),
        DELETE_FOLDER = Q_UINT64_C(64),
        RENAME_FOLDER = Q_UINT64_C(128),
        MOVE_FOLDER = Q_UINT64_C(256),
        READ_CHAT = Q_UINT64_C(512),
        SEND_CHAT = Q_UINT64_C(1024),
        OPEN_CHAT = Q_UINT64_C(2048),
        CLOSE_CHAT = Q_UINT64_C(4096),
        SHOW_IN_LIST = Q_UINT64_C(8192),
        CREATE_USER = Q_UINT64_C(16384),
        DELETE_USER = Q_UINT64_C(32768),
        OPEN_USER = Q_UINT64_C(65536),
        MODIFY_USER = Q_UINT64_C(131072),
        CHANGE_OWN_PASSWORD = Q_UINT64_C(262144),
        SEND_PRIV_MESSAGE = Q_UINT64_C(524288),
        READ_NEWS = Q_UINT64_C(1048576),
        POST_NEWS = Q_UINT64_C(2097152),
        DISCONNECT_USER = Q_UINT64_C(4194304),
        CANNOT_BE_DISCONNECTED = Q_UINT64_C(8388608),
        GET_CLIENT_INFO = Q_UINT64_C(16777216),
        UPLOAD_ANYWHERE = Q_UINT64_C(33554432),
        ANY_NAME = Q_UINT64_C(67108864),
        NO_AGREEMENT = Q_UINT64_C(134217728),
        SET_FILE_COMMENT = Q_UINT64_C(268435456),
        SET_FOLDER_COMMENT = Q_UINT64_C(536870912),
        VIEW_DROPBOXES = Q_UINT64_C(1073741824),
        MAKE_ALIAS = Q_UINT64_C(2147483648),
        BROADCAST = Q_UINT64_C(4294967296),
        CREATE_NEWS_CATEGORY = Q_UINT64_C(8589934592),
        DELETE_NEWS_CATEGORY = Q_UINT64_C(17179869184),
        CREATE_FOLDER_NEWS = Q_UINT64_C(34359738368),
        DELETE_FOLDER_NEWS = Q_UINT64_C(68719476736)
    };

    static bool HavePrivileges(ConnectionController * connection, quint64 mask)
    {
        if(!connection)
        {
            qDebug() << "Privileges translator doesn't have a connection";
            return false;
        }
        return mask & connection->pPermissionBitmap;
    }
};

#endif // ACCESSTRANSLATOR_H
