#include "connectionparameter.h"

#include <QtEndian>
#include <QTextCodec>
#include <QSettings>

ConnectionParameter::ConnectionParameter(s_parameter parameterData) {
    QObject();

    pId = parameterData.id;
    pLength = parameterData.length;
    pType = parameterData.type;
    pData = (char *) malloc(pLength);
    memcpy(pData, parameterData.data, pLength);
}

ConnectionParameter::~ConnectionParameter() {
    free(pData);
}

quint16 ConnectionParameter::id() {
    return pId;
}

quint16 ConnectionParameter::length() {
    return pLength;
}

qint32 ConnectionParameter::type() {
    return pType;
}

QString ConnectionParameter::toString() {
    char * buff = (char *) malloc(pLength + 1);
    memcpy(buff, pData, pLength);
    buff[pLength] = '\0';

    QString string = QString(buff);

    QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
    QTextCodec * codec = QTextCodec::codecForName(settings.value(QString::fromUtf8("Encoding"), "Apple Roman").toString().toUtf8());
    if(!codec)
    {
        codec = QTextCodec::codecForName(QString::fromUtf8("Apple Roman").toUtf8());
    }
    string = codec->toUnicode(buff, pLength);
    free(buff);

    return string;
}

quint16 ConnectionParameter::toShort() {
    quint16 result;
    memcpy(&result, pData, 2);
    return qFromBigEndian(result);
}

quint32 ConnectionParameter::toInt() {
    quint32 result;
    memcpy(&result, pData, 4);
    return qFromBigEndian(result);
}

QVariant ConnectionParameter::value() {
    if (pType == TYPE_INT) {
        return QVariant(toInt());
    } else if (pType == TYPE_SHORT) {
        return QVariant(toShort());
    } else if (pType == TYPE_STRING) {
        return QVariant(toString());
    } else {
        QByteArray bytes(pData, pLength);
        return QVariant(bytes);
    }
}




























