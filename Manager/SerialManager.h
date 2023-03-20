//
// Created by castdata on 2/8/2023.
//
#pragma once
#ifndef TASTYSERIAL_SERIALMANAGER_H
#define TASTYSERIAL_SERIALMANAGER_H

#include "imgui.h"
#include "DTypes.h"
#include "ClockTime.h"
#include "SerialConnection.h"
#include <string>
#include <vector>
#include <iostream>



class SerialManager {
public:
    explicit SerialManager(ClockTime *clockTime);

    void updatePortList();

    bool anyPortAvailable();
    bool portsReady2Use();
    std::vector<std::string> getAvailablePortList();

    SerialPortData* getSerialPortDataByPortName(const std::string &portName);
    SerialConnection* newSerialConnection(const std::string &portName);

    void update();

    virtual ~SerialManager();
private:
    std::vector<SerialPortData*> portsDataList;
    std::vector<SerialConnection*> serialConnections;
    ClockTime *clockTime;
};



#endif //TASTYSERIAL_SERIALMANAGER_H
