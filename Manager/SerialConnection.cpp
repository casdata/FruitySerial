//
// Created by castdata on 2/17/2023.
//

#include "SerialConnection.h"

SerialConnection::SerialConnection(const std::string &port) {

    portName = port;

    mySerial = new serial::Serial(port, 9600, serial::Timeout::simpleTimeout(1000));

    inputEolType = newLine;
    outputEolType = newLine;

    timeStamp = DISABLE;
    textEncoding = UTF_8_SPECIAL;

    scroll2Bottom = false;
    autoScroll = true;
    listening = true;
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

void SerialConnection::swapListening() {
    listening = !listening;
}

bool SerialConnection::isListening() {
    return listening;
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

    bool restoreStopBits = false;

    try {
        mySerial->setBytesize(static_cast<serial::bytesize_t>(bytesize));
    }
    catch (serial::IOException e){
        std::cout<<" "<<e.what()<<std::endl;
        restoreStopBits = true;
    }

    if(restoreStopBits){
        try{
            mySerial->setStopbits(serial::stopbits_one);
            mySerial->setBytesize(static_cast<serial::bytesize_t>(bytesize));
            mySerial->open();
        }
        catch (serial::IOException e){
            std::cout<<"x2 "<<e.what()<<std::endl;
        }
    }
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

    bool reConnect = false;

    try{
        switch(stopbits){
            case stopbits_one:
                mySerial->setStopbits(serial::stopbits_one);
                break;
            case stopbits_one_point_five:
                mySerial->setStopbits(serial::stopbits_one_point_five);
                break;
            case stopbits_two:
                mySerial->setStopbits(serial::stopbits_two);
                break;
        }
    }
    catch (serial::IOException e){
        std::cout<<" "<<e.what()<<std::endl;
        reConnect = true;
    }



    if(reConnect){
        try{
            mySerial->setStopbits(serial::stopbits_one);
            mySerial->open();
        }catch (serial::IOException e){
            std::cout<<" "<<e.what()<<std::endl;
        }
    }
}

serialStopbits SerialConnection::getStopbits() {

    serialStopbits sStopBits ;

    switch(mySerial->getStopbits()){
        case serial::stopbits_one:
            sStopBits = stopbits_one;
            break;
        case serial::stopbits_two:
            sStopBits = stopbits_two;
            break;
        case serial::stopbits_one_point_five:
            sStopBits = stopbits_one_point_five;
            break;
    }

    return sStopBits;
}

void SerialConnection::setFlowcontrol(serialFlowcontrol flowcontrol) {
    mySerial->setFlowcontrol(static_cast<serial::flowcontrol_t>(flowcontrol));
}

serialFlowcontrol SerialConnection::getFlowcontrol() {
    return static_cast<serialFlowcontrol>(mySerial->getFlowcontrol());
}

void SerialConnection::setTextEnconding(TextEncoding txtEncoding) {
    textEncoding = txtEncoding;
}

TextEncoding SerialConnection::getTextEncoding() {
    return textEncoding;
}

void SerialConnection::setTimeStamp(bool state) {
    timeStamp = state ? EN_TIME : DISABLE;
}

void SerialConnection::swapTimeStamp() {
    timeStamp = timeStamp == EN_TIME ? DISABLE : EN_TIME;
}

bool SerialConnection::isTimeStampEnabled() {
    return timeStamp == EN_TIME ? true : false;
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
    std::string preBuff;

    ImGuiListClipper clipper;

    clipper.Begin(dataLines.size());

    while(clipper.Step()){
        for(int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++){
            if(!(dataLines.at(i)).empty()) {
                preBuff.assign(dataLines.at(i));

                std::string postBuff = " ";

                if (timeStamp == EN_TIME) {
                    postBuff.append(preBuff.substr(0, 13));

                    ImGui::TextUnformatted(postBuff.c_str());
                    ImGui::SameLine(0, 0);

                    postBuff.assign(std::string());

                    postBuff = preBuff.substr(13, 3);
                    ImGui::TextUnformatted(postBuff.c_str());
                    ImGui::SameLine(0, 0);
                }
                else{
                    ImGui::TextUnformatted(postBuff.c_str());
                    ImGui::SameLine(0, 0);
                }

                postBuff.assign(std::string());

                std::string::iterator strIt;

                if(textEncoding == RAW_DEC || textEncoding == RAW_HEX){
                    std::stringstream sStream;

                    bool postBuffActive = false;

                    for(strIt = preBuff.begin() + 16; strIt != preBuff.end(); strIt++) {

                        if(textEncoding == RAW_DEC)
                            sStream<<std::setfill('0')<<std::setw(3)<<std::dec<<static_cast<int>(*strIt);
                        else
                            sStream<<std::uppercase<<std::setfill('0')<<std::setw(2)<<std::hex<<static_cast<int>(*strIt);

                        if(static_cast<int>(*strIt) < 32){
                            if(postBuffActive){
                                postBuffActive = false;
                                ImGui::TextUnformatted(postBuff.c_str());
                                ImGui::SameLine(0,0);

                                postBuff.assign(std::string());
                            }

                            if(uiTheme == DARK)
                                ImGui::PushStyleColor(ImGuiCol_Text, DARK_SPECIAL_UTF8_COL);
                            else
                                ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_SPECIAL_UTF8_COL);

                            ImGui::TextUnformatted(sStream.str().c_str());
                            ImGui::SameLine(0,0);
                            ImGui::TextUnformatted(" ");
                            ImGui::SameLine(0,0);
                            ImGui::PopStyleColor();

                        }
                        else{

                            postBuff.append(sStream.str());
                            postBuff.push_back(' ');

                            postBuffActive = true;
                        }

                        sStream.str(std::string());

                    }

                    if(postBuffActive)
                        ImGui::TextUnformatted(postBuff.c_str());

                    ImGui::NewLine();


                }
                else if(textEncoding == UTF_8_ESP_LOG){

                    bool nullData = true;

                    if(preBuff.length() > 24){

                        if(static_cast<int>(preBuff.at(16)) == 0x1B &&
                                static_cast<int>(preBuff.at(17)) == 0x5B &&
                                static_cast<int>(preBuff.at(18)) == 0x30 &&
                                static_cast<int>(preBuff.at(19)) == 0x3B &&
                                static_cast<int>(preBuff.at(20)) == 0x33 &&
                                static_cast<int>(preBuff.at(22)) == 0x6D){

                            nullData = false;
                            bool pushColor = false;

                            switch(static_cast<int>(preBuff.at(21))){
                                case 0x31:                                                                              //E
                                    pushColor = true;
                                    if(uiTheme == DARK)
                                        ImGui::PushStyleColor(ImGuiCol_Text, DARK_ESP_LOG_E_COL);
                                    else
                                        ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_ESP_LOG_E_COL);
                                    break;
                                case 0x32:                                                                              //I
                                    pushColor = true;
                                    if(uiTheme == DARK)
                                        ImGui::PushStyleColor(ImGuiCol_Text, DARK_ESP_LOG_I_COL);
                                    else
                                        ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_ESP_LOG_I_COL);

                                    break;
                                case 0x33:                                                                              //W
                                    pushColor = true;
                                    if(uiTheme == DARK)
                                        ImGui::PushStyleColor(ImGuiCol_Text, DARK_ESP_LOG_W_COL);
                                    else
                                        ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_ESP_LOG_W_COL);
                                    break;
                            }

                            for(size_t j = 23; j < preBuff.length(); j++){

                                if(static_cast<int>(preBuff.at(j)) == 0x1B && (j + 5) < preBuff.length()){
                                    if(static_cast<int>(preBuff.at(j + 1)) == 0x5B &&
                                            static_cast<int>(preBuff.at(j + 2)) == 0x30 &&
                                            static_cast<int>(preBuff.at(j + 3)) == 0x6D &&
                                            static_cast<int>(preBuff.at(j + 4)) == 0xD &&
                                            static_cast<int>(preBuff.at(j + 5)) == 0xA){

                                        break;
                                    }
                                }
                                else
                                    postBuff.push_back(preBuff.at(j));

                            }

                            ImGui::TextUnformatted(postBuff.c_str());
                            ImGui::SameLine(0,0);

                            if(pushColor)
                                ImGui::PopStyleColor();

                        }


                        else if(static_cast<int>(preBuff.at(17)) == 0x20 &&
                                 static_cast<int>(preBuff.at(18)) == 0x28){

                            nullData = false;
                            bool pushColor = false;

                            switch(static_cast<int>(preBuff.at(16))){
                                case 0x44:                                                                              //D debug
                                    pushColor = true;
                                    if(uiTheme == DARK)
                                        ImGui::PushStyleColor(ImGuiCol_Text, DARK_ESP_LOG_D_COL);
                                    else
                                        ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_ESP_LOG_D_COL);
                                    break;
                                case 0x56:                                                                              //V verbose
                                    pushColor = true;
                                    if(uiTheme == DARK)
                                        ImGui::PushStyleColor(ImGuiCol_Text, DARK_ESP_LOG_V_COL);
                                    else
                                        ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_ESP_LOG_V_COL);
                                    break;
                            }

                            for(size_t j = 16; j < preBuff.length(); j++){

                                if(static_cast<int>(preBuff.at(j)) == 0x1B && (j + 5) < preBuff.length()){
                                    if(static_cast<int>(preBuff.at(j + 1)) == 0x5B &&
                                       static_cast<int>(preBuff.at(j + 2)) == 0x30 &&
                                       static_cast<int>(preBuff.at(j + 3)) == 0x6D &&
                                       static_cast<int>(preBuff.at(j + 4)) == 0xD &&
                                       static_cast<int>(preBuff.at(j + 5)) == 0xA){

                                        break;
                                    }
                                }
                                else
                                    postBuff.push_back(preBuff.at(j));

                            }

                            ImGui::TextUnformatted(postBuff.c_str());
                            ImGui::SameLine(0,0);

                            if(pushColor)
                                ImGui::PopStyleColor();
                        }
                    }

                    if(nullData){
                        ImGui::TextUnformatted(preBuff.substr(16, preBuff.size()).c_str());
                        ImGui::SameLine(0,0);
                    }

                    ImGui::NewLine();
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
                                        postBuffActive = true;
                                        break;
                                }
                                break;
                            case UTF_8_RAW_DEC:
                                if(static_cast<int>(*strIt) < 32)
                                    FunctionTools::printDECorHEX_UTF8(true, *strIt, uiTheme, postBuffActive, postBuff);
                                else{
                                    postBuff.push_back(*strIt);
                                    postBuffActive = true;
                                }

                                break;
                            case UTF_8_RAW_HEX:
                                if(static_cast<int>(*strIt) < 32)
                                    FunctionTools::printDECorHEX_UTF8(false, *strIt, uiTheme, postBuffActive, postBuff);
                                else{
                                    postBuff.push_back(*strIt);
                                    postBuffActive = true;
                                }
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
                        case UTF_8_RAW_DEC:
                        case UTF_8_RAW_HEX:
                            if(postBuffActive)
                                ImGui::TextUnformatted(postBuff.c_str());

                            ImGui::NewLine();
                            break;
                        default:

                            break;
                    }


                }

            }
        }
    }

    clipper.End();

}

void SerialConnection::checkAndReadPort(ClockTime *clockTime) {
    if(mySerial->isOpen()){
        size_t bytesBuff = mySerial->available();

        if(bytesBuff > 0){
            std::string strBuff;
            mySerial->read(strBuff, bytesBuff);

            if(listening) {
                if (dataLines.empty()) {
                    dataLines.push_back(std::string());
                    scroll2Bottom = true;
                }

                std::string strLine = dataLines.back();
                dataLines.pop_back();

                if (strLine.empty()) {
                    strLine.append(clockTime->getTimestamp());
                    strLine.push_back(static_cast<char>(0x20));                 //space
                    FunctionTools::unicode2Utf8(0xBB, strLine);
                    strLine.push_back(static_cast<char>(0x20));                 //space
                }


                auto &dataLinesRef = dataLines;
                auto &scroll2BottomRef = scroll2Bottom;

                auto addNewLine = [&strLine, &dataLinesRef, &scroll2BottomRef]() {
                    dataLinesRef.push_back(strLine);
                    strLine.assign(std::string());
                    scroll2BottomRef = true;
                };

                std::string::iterator strIt;

                bool crSet = false;                                                                                         //carrie return set

                for (strIt = strBuff.begin(); strIt != strBuff.end(); strIt++) {

                    bool preSetCr = false;

                    if (strLine.empty()) {
                        strLine.append(clockTime->getTimestamp());
                        strLine.push_back(static_cast<char>(0x20));                 //space
                        FunctionTools::unicode2Utf8(0xBB, strLine);
                        strLine.push_back(static_cast<char>(0x20));                 //space
                    }

                    strLine.push_back(*strIt);

                    switch (*strIt) {
                        case 10:                                                                                            //new line LF

                            switch (inputEolType) {
                                case newLine:
                                    addNewLine();
                                    break;
                                case crAndLf:
                                    if (crSet)
                                        addNewLine();
                                    break;
                                default:

                                    break;
                            }


                            break;
                        case 13:                                                                                            //carrie return CR

                            if (inputEolType == carrieReturn)
                                addNewLine();
                            else
                                preSetCr = true;

                            break;
                    }

                    if (preSetCr)
                        crSet = true;
                    else
                        crSet = false;


                }

                dataLines.push_back(strLine);

            }
        }
    }
}

void SerialConnection::write2Port(const std::string &data) {

}


SerialConnection::~SerialConnection() {
    delete mySerial;
}









