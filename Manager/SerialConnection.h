//
// Created by castdata on 2/17/2023.
//

#ifndef TASTYSERIAL_SERIALCONNECTION_H
#define TASTYSERIAL_SERIALCONNECTION_H

#include "imgui.h"
#include "DTypes.h"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include "FunctionTools.h"
#include "ClockTime.h"
#include <iomanip>


class SerialConnection{
public:
    explicit SerialConnection(const std::string &port);

    std::string getPortName();

    void openConnection();
    void closeConnection();
    bool isConnectionOpen();

    void setTimeout(const uint32_t timeout);
    uint32_t getTimeout();

    void setBaudrate(serialBaudrate baudrate);
    serialBaudrate getBaudrate();

    void setBytesize(serialByteSize bytesize);
    serialByteSize getByteSize();

    void setParity(serialParity parity);
    serialParity getParity();

    void setStopbits(serialStopbits stopbits);
    serialStopbits getStopbits();

    void setFlowcontrol(serialFlowcontrol flowcontrol);
    serialFlowcontrol getFlowcontrol();

    bool getScroll2Bottom();

    void setAutoScroll(bool state);
    bool getAutoScroll();

    void update(ClockTime *clockTime);
    void printLines(const UI_Theme& uiTheme);

    void write2Port(const std::string &data);


    virtual ~SerialConnection();
private:

    void checkAndReadPort(ClockTime *clockTime);
    //void printUtf8Line(char std::string& )

    std::string portName;
    serial::Serial *mySerial;
    std::vector<std::string> dataLines;
    serialEndOfLine inputEolType;
    serialEndOfLine outputEolType;

    TimeStamp timeStamp;
    TextEncoding textEncoding;
    bool scroll2Bottom;
    bool autoScroll;

};


#endif //TASTYSERIAL_SERIALCONNECTION_H
