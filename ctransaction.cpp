#include "ctransaction.h"
#include <QtEndian>
#include <string.h>

CTransaction::CTransaction(char * headerData) {
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

    qDebug() << "Total size: " << pDataLength;
    qDebug() << "Data size: " << dataLengthCheck;

    if(dataLengthCheck != pDataLength) {
        qDebug() << "Data length inconsistency";
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

void CTransaction::setDataLength(qint32 l) {
    pDataLength = l;
}

void CTransaction::addData(char * data) {
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

        qDebug() << "Parameter no." << i << " is id: " << id << " (" << size << " bytes)";

        quint16 pshort = 0;
        quint32 pint = 0;

        s_parameter * newParameter = (s_parameter *) malloc(sizeof(s_parameter));
        newParameter->type = -1;

        switch(id) {
        case 100:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 101:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 102:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 103:
            memcpy(&pshort, data + 4+ offset, 2);
            newParameter->type = TYPE_SHORT;
            newParameter->id = id;
            newParameter->length = 2;
            newParameter->shortValue = pshort;
            break;
        case 104:
            memcpy(&pshort, data + 4+ offset, 2);
            newParameter->type = TYPE_SHORT;
            newParameter->id = id;
            newParameter->length = 2;
            newParameter->shortValue = pshort;
            break;
        case 105:
        case 106:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 107:
            memcpy(&pint, data + 4+ offset, 4);
            newParameter->type = TYPE_INT;
            newParameter->id = id;
            newParameter->length = 4;
            newParameter->intValue = pint;
            break;
        case 108:
            if(size <= 2) {
                memcpy(&pshort, data + 4+ offset, 2);
                newParameter->type = TYPE_SHORT;
                newParameter->id = id;
                newParameter->length = 2;
                newParameter->shortValue = pshort;
            } else {
                memcpy(&pint, data + 4+ offset, 4);
                newParameter->type = TYPE_INT;
                newParameter->id = id;
                newParameter->length = 4;
                newParameter->intValue = pint;
            }
            break;
        case 112:
            memcpy(&pshort, data + 4+ offset, 2);
            newParameter->type = TYPE_SHORT;
            newParameter->id = id;
            newParameter->length = 2;
            newParameter->shortValue = pshort;
            break;
        case 110:
        case 115:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
        case 116:
            if(size <= 2) {
                memcpy(&pshort, data + 4+ offset, 2);
                newParameter->type = TYPE_SHORT;
                newParameter->id = id;
                newParameter->length = 2;
                newParameter->shortValue = pshort;
            } else {
                memcpy(&pint, data + 4+ offset, 4);
                newParameter->type = TYPE_INT;
                newParameter->id = id;
                newParameter->length = 4;
                newParameter->intValue = pint;
            }
            break;
        case 160:
            memcpy(&pshort, data + 4+ offset, 2);
            newParameter->type = TYPE_SHORT;
            newParameter->id = id;
            newParameter->length = 2;
            newParameter->shortValue = pshort;
            break;
        case 161:
            memcpy(&pshort, data + 4+ offset, 2);
            newParameter->type = TYPE_SHORT;
            newParameter->id = id;
            newParameter->length = 2;
            newParameter->shortValue = pshort;
            break;
        case 162:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 200:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 207:
            if(size <= 2) {
                memcpy(&pshort, data + 4+ offset, 2);
                newParameter->type = TYPE_SHORT;
                newParameter->id = id;
                newParameter->length = 2;
                newParameter->shortValue = pshort;
            } else {
                memcpy(&pint, data + 4+ offset, 4);
                newParameter->type = TYPE_INT;
                newParameter->id = id;
                newParameter->length = 4;
                newParameter->intValue = pint;
            }
            break;
        case 300:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 320:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 321:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 323:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 329:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 330:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        case 333:
            newParameter->type = TYPE_STRING;
            newParameter->id = id;
            newParameter->length = size;
            newParameter->data = (char *) malloc(sizeof(char)*size);
            memcpy(newParameter->data, data + 4 + offset, size);
            break;
        default:
            qDebug() << "Warning: Unknown parameter id " << id << "ignored";
        }

        if(newParameter->type != -1) {
            pParameters.push_back(newParameter);
        }

        offset += 4 + size;
    }
}

char * CTransaction::bytes() {
    char * b = (char *) malloc(sizeof(char) * length());

    qDebug() << "Transaction is " << length() << " bytes";

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
        qint16 id = qToBigEndian(pParameters[i]->id);
        memcpy(b+22+offset, &id, 2);

        qint16 plen = qToBigEndian(pParameters[i]->length);
        memcpy(b+24+offset, &plen, 2);

        qint16 sval, ival;
        switch(pParameters[i]->type) {
        case TYPE_STRING:
            for(qint32 j=0; j<pParameters[i]->length; j++) {
                b[26+j + offset] = pParameters[i]->data[j];
            }
            break;
        case TYPE_SHORT:
            sval = qToBigEndian(pParameters[i]->shortValue);
            memcpy(b + 26 + offset, &sval, 2);
            break;
        case TYPE_INT:
            ival = qToBigEndian(pParameters[i]->intValue);
            memcpy(b + 26 + offset, &ival, 4);
            break;
        }
        offset += 4 + pParameters[i]->length;
    }
    qDebug() << "Built transaction bytes";
    return b;
}

void CTransaction::addParameter(qint16 parameterID, qint16 parameterLength, char * parameterData) {
    s_parameter * newParameter = (s_parameter *) malloc(sizeof(s_parameter));
    newParameter->type = TYPE_STRING;
    newParameter->id = parameterID;
    newParameter->length = parameterLength;
    newParameter->data = (char *) malloc(parameterLength);
    memcpy(newParameter->data, parameterData, parameterLength);
    pParameters.push_back(newParameter);
    pNumberOfParameters = pParameters.size();
    pDataLength += parameterLength + 4;
}

void CTransaction::addParameter(qint16 parameterID, qint32 parameterData) {
    s_parameter * newParameter = (s_parameter *) malloc(sizeof(s_parameter));
    newParameter->id = parameterID;
    if(parameterData > 65535) {
        newParameter->type = TYPE_INT;
        newParameter->length = 4;
        newParameter->intValue = parameterData;
        pDataLength += 8;
    } else {
        newParameter->type = TYPE_SHORT;
        newParameter->length = 2;
        newParameter->shortValue = parameterData;
        newParameter->intValue = parameterData;
        pDataLength += 6;
    }
    pParameters.push_back(newParameter);
    pNumberOfParameters = pParameters.size();
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
        len += pParameters[i]->length + 4;
    }
    return len;
}

s_parameter * CTransaction::getParameterById(qint16 parameterID) {
    for(quint32 i=0; i<pParameters.size(); i++) {
        if(pParameters[i]->id == parameterID) {
            return pParameters[i];
        }
    }
    return NULL;
}


s_parameter * CTransaction::getParameter(quint32 i) {
    if(i < pParameters.size()) {
        return pParameters[i];
    }
    qDebug() << "Requested inexistent parameter! (" << i << " of " << pParameters.size() << ")";
    return NULL;
}
