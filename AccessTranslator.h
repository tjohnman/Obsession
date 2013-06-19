#ifndef ACCESSTRANSLATOR_H
#define ACCESSTRANSLATOR_H

#include <QtCore>
#include <QDebug>
#include "connectioncontroller.h"

class AccessTranslator
{
public:
    enum : quint64
    {
        DELETE_FILE = (1),
        UPLOAD_FILE = (2),
        DOWNLOAD_FILE = (4),
        RENAME_FILE = (8),
        MOVE_FILE = (16),
        CREATE_FOLDER = (32),
        DELETE_FOLDER = (64),
        RENAME_FOLDER = (128),
        MOVE_FOLDER = (256),
        READ_CHAT = (512),
        SEND_CHAT = (1024),
        OPEN_CHAT = (2048),
        CLOSE_CHAT = (4096),
        SHOW_IN_LIST = (8192),
        CREATE_USER = (16384),
        DELETE_USER = (32768),
        OPEN_USER = (65536),
        MODIFY_USER = (131072),
        CHANGE_OWN_PASSWORD = (262144),
        SEND_PRIV_MESSAGE = (524288),
        READ_NEWS = (1048576),
        POST_NEWS = (2097152),
        DISCONNECT_USER = (4194304),
        CANNOT_BE_DISCONNECTED = (8388608),
        GET_CLIENT_INFO = (16777216),
        UPLOAD_ANYWHERE = (33554432),
        ANY_NAME = (67108864),
        NO_AGREEMENT = (134217728),
        SET_FILE_COMMENT = (268435456),
        SET_FOLDER_COMMENT = (536870912),
        VIEW_DROPBOXES = (1073741824),
        MAKE_ALIAS = (2147483648),
        BROADCAST = (4294967296),
        DELETE_NEWS_ARTICLE = (8589934592),
        CREATE_NEWS_CATEGORY = (17179869184),
        DELETE_NEWS_CATEGORY = (34359738368),
        CREATE_FOLDER_NEWS = (68719476736),
        DELETE_FOLDER_NEWS = (137438953472)
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

    static bool HavePrivileges(quint64 bitmap, quint64 mask)
    {
        return mask & bitmap;
    }

    static void AddPrivilege(quint64 &bitmap, quint64 mask)
    {
        bitmap |= mask;
    }

    static void RemovePrivilege(quint64 &bitmap, quint64 mask)
    {
        bitmap ^= mask;
    }
};

#endif // ACCESSTRANSLATOR_H
