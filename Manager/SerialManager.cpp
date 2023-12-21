//
// Created by castdata on 2/8/2023.
//

#include "SerialManager.h"


SerialManager::SerialManager(ClockTime *clockTime) {

    this->clockTime = clockTime;

    updatePortList();
}

void SerialManager::updatePortList() {

    std::vector<serial::PortInfo> devices_found = serial::list_ports();

    std::vector<serial::PortInfo>::iterator iter = devices_found.begin();


    std::vector<SerialPortData> tempPortDataList;

    while(iter != devices_found.end()){
        serial::PortInfo device = *iter++;

        SerialPortData spData;
        spData.portInfo = device;
        spData.used = false;
        spData.enable = true;
        spData.updateIt = false;

        tempPortDataList.push_back(spData);
    }

    for(auto* serialData : portsDataList)
        serialData->enable = false;

    std::vector<SerialPortData>::iterator it;

    for(it = tempPortDataList.begin(); it != tempPortDataList.end(); it++){
        std::vector<SerialPortData*>::iterator jt;

        bool add2List = true;
        for(jt = portsDataList.begin(); jt != portsDataList.end(); jt++){
            if((*it).portInfo.port.compare((*jt)->portInfo.port) == 0){
                add2List = false;

                (*jt)->portInfo = (*it).portInfo;
                (*jt)->enable = true;
                break;
            }
        }

        if(add2List){
            auto* serialPortData = new SerialPortData;

            serialPortData->portInfo = (*it).portInfo;
            serialPortData->used = (*it).used;
            serialPortData->enable = (*it).enable;

            portsDataList.push_back(serialPortData);
        }

    }

}

bool SerialManager::anyPortAvailable() {
    return portsDataList.empty() ? false : true;
}

bool SerialManager::portsReady2Use() {

    bool ready = false;

    for(auto* serialPortData : portsDataList){
        if(!serialPortData->used && serialPortData->enable){
            ready = true;
            break;
        }
    }

    return ready;
}

std::vector<std::string> SerialManager::getAvailablePortList() {

    std::vector<std::string> availablePortsList;

    for(auto* serialPortData : portsDataList){
        if(!serialPortData->used && serialPortData->enable && serialPortData->portInfo.hardware_id.compare("n/a"))
            availablePortsList.push_back(serialPortData->portInfo.port);
    }

    return availablePortsList;
}

SerialPortData *SerialManager::getSerialPortDataByPortName(const std::string &portName) {

    for(auto* portData : portsDataList){
        if(portData->portInfo.port.compare(portName) == 0)
            return portData;
    }

    return nullptr;
}

SerialConnection* SerialManager::newSerialConnection(const std::string &portName) {
    auto* serialConnection = new SerialConnection(portName);
    serialConnections.push_back(serialConnection);

    return serialConnection;
}


void SerialManager::update() {

    for(auto *portData : portsDataList){
        if(portData->updateIt){
            portData->updateIt = false;

            if(!portData->used) {                                                                                       //delete serialConnection
                std::vector<SerialConnection*>::iterator it;

                for (it = serialConnections.begin(); it != serialConnections.end(); it++) {
                    if ((*it)->getPortName().compare(portData->portInfo.port) == 0) {
                        delete *it;
                        serialConnections.erase(it);
                        break;
                    }
                }
            }
        }
    }

    std::vector<SerialConnection*>::iterator it;

    for(it = serialConnections.begin(); it != serialConnections.end(); it++)
        (*it)->update(clockTime);


}


SerialManager::~SerialManager() {

    for(auto* portData : portsDataList)
        delete portData;

    portsDataList.clear();


}




