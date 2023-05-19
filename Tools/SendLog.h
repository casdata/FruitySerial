//
// Created by castdata on 5/18/2023.
//

#ifndef FRUITYSERIAL_SENDLOG_H
#define FRUITYSERIAL_SENDLOG_H

#include "FunctionTools.h"
#include "DTypes.h"
#include <vector>
#include <iostream>
#include <string>

class SendLog {
public:
    explicit SendLog();

    void addNewLog(const std::string &dataLog);
    std::string* getPreviousLog();
    std::string* getNextLog();

    bool dataOnLogList();

    virtual ~SendLog();

private:

    std::vector<std::string*>    sendLogList;

    int logPC;
    const int LOG_SIZE = 30;

};


#endif //FRUITYSERIAL_SENDLOG_H
