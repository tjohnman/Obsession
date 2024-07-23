#ifndef CONNECTIONPARAMETER_H
#define CONNECTIONPARAMETER_H

#include <QObject>
#include "defines.h"

class ConnectionParameter : public QObject
{
    Q_OBJECT
public:
    ConnectionParameter(s_parameter parameterData);
    ~ConnectionParameter();

    quint16 id();
    quint16 length();
    qint32 type();
    QString toString();
    quint16 toShort();
    quint32 toInt();
    QVariant value();

private:
    quint16 pId;
    quint16 pLength;
    qint32 pType;
    char * pData;
};

#endif // CONNECTIONPARAMETER_H
