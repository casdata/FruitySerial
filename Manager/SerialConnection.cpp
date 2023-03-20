//
// Created by castdata on 2/17/2023.
//

#include "SerialConnection.h"

SerialConnection::SerialConnection(const std::string &port) {

    portName = port;

    mySerial = new serial::Serial(port, 9600, serial::Timeout::simpleTimeout(1000));

    inputEolType = newLine;
    outputEolType = newLine;

    timeStamp = EN_TIME;
    textEncoding = UTF_8_SPECIAL;

    scroll2Bottom = false;
    autoScroll = true;
}

std::string SerialConnection::getPortName() {
    return portName;
}


void SerialConnection::openConnection() {
    if(!mySerial->isOpen())
        mySerial->open();
}

void SerialConnection::closeConnection() {
    if(mySerial->isOpen())
        mySerial->close();
}

bool SerialConnection::isConnectionOpen() {
    return mySerial->isOpen();
}

void SerialConnection::setTimeout(const uint32_t timeout) {
    serial::Timeout timeout1 = serial::Timeout::simpleTimeout(timeout);
    mySerial->setTimeout(timeout1);
}

uint32_t SerialConnection::getTimeout() {
    serial::Timeout timeout = mySerial->getTimeout();
    return timeout.read_timeout_constant;
}

void SerialConnection::setBaudrate(serialBaudrate baudrate) {
    mySerial->setBaudrate(FunctionTools::baudrateEnum2int(baudrate));
}

serialBaudrate SerialConnection::getBaudrate() {
    return FunctionTools::int2BaudrateEnum(mySerial->getBaudrate());
}

void SerialConnection::setBytesize(serialByteSize bytesize) {
    mySerial->setBytesize(static_cast<serial::bytesize_t>(bytesize));
}

serialByteSize SerialConnection::getByteSize() {
    return static_cast<serialByteSize>(mySerial->getBytesize());
}

void SerialConnection::setParity(serialParity parity) {
    mySerial->setParity(static_cast<serial::parity_t>(parity));
}

serialParity SerialConnection::getParity() {
    return static_cast<serialParity>(mySerial->getParity());
}

void SerialConnection::setStopbits(serialStopbits stopbits) {
    mySerial->setStopbits(static_cast<serial::stopbits_t>(stopbits));
}

serialStopbits SerialConnection::getStopbits() {
    return static_cast<serialStopbits>(mySerial->getStopbits());
}

void SerialConnection::setFlowcontrol(serialFlowcontrol flowcontrol) {
    mySerial->setFlowcontrol(static_cast<serial::flowcontrol_t>(flowcontrol));
}

serialFlowcontrol SerialConnection::getFlowcontrol() {
    return static_cast<serialFlowcontrol>(mySerial->getFlowcontrol());
}

bool SerialConnection::getScroll2Bottom() {
    bool bottomState = scroll2Bottom & autoScroll;
    scroll2Bottom = false;

    return bottomState;
}

void SerialConnection::setAutoScroll(bool state) {
    autoScroll = state;
}

bool SerialConnection::getAutoScroll() {
    return autoScroll;
}


void SerialConnection::update(ClockTime *clockTime) {
    checkAndReadPort(clockTime);
}

void SerialConnection::printLines(const UI_Theme& uiTheme) {
    std::vector<std::string>::iterator linesIt;

    std::string preBuff;

    for(linesIt = dataLines.begin(); linesIt != dataLines.end(); linesIt++){

        if(!(*linesIt).empty()) {
            preBuff.assign(*linesIt);

            std::string postBuff = " ";

            if (timeStamp == EN_TIME)
                postBuff.append(preBuff.substr(0, 13));


            ImGui::TextUnformatted(postBuff.c_str());
            ImGui::SameLine(0,0);

            postBuff.assign(std::string());

            postBuff = preBuff.substr(13, 3);
            ImGui::TextUnformatted(postBuff.c_str());
            ImGui::SameLine(0,0);

            postBuff.assign(std::string());

            std::string::iterator strIt;

            if(textEncoding == RAW_DEC || textEncoding == RAW_HEX){
                std::stringstream sStream;

                for(strIt = preBuff.begin() + 16; strIt != preBuff.end(); strIt++){
                    if(textEncoding == RAW_DEC){
                        sStream<<std::setfill('0')<<std::setw(3)<<std::dec<<static_cast<int>(*strIt);
                        postBuff.append(sStream.str());
                        postBuff.push_back(' ');
                        sStream.str(std::string());
                    }
                    else{
                        sStream<<std::uppercase<<std::setfill('0')<<std::setw(2)<<std::hex<<static_cast<int>(*strIt);
                        postBuff.append(sStream.str());
                        postBuff.push_back(' ');
                        sStream.str(std::string());
                    }
                }

                ImGui::TextUnformatted(postBuff.c_str());

            }
            else{

                bool postBuffActive = false;

                for(strIt = preBuff.begin() + 16; strIt != preBuff.end(); strIt++){

                    switch(textEncoding){
                        case UTF_8:
                            postBuff.push_back(*strIt);
                            break;
                        case UTF_8_SPECIAL:
                            switch(static_cast<int>(*strIt)){
                                case 0:
                                    FunctionTools::printSpecialUTF8("NUL", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 1:
                                    FunctionTools::printSpecialUTF8("SOH", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 2:
                                    FunctionTools::printSpecialUTF8("STX", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 3:
                                    FunctionTools::printSpecialUTF8("ETX", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 4:
                                    FunctionTools::printSpecialUTF8("EOT", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 5:
                                    FunctionTools::printSpecialUTF8("ENQ", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 6:
                                    FunctionTools::printSpecialUTF8("ACK", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 7:
                                    FunctionTools::printSpecialUTF8("BEL", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 8:
                                    FunctionTools::printSpecialUTF8("BS", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 9:
                                    FunctionTools::printSpecialUTF8("TAB", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 10:
                                    FunctionTools::printSpecialUTF8("LF", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 11:
                                    FunctionTools::printSpecialUTF8("VT", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 12:
                                    FunctionTools::printSpecialUTF8("FF", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 13:
                                    FunctionTools::printSpecialUTF8("CR", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 14:
                                    FunctionTools::printSpecialUTF8("SO", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 15:
                                    FunctionTools::printSpecialUTF8("SI", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 16:
                                    FunctionTools::printSpecialUTF8("DLE", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 17:
                                    FunctionTools::printSpecialUTF8("DC1", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 18:
                                    FunctionTools::printSpecialUTF8("DC2", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 19:
                                    FunctionTools::printSpecialUTF8("DC3", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 20:
                                    FunctionTools::printSpecialUTF8("DC4", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 21:
                                    FunctionTools::printSpecialUTF8("NAK", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 22:
                                    FunctionTools::printSpecialUTF8("SYN", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 23:
                                    FunctionTools::printSpecialUTF8("ETB", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 24:
                                    FunctionTools::printSpecialUTF8("CAN", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 25:
                                    FunctionTools::printSpecialUTF8("EM", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 26:
                                    FunctionTools::printSpecialUTF8("SUB", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 27:
                                    FunctionTools::printSpecialUTF8("ESC", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 28:
                                    FunctionTools::printSpecialUTF8("FS", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 29:
                                    FunctionTools::printSpecialUTF8("GS", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 30:
                                    FunctionTools::printSpecialUTF8("RS", uiTheme, postBuffActive, postBuff);
                                    break;
                                case 31:
                                    FunctionTools::printSpecialUTF8("US", uiTheme, postBuffActive, postBuff);
                                    break;
                                default:
                                    postBuff.push_back(*strIt);
                                    //FunctionTools::char2Utf8(*strIt, postBuff);
                                    postBuffActive = true;
                                    break;
                            }


                            break;
                        case UTF_8_RAW_DEC:

                            break;
                        case UTF_8_RAW_HEX:

                            break;
                        default:

                            break;
                    }

                }

                switch(textEncoding){
                    case UTF_8:
                        ImGui::TextUnformatted(postBuff.c_str());
                        break;
                    case UTF_8_SPECIAL:
                        if(postBuffActive)
                            ImGui::TextUnformatted(postBuff.c_str());

                        ImGui::NewLine();
                        break;
                }


            }

        }

    }
}

void SerialConnection::checkAndReadPort(ClockTime *clockTime) {
    if(mySerial->isOpen()){
        size_t bytesBuff = mySerial->available();

        if(bytesBuff > 0){
            std::string strBuff;
            mySerial->read(strBuff, bytesBuff);

            if(dataLines.empty()) {
                dataLines.push_back(std::string());
                scroll2Bottom = true;
            }

            std::string strLine = dataLines.back();
            dataLines.pop_back();

            if(strLine.empty()) {
                strLine.append(clockTime->getTimestamp());
                strLine.push_back(static_cast<char>(0x20));                 //space
                FunctionTools::unicode2Utf8(0xBB, strLine);
                strLine.push_back(static_cast<char>(0x20));                 //space
            }


            auto& dataLinesRef = dataLines;
            auto& scroll2BottomRef = scroll2Bottom;

            auto addNewLine = [&strLine, &dataLinesRef, &scroll2BottomRef]() {
                dataLinesRef.push_back(strLine);
                strLine.assign(std::string());
                scroll2BottomRef = true;
            };

            std::string::iterator strIt;

            bool crSet = false;                                                                                         //carrie return set

            for(strIt = strBuff.begin(); strIt != strBuff.end(); strIt++){

                bool preSetCr = false;

                if(strLine.empty()) {
                    strLine.append(clockTime->getTimestamp());
                    strLine.push_back(static_cast<char>(0x20));                 //space
                    FunctionTools::unicode2Utf8(0xBB, strLine);
                    strLine.push_back(static_cast<char>(0x20));                 //space
                }

                strLine.push_back(*strIt);

                switch(*strIt){
                    case 10:                                                                                            //new line LF

                        switch(inputEolType){
                            case newLine:
                                addNewLine();
                                break;
                            case crAndLf:
                                if(crSet)
                                    addNewLine();
                                break;
                        }


                        break;
                    case 13:                                                                                            //carrie return CR

                        if(inputEolType == carrieReturn)
                            addNewLine();
                        else
                            preSetCr = true;

                        break;
                }

                if(preSetCr)
                    crSet = true;
                else
                    crSet = false;


            }

            dataLines.push_back(strLine);

        }
    }
}

void SerialConnection::write2Port(const std::string &data) {

}


SerialConnection::~SerialConnection() {
    delete mySerial;
}









