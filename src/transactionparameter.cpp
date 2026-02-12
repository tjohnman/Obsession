#include "transactionparameter.h"

#include <QtEndian>
#include <QStringConverter>
#include <QSettings>
#include "SettingsManager.h"

TransactionParameter::TransactionParameter(s_parameter parameterData) {
    QObject();

    pId = parameterData.id;
    pLength = parameterData.length;
    pType = parameterData.type;
    
    // Use vector for automatic memory management
    m_data.resize(pLength);
    memcpy(m_data.data(), parameterData.data, pLength);
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
    // Create a null-terminated copy for string conversion
    std::vector<char> buff(pLength + 1);
    memcpy(buff.data(), m_data.data(), pLength);
    buff[pLength] = '\0';

    auto& settings = SettingsManager::instance();
    QString encodingName = settings.value(QString::fromUtf8("Encoding"), QString::fromUtf8("Apple Roman")).toString();
    
    auto decoder = QStringDecoder(encodingName.toUtf8().constData());
    if(!decoder.isValid())
    {
        decoder = QStringDecoder("Apple Roman");
    }
    QString string = decoder.decode(QByteArrayView(buff.data(), pLength));

    return string;
}

const char * TransactionParameter::data() {
    return m_data.data();
}

void TransactionParameter::setData(size_t offset, char byte) {
    if (offset >= pLength) return;
    m_data[offset] = byte;
}

quint16 TransactionParameter::toShort() {
    quint16 result;
    memcpy(&result, m_data.data(), 2);
    return qFromBigEndian(result);
}

quint32 TransactionParameter::toInt() {
    if (pLength < 4) return (quint32) toShort();
    quint32 result;
    memcpy(&result, m_data.data(), 4);
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
        QByteArray bytes(m_data.data(), pLength);
        return QVariant(bytes);
    }
}




























