//
// Created by castdata on 2/20/2023.
//

#ifndef TASTYSERIAL_CLOCKTIME_H
#define TASTYSERIAL_CLOCKTIME_H

#include <string>
#include <ctime>
#include <chrono>

class ClockTime {
public:
    void update();
    std::string getTimestamp();
    std::string getTimestampPlusDate();
private:
    std::tm getTimeStruct();

    int previousSec;

    clock_t startTime;
};


#endif //TASTYSERIAL_CLOCKTIME_H
