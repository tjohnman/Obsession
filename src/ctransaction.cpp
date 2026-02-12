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

        // Determine parameter type based on ID
        qint32 paramType = -1;
        quint16 paramLength = size;
        
        switch(id) {
        case 100:
        case 101:
        case 102:
            paramType = TYPE_STRING;
            break;
        case 103:
        case 104:
            paramType = TYPE_SHORT;
            paramLength = 2;
            break;
        case 105:
        case 106:
            paramType = TYPE_STRING;
            break;
        case 107:
            paramType = TYPE_INT;
            paramLength = 4;
            break;
        case 108:
            if(size <= 2) {
                paramType = TYPE_SHORT;
                paramLength = 2;
            } else {
                paramType = TYPE_INT;
                paramLength = 4;
            }
            break;
        case 112:
            paramType = TYPE_SHORT;
            paramLength = 2;
            break;
        case 110:
        case 115:
            paramType = TYPE_STRING;
            break;
        case 116:
            if(size <= 2) {
                paramType = TYPE_SHORT;
                paramLength = 2;
            } else {
                paramType = TYPE_INT;
                paramLength = 4;
            }
            break;
        case 160:
        case 161:
            paramType = TYPE_SHORT;
            paramLength = 2;
            break;
        case 162:
        case 200:
            paramType = TYPE_STRING;
            break;
        case 207:
            if(size <= 2) {
                paramType = TYPE_SHORT;
                paramLength = 2;
            } else {
                paramType = TYPE_INT;
                paramLength = 4;
            }
            break;
        case 300:
        case 320:
        case 321:
        case 323:
        case 327:  // Unknown news parameter - handle as string
        case 328:  // Unknown news parameter - handle as string
        case 329:
        case 330:
        case 331:  // Unknown news parameter - handle as string
        case 332:  // Unknown news parameter - handle as string
        case 333:
        case 335:  // Unknown news parameter - handle as string
        case 336:  // Unknown news parameter - handle as string
            paramType = TYPE_STRING;
            break;
        default:
            qDebug() << "Warning: Unknown parameter id " << id << "ignored";
        }

        if(paramType != -1) {
            pParameters.push_back(new TransactionParameter(id, paramLength, paramType, data + 4 + offset));
        }

        offset += 4 + size;
    }
}

QByteArray CTransaction::toByteArray() const {
    QByteArray buffer(length(), Qt::Uninitialized);
    char* b = buffer.data();

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

    qint16 paramNum = qToBigEndian(static_cast<qint16>(pParameters.size()));
    memcpy(b+20, &paramNum, 2);

    qint32 offset = 0;

    for(quint32 i=0; i<pParameters.size(); i++) {
        qint16 id = qToBigEndian(pParameters[i]->id());
        memcpy(b+22+offset, &id, 2);

        qint16 plen = qToBigEndian(pParameters[i]->length());
        memcpy(b+24+offset, &plen, 2);

        qint16 sval;
        qint32 ival;
        
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

    return buffer;
}

void CTransaction::addParameter(qint16 parameterID, qint16 parameterLength, const char * parameterData) {
    pParameters.push_back(new TransactionParameter(parameterID, parameterLength, TYPE_STRING, parameterData));
    pNumberOfParameters = pParameters.size();
    pDataLength += parameterLength + 4;
}

void CTransaction::addParameter(qint16 parameterID, qint32 parameterData) {
    if(parameterData > 65535) {
        // Use 4-byte INT type
        char buffer[4];
        quint32 bigEndianData = qToBigEndian(parameterData);
        memcpy(buffer, &bigEndianData, 4);
        
        pParameters.push_back(new TransactionParameter(parameterID, 4, TYPE_INT, buffer));
        pDataLength += 8;
    } else {
        // Use 2-byte SHORT type
        char buffer[2];
        quint16 shortData = static_cast<quint16>(parameterData);
        quint16 bigEndianData = qToBigEndian(shortData);
        memcpy(buffer, &bigEndianData, 2);
        
        pParameters.push_back(new TransactionParameter(parameterID, 2, TYPE_SHORT, buffer));
        pDataLength += 6;
    }
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

quint32 CTransaction::length() const {
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
