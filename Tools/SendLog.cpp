//
// Created by castdata on 5/18/2023.
//

#include "SendLog.h"

SendLog::SendLog() {
    logPC = 0;

    sendLogList = std::vector<std::string*>();

}

void SendLog::addNewLog(const std::string &dataLog) {

    bool addLog = true;

    if(!sendLogList.empty() && sendLogList.at(sendLogList.size() - 1)){

        if(dataLog.compare(*sendLogList.at(sendLogList.size() - 1)) == 0)
            addLog = false;
    }


    if(addLog) {

        auto *nLog = new std::string(dataLog);

        if (sendLogList.size() >= LOG_SIZE)
            sendLogList.erase(sendLogList.begin());

        sendLogList.push_back(nLog);


        logPC = sendLogList.size();
    }

}

std::string *SendLog::getPreviousLog() {

    if(--logPC < 0)
        logPC = 0;

    return sendLogList.at(logPC);
}

std::string *SendLog::getNextLog() {

    if(++logPC >= sendLogList.size())
        logPC = sendLogList.size() - 1;

    return sendLogList.at(logPC);
}

bool SendLog::dataOnLogList() {
    return !sendLogList.empty();
}

SendLog::~SendLog() {
    for(auto* oldLog: sendLogList){
        oldLog->clear();

        delete oldLog;
    }

    sendLogList.clear();

}

