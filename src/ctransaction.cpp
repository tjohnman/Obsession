#include "ctransaction.h"
#include <QtEndian>
#include <string.h>

CTransaction::CTransaction(const char * headerData) {
    quint32 dataLengthCheck;

    memcpy(&pIsReply, headerData, 2);
    memcpy(&pTransactionID, headerData + 2, 2);
    memcpy(&pTaskID, headerData + 4, 4);
    memcpy(&pErrorCode, headerData + 8, 4);
    memcpy(&pDataLength, headerData + 12, 4);
    memcpy(&dataLengthCheck, headerData + 16, 4);
    memcpy(&pNumberOfParameters, headerData + 20, 2);

    pIsReply = qFromBigEndian(pIsReply);
    pTransactionID = qFromBigEndian(pTransactionID);
    pTaskID = qFromBigEndian(pTaskID);
    pErrorCode = qFromBigEndian(pErrorCode);
    pDataLength = qFromBigEndian(pDataLength);
    dataLengthCheck = qFromBigEndian(dataLengthCheck);
    pNumberOfParameters = qFromBigEndian(pNumberOfParameters);

    if(dataLengthCheck != pDataLength) {
        qDebug() << "Data length inconsistency.";
        pErrorCode = -1;
    }
    done = false;
}

CTransaction::CTransaction(qint16 tranID, qint32 taskID) {
    pIsReply = 0;
    pTransactionID = tranID;
    pTaskID = taskID;
    pErrorCode = 0;
    pDataLength = 2;
    pNumberOfParameters = 0;
    done = false;
}

CTransaction::~CTransaction() {
    while (!pParameters.empty()) {
        TransactionParameter * parameter = pParameters.back();
        pParameters.pop_back();
        delete parameter;
    }
}

void CTransaction::setDataLength(qint32 l) {
    pDataLength = l;
}

void CTransaction::addData(const char * data) {
    if(pDataLength == 0) {
        qDebug() << "Warning: Tried to read data, but reported data length is zero.";
    }

    qint32 offset = 0;

    for(qint32 i=0; i<pNumberOfParameters; i++) {
        quint16 id, size;

        memcpy(&id, data + offset, 2);
        memcpy(&size, data + 2 + offset, 2);

        id = qFromBigEndian(id);
        size = qFromBigEndian(size);


        s_parameter newParameter;
        newParameter.type = -1;

        newParameter.id = id;
        newParameter.length = size;

        newParameter.data = (char *) malloc(size);
        memcpy(newParameter.data, data + 4 + offset, size);

        memcpy(&newParameter.intValue, data + 4 + offset, 4);
        newParameter.intValue = qFromBigEndian(newParameter.intValue);
        memcpy(&newParameter.shortValue, data + 4 + offset, 2);
        newParameter.shortValue = qFromBigEndian(newParameter.shortValue);

        switch(id) {
        case 100:
            newParameter.type = TYPE_STRING;
            break;
        case 101:
            newParameter.type = TYPE_STRING;
            break;
        case 102:
            newParameter.type = TYPE_STRING;
            break;
        case 103:
            newParameter.type = TYPE_SHORT;
            newParameter.length = 2;
            break;
        case 104:
            newParameter.type = TYPE_SHORT;
            newParameter.length = 2;
            break;
        case 105:
        case 106:
            newParameter.type = TYPE_STRING;
            break;
        case 107:
            newParameter.type = TYPE_INT;
            newParameter.length = 4;
            break;
        case 108:
            if(size <= 2) {
                newParameter.type = TYPE_SHORT;
                newParameter.length = 2;
            } else {
                newParameter.type = TYPE_INT;
                newParameter.length = 4;
            }
            break;
        case 112:
            newParameter.type = TYPE_SHORT;
            newParameter.length = 2;
            break;
        case 110:
        case 115:
            newParameter.type = TYPE_STRING;
        case 116:
            if(size <= 2) {
                newParameter.type = TYPE_SHORT;
                newParameter.length = 2;
            } else {
                newParameter.type = TYPE_INT;
                newParameter.length = 4;
            }
            break;
        case 160:
            newParameter.type = TYPE_SHORT;
            newParameter.length = 2;
            break;
        case 161:
            newParameter.type = TYPE_SHORT;
            newParameter.length = 2;
            break;
        case 162:
            newParameter.type = TYPE_STRING;
            break;
        case 200:
            newParameter.type = TYPE_STRING;
            break;
        case 207:
            if(size <= 2) {
                newParameter.type = TYPE_SHORT;
                newParameter.length = 2;
            } else {
                newParameter.type = TYPE_INT;
                newParameter.length = 4;
            }
            break;
        case 300:
            newParameter.type = TYPE_STRING;
            break;
        case 320:
            newParameter.type = TYPE_STRING;
            break;
        case 321:
            newParameter.type = TYPE_STRING;
            break;
        case 323:
            newParameter.type = TYPE_STRING;
            break;
        case 329:
            newParameter.type = TYPE_STRING;
            break;
        case 330:
            newParameter.type = TYPE_STRING;
            break;
        case 333:
            newParameter.type = TYPE_STRING;
            break;
        default:
            qDebug() << "Warning: Unknown parameter id " << id << "ignored";
        }

        if(newParameter.type != -1) {
            pParameters.push_back(new TransactionParameter(newParameter));
        }

        if (newParameter.data != 0) {
            free(newParameter.data);
        }

        offset += 4 + size;
    }
}

char * CTransaction::bytes() {
    char * b = (char *) malloc(sizeof(char) * length());

    qint16 reply = qToBigEndian(pIsReply);
    memcpy(b, &reply, 2);

    qint16 tranid = qToBigEndian(pTransactionID);
    memcpy(b+2, &tranid, 2);

    qint32 taskid = qToBigEndian(pTaskID);
    memcpy(b+4, &taskid, 4);

    qint32 error = qToBigEndian(pErrorCode);
    memcpy(b+8, &error, 4);

    qint32 len = qToBigEndian(pDataLength);
    memcpy(b+12, &len, 4);
    memcpy(b+16, &len, 4);

    qint16 paramNum = qToBigEndian((qint16)pParameters.size());
    memcpy(b+20, &paramNum, 2);

    qint32 offset = 0;

    for(quint32 i=0; i<pParameters.size(); i++) {
        qint16 id = qToBigEndian(pParameters[i]->id());
        memcpy(b+22+offset, &id, 2);

        qint16 plen = qToBigEndian(pParameters[i]->length());
        memcpy(b+24+offset, &plen, 2);

        qint16 sval, ival;
        switch(pParameters[i]->type()) {
        case TYPE_STRING:
            for(qint32 j=0; j<pParameters[i]->length(); j++) {
                b[26+j + offset] = pParameters[i]->data()[j];
            }
            break;
        case TYPE_SHORT:
            sval = qToBigEndian(pParameters[i]->toShort());
            memcpy(b + 26 + offset, &sval, 2);
            break;
        case TYPE_INT:
            ival = qToBigEndian(pParameters[i]->toInt());
            memcpy(b + 26 + offset, &ival, 4);
            break;
        }
        offset += 4 + pParameters[i]->length();
    }

    return b;
}

void CTransaction::addParameter(qint16 parameterID, qint16 parameterLength, char * parameterData) {
    s_parameter newParameter;
    newParameter.type = TYPE_STRING;
    newParameter.id = parameterID;
    newParameter.length = parameterLength;
    newParameter.data = (char *) malloc(parameterLength);

    memcpy(&newParameter.shortValue, parameterData, 2);
    newParameter.shortValue = qFromBigEndian(newParameter.shortValue);

    memcpy(&newParameter.intValue, parameterData, 4);
    newParameter.intValue = qFromBigEndian(newParameter.intValue);

    memcpy(newParameter.data, parameterData, parameterLength);
    pParameters.push_back(new TransactionParameter(newParameter));
    pNumberOfParameters = pParameters.size();
    pDataLength += parameterLength + 4;
}

void CTransaction::addParameter(qint16 parameterID, qint32 parameterData) {
    s_parameter newParameter;
    newParameter.id = parameterID;

    if(parameterData > 65535) {
        newParameter.type = TYPE_INT;
        newParameter.length = 4;
        newParameter.intValue = parameterData;
        newParameter.shortValue = 0;

        newParameter.data = (char *) malloc(4);
        parameterData = qToBigEndian(parameterData);
        memcpy(newParameter.data, &parameterData, 4);

        pDataLength += 8;
    } else {
        newParameter.type = TYPE_SHORT;
        newParameter.length = 2;
        newParameter.shortValue = parameterData;
        newParameter.intValue = parameterData;

        newParameter.data = (char *) malloc(2);
        quint16 shortParameterData = parameterData;
        shortParameterData = qToBigEndian(shortParameterData);
        memcpy(newParameter.data, &shortParameterData, 2);

        pDataLength += 6;
    }
    pParameters.push_back(new TransactionParameter(newParameter));
    pNumberOfParameters = pParameters.size();

    free(newParameter.data);
}


quint16 CTransaction::isReply() {
    return pIsReply;
}

quint16 CTransaction::transactionID() {
    return pTransactionID;
}

quint32 CTransaction::taskID() {
    return pTaskID;
}

qint32 CTransaction::errorCode() {
    return pErrorCode;
}

quint32 CTransaction::dataLength() {
    return pDataLength;
}

quint16 CTransaction::numberOfParameters() {
    return pNumberOfParameters;
}

quint32 CTransaction::length() {
    qint32 len = 22;

    for(quint32 i=0; i<pParameters.size(); i++) {
        len += pParameters[i]->length() + 4;
    }
    return len;
}

TransactionParameter * CTransaction::getParameterById(qint16 parameterID) {
    for(quint32 i=0; i<pParameters.size(); i++) {
        if(pParameters[i]->id() == parameterID) {
            return pParameters[i];
        }
    }
    return nullptr;
}


TransactionParameter * CTransaction::getParameter(quint32 i) {
    if(i < pParameters.size()) {
        return pParameters[i];
    }
    qDebug() << "Requested inexistent parameter (" << i << " of " << pParameters.size() << ").";
    return nullptr;
}
