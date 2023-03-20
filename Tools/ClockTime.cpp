//
// Created by castdata on 2/20/2023.
//

#include "ClockTime.h"


void ClockTime::update() {
    std::tm bt = getTimeStruct();

    if(previousSec != bt.tm_sec){
        previousSec = bt.tm_sec;
        startTime = clock();
    }

}

std::string ClockTime::getTimestamp() {

    std::tm bt = getTimeStruct();

    char buf[32];

    std::string fmt = "%T.";

    std::strftime(buf, sizeof(buf), fmt.c_str(), &bt);

    std::string timestamp(buf);

    int milliseconds = (float(clock() - startTime) / CLOCKS_PER_SEC) * 1000;
    std::string millisStr = std::to_string(milliseconds);
    int precision = 3 - millisStr.length();

    std::string postMillisStr(precision, '0');
    postMillisStr.append(millisStr);

    timestamp.append(postMillisStr);

    return timestamp;
}

std::string ClockTime::getTimestampPlusDate() {

    std::tm bt = getTimeStruct();

    char buf[64];

    std::string fmt = "%F %T.";

    std::strftime(buf, sizeof(buf), fmt.c_str(), &bt);

    std::string timestamp(buf);

    int milliseconds = (float(clock() - startTime) / CLOCKS_PER_SEC) * 1000;
    timestamp.append(std::to_string(milliseconds));
    timestamp.append("|");

    return timestamp;
}

std::tm ClockTime::getTimeStruct() {

    std::time_t timer = std::time(0);
    std::tm bt{};

#ifdef _WIN32
    localtime_s(&bt, &timer);
#elif defined __linux__
    localtime_r(&timer, &bt);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    bt = *std::localtime(&timer);
#endif


    return bt;
}

