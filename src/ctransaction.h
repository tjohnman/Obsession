#ifndef CTRANSACTION_H
#define CTRANSACTION_H

#include <vector>
#include "transactionparameter.h"

class CTransaction
{
public:
    CTransaction(const char * headerData);
    CTransaction(qint16 tranID, qint32 taskID);
    ~CTransaction();

    void addData(const char * data);

    char * bytes();
    void addParameter(qint16 parameterID, qint16 parameterLength, const char * parameterData);
    void addParameter(qint16 parameterID, qint32 parameterData);

    quint16 isReply();
    quint16 transactionID();
    quint32 taskID();
    qint32 errorCode();
    quint32 dataLength();
    quint16 numberOfParameters();
    bool done;

    void setDataLength(qint32 l);

    quint32 length();

    TransactionParameter * getParameterById(qint16 parameterID);
    TransactionParameter * getParameter(quint32 i);

private:
    quint16 pIsReply;
    quint16 pTransactionID;
    quint32 pTaskID;
    qint32 pErrorCode;
    quint32 pDataLength;
    quint16 pNumberOfParameters;

    std::vector<TransactionParameter *> pParameters;
};

#endif // CTRANSACTION_H
