#include "transactionparameter.h"

#include <QtEndian>
#include <QTextCodec>
#include <QSettings>

TransactionParameter::TransactionParameter(s_parameter parameterData) {
    QObject();

    pId = parameterData.id;
    pLength = parameterData.length;
    pType = parameterData.type;
    pData = (char *) malloc(pLength);
    memcpy(pData, parameterData.data, pLength);
}

TransactionParameter::~TransactionParameter() {
    free(pData);
}

quint16 TransactionParameter::id() {
    return pId;
}

quint16 TransactionParameter::length() {
    return pLength;
}

qint32 TransactionParameter::type() {
    return pType;
}

QString TransactionParameter::toString() {
    char * buff = (char *) malloc(pLength + 1);
    memcpy(buff, pData, pLength);
    buff[pLength] = '\0';

    QString string = QString(buff);

    QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
    QTextCodec * codec = QTextCodec::codecForName(settings.value(QString::fromUtf8("Encoding"), "Apple Roman").toString().toUtf8());
    if(!codec)
    {
        codec = QTextCodec::codecForName("Apple Roman");
    }
    string = codec->toUnicode(buff, pLength);
    free(buff);

    return string;
}

const char * TransactionParameter::data() {
    return pData;
}

void TransactionParameter::setData(size_t offset, char byte) {
    if (offset >= pLength) return;
    pData[offset] = byte;
}

quint16 TransactionParameter::toShort() {
    quint16 result;
    memcpy(&result, pData, 2);
    return qFromBigEndian(result);
}

quint32 TransactionParameter::toInt() {
    if (pLength < 4) return (quint32) toShort();
    quint32 result;
    memcpy(&result, pData, 4);
    return qFromBigEndian(result);
}

QVariant TransactionParameter::value() {
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




























