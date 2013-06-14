#ifndef CTRANSACTION_H
#define CTRANSACTION_H

#include <vector>
#include "defines.h"

class CTransaction
{
public:
    CTransaction(char * headerData);
    CTransaction(short tranID, int taskID);

    void addData(char * data);

    char * bytes();
    void addParameter(short parameterID, short parameterLength, char * parameterData);
    void addParameter(short parameterID, int parameterData);

    unsigned short isReply();
    unsigned short transactionID();
    unsigned int taskID();
    int errorCode();
    unsigned int dataLength();
    unsigned short numberOfParameters();
    bool done;

    void setDataLength(int l);

    unsigned int length();

    s_parameter * getParameterById(short parameterID);
    s_parameter * getParameter(unsigned int i);

private:
    unsigned short pIsReply;
    unsigned short pTransactionID;
    unsigned int pTaskID;
    int pErrorCode;
    unsigned int pDataLength;
    unsigned short pNumberOfParameters;

    std::vector<s_parameter *> pParameters;
};

#endif // CTRANSACTION_H
