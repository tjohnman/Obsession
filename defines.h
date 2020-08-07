#ifndef DEFINES_H
#define DEFINES_H

#define TYPE_INT 1
#define TYPE_SHORT 2
#define TYPE_STRING 3
#define TYPE_BINARY 4

#include <QString>

class DialogPrivateMessaging;
class DialogUserInfo;

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

#include <QDebug>

#endif // DEFINES_H
