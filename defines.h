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
    unsigned int reference;
    unsigned int size;
    QString name;

    s_item(unsigned int s, QString str) {
        reference = 0;
        size = s;
        name = str;
    }
};

typedef struct {
    unsigned short id;
    unsigned short length;
    int type;
    char * data;
    unsigned short shortValue;
    unsigned int intValue;
} s_parameter;

typedef struct {
    unsigned short id;
    unsigned short icon;
    QString * iconPath;
    unsigned short flags;
    unsigned short nameLength;
    char * name;
    bool doesCET;
    DialogPrivateMessaging * messagingWindow;
    DialogUserInfo * infoWindow;
    unsigned int orderInList;
} s_user;

typedef struct {
    char * type;
    unsigned int size;
    unsigned short nameSize;
    char * name;
} s_hotlineFile;

#include <QDebug>

#endif // DEFINES_H
