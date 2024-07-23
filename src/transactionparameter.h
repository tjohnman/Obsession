#ifndef TRANSACTIONPARAMETER_H
#define TRANSACTIONPARAMETER_H

#include <QObject>
#include "defines.h"

class TransactionParameter : public QObject
{
    Q_OBJECT
public:
    TransactionParameter(s_parameter parameterData);
    ~TransactionParameter();

    quint16 id();
    quint16 length();
    qint32 type();
    QString toString();
    quint16 toShort();
    quint32 toInt();
    QVariant value();
    const char * data();
    void setData(size_t offset, char byte);

private:
    quint16 pId;
    quint16 pLength;
    qint32 pType;
    char * pData;
};

#endif // TRANSACTIONPARAMETER_H
