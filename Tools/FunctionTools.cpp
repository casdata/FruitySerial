//
// Created by castdata on 1/18/23.
//

#include <iomanip>
#include "FunctionTools.h"
#include "imgui_internal.h"

bool FunctionTools::loadTextureFromFile(const char *filename, GLuint *outTexture, int *outWidth, int *outHeight) {

    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *outTexture = image_texture;
    *outWidth = image_width;
    *outHeight = image_height;


    return true;
}

bool FunctionTools::replaceData2File(const char *fileName, const int position, const int bytesSize, const char *data) {

    std::ifstream readFile(fileName);

    if(readFile.is_open()){

        readFile.seekg(0, readFile.end);
        int length = readFile.tellg();
        readFile.seekg(0, readFile.beg);

        char* buffer = new char[length];

        readFile.read(buffer, length);
        readFile.close();

        for(size_t i = 0; i < bytesSize; i++)
            *(buffer + (i + position)) = (char)*(data + i);


        std::ofstream writeFile(fileName, std::ofstream::out | std::ofstream::trunc);

        if(writeFile.is_open()){
            writeFile.write(buffer, length);

            delete[] buffer;
            return true;
        }
        else {
            delete[] buffer;
            return false;
        }
    }
    else
        return false;


}

void FunctionTools::imGuiAlignForWidth(const float width, const float alignment) {
    ImGuiStyle &style = ImGui::GetStyle();
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if(off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

ImVec2 FunctionTools::getMouseRealPos(const int &windowPosX, const int &windowPosY) {

    float x,y;
    ImVec2 rawMousePos = ImGui::GetMousePos();

    //std::cout<<"-> "<<rawMousePos.x<<" "<<rawMousePos.y<<" - "<<windowPosX<<" "<<windowPosY<<std::endl;

    if(windowPosX < 0)
        x = rawMousePos.x + (windowPosX * (-1));
    else
        x = rawMousePos.x - windowPosX;

    if(windowPosY < 0)
        y = rawMousePos.y + (windowPosY * (-1));
    else
        y = rawMousePos.y - windowPosY;

    //std::cout<<"x-> "<<x<<" "<<rawMousePos.x<<" "<<windowPosX<<" - "<<y<<" "<<rawMousePos.y<<" "<<windowPosY<<std::endl;
    return ImVec2(x, y);
}

int FunctionTools::getRealValue(const int &rawValue, const int &referenceValue) {
    int value;

    if(referenceValue < 0)
        value = rawValue + (referenceValue * (-1));
    else
        value = rawValue - referenceValue;

    return value;
}

bool
FunctionTools::btnStateInsideArea(BtnState &&btnState, const BtnState &mouseBtn, const ImVec2 &cursorPos, ImVec2 &&areaXY, ImVec2 &&areaHW) {

    if(mouseBtn == btnState){
        if(cursorPos.x > areaXY.x && cursorPos.x < areaHW.x){
            if(cursorPos.y > areaXY.y && cursorPos.y < areaHW.y)
                return true;
        }
    }

    return false;
}

uint32_t FunctionTools::baudrateEnum2int(serialBaudrate sBaudrate) {

    switch(sBaudrate){
        case baud_300:
            return 300;
            break;
        case baud_1200:
            return 1200;
            break;
        case baud_2400:
            return 2400;
            break;
        case baud_4800:
            return 4800;
            break;
        case baud_9600:
            return 9600;
            break;
        case baud_19200:
            return 19200;
            break;
        case baud_38400:
            return 38400;
            break;
        case baud_57600:
            return 57600;
            break;
        case baud_74880:
            return 74880;
            break;
        case baud_115200:
            return 115200;
            break;
        case baud_230400:
            return 230400;
            break;
        case baud_250000:
            return 250000;
            break;
        case baud_500000:
            return 500000;
            break;
        case baud_1000000:
            return 1000000;
            break;
        case baud_2000000:
            return 2000000;
            break;
        default:
            return 9600;
    }

}

serialBaudrate FunctionTools::int2BaudrateEnum(uint32_t baudrate) {

    switch(baudrate){
        case 300:
            return baud_300;
            break;
        case 1200:
            return baud_1200;
            break;
        case 2400:
            return baud_2400;
            break;
        case 4800:
            return baud_4800;
            break;
        case 9600:
            return baud_9600;
            break;
        case 19200:
            return baud_19200;
            break;
        case 38400:
            return baud_38400;
            break;
        case 57600:
            return baud_57600;
            break;
        case 74880:
            return baud_74880;
            break;
        case 115200:
            return baud_115200;
            break;
        case 230400:
            return baud_230400;
            break;
        case 250000:
            return baud_250000;
            break;
        case 500000:
            return baud_500000;
            break;
        case 1000000:
            return baud_1000000;
            break;
        case 2000000:
            return baud_2000000;
            break;
        default:
            return baud_9600;
    }


}

std::string FunctionTools::textEncodingEnum2charptr(TextEncoding tEncoding) {

    switch(tEncoding){
        case RAW_DEC:
            return std::string("RAW-DEC");
            break;
        case RAW_HEX:
            return std::string("RAW-HEX");
            break;
        case UTF_8_RAW_DEC:
            return std::string("UTF-8 RAW-DEC");
            break;
        case UTF_8_RAW_HEX:
            return std::string("UTF-8 RAW-HEX");
            break;
        case UTF_8_SPECIAL:
            return std::string("UTF-8 RULES");
            break;
        case UTF_8_ESP_LOG:
            return std::string("UTF-8 ESP-LOG");
            break;
        default:
            return std::string("UTF-8");
    }

}

TextEncoding FunctionTools::int2TextEncodingEnum(uint32_t tEncodingIndex) {
    return static_cast<TextEncoding>(tEncodingIndex);
}

void FunctionTools::char2Utf8(char character, std::string &str) {
    unsigned int unicode = static_cast<int>(character);

    if (unicode <= 0x7f)
        str.append(1, static_cast<char>(unicode));
    else if (unicode <= 0x7ff){
        str.append(1, static_cast<char>(0xc0 | ((unicode >> 6) & 0x1f)));
        str.append(1, static_cast<char>(0x80 | (unicode & 0x3f)));
    }
}

void FunctionTools::unicode2Utf8(unsigned int unicode, std::string &str) {

    if (unicode <= 0x7f)
        str.append(1, static_cast<char>(unicode));
    else if (unicode <= 0x7ff){
        str.append(1, static_cast<char>(0xc0 | ((unicode >> 6) & 0x1f)));
        str.append(1, static_cast<char>(0x80 | (unicode & 0x3f)));
    }
    else if (unicode <= 0xffff){
        str.append(1, static_cast<char>(0xe0 | ((unicode >> 12) & 0x0f)));
        str.append(1, static_cast<char>(0x80 | ((unicode >> 6) & 0x3f)));
        str.append(1, static_cast<char>(0x80 | (unicode & 0x3f)));
    }
    else{
        str.append(1, static_cast<char>(0xf0 | ((unicode >> 18) & 0x07)));
        str.append(1, static_cast<char>(0x80 | ((unicode >> 12) & 0x3f)));
        str.append(1, static_cast<char>(0x80 | ((unicode >> 6) & 0x3f)));
        str.append(1, static_cast<char>(0x80 | (unicode & 0x3f)));
    }

}

void FunctionTools::printDECorHEX_UTF8(bool&& decType, const char charData, const UI_Theme &uiTheme, bool &postBuffActive,
                                       std::string &postBuff, const bool inColor, const Fruits *fruits, const ImU32 *txtColor) {

    if(postBuffActive){
        postBuffActive = false;

        if (inColor && *fruits == WITH_FRUITS)
            ImGui::PushStyleColor(ImGuiCol_Text, *txtColor);

        ImGui::TextUnformatted(postBuff.c_str());
        ImGui::SameLine(0,0);

        if (inColor && *fruits == WITH_FRUITS)
            ImGui::PopStyleColor();

        postBuff.assign(std::string());
    }

    if(uiTheme == DARK)
        ImGui::PushStyleColor(ImGuiCol_Text, DARK_BRACKET_COL);
    else
        ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_BRACKET_COL);

    ImGui::TextUnformatted("[");
    ImGui::SameLine(0,0);

    if(uiTheme == DARK)
        ImGui::PushStyleColor(ImGuiCol_Text, DARK_SPECIAL_UTF8_COL);
    else
        ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_SPECIAL_UTF8_COL);

    std::stringstream sStream;

    if(decType)
        sStream<<std::setfill('0')<<std::setw(3)<<std::dec<<static_cast<int>(charData);
    else
        sStream<<std::uppercase<<std::setfill('0')<<std::setw(2)<<std::hex<<static_cast<int>(charData);

    ImGui::TextUnformatted(sStream.str().c_str());
    ImGui::SameLine(0,0);
    ImGui::PopStyleColor();
    ImGui::TextUnformatted("]");
    ImGui::SameLine(0,0);
    ImGui::PopStyleColor();

}

void FunctionTools::printSpecialUTF8(std::string&& name, const UI_Theme &uiTheme, bool& postBuffActive, std::string& postBuff, const bool inColor, const Fruits *fruits, const ImU32 *txtColor) {


    if(postBuffActive) {
        postBuffActive = false;

        if (inColor && *fruits == WITH_FRUITS)
            ImGui::PushStyleColor(ImGuiCol_Text, *txtColor);

        ImGui::TextUnformatted(postBuff.c_str());
        ImGui::SameLine(0,0);

        if (inColor && *fruits == WITH_FRUITS)
            ImGui::PopStyleColor();

        postBuff.assign(std::string());
    }

    if(uiTheme == DARK)
        ImGui::PushStyleColor(ImGuiCol_Text, DARK_BRACKET_COL);
    else
        ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_BRACKET_COL);

    ImGui::TextUnformatted("[");
    ImGui::SameLine(0,0);

    if(uiTheme == DARK)
        ImGui::PushStyleColor(ImGuiCol_Text, DARK_SPECIAL_UTF8_COL);
    else
        ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_SPECIAL_UTF8_COL);

    ImGui::TextUnformatted(name.c_str());
    ImGui::SameLine(0,0);
    ImGui::PopStyleColor();
    ImGui::TextUnformatted("]");
    ImGui::SameLine(0,0);
    ImGui::PopStyleColor();
}

float FunctionTools::norm2HeightFloat(const int &value, bool setLegacyHeight, int newHeight) {
    //return static_cast<int>((static_cast<float>(value) * originalHeight) / LEGACY_HEIGHT);
    static int originalHeight = 0;

    if(setLegacyHeight) {
        originalHeight = newHeight;
        return 0;
    }
    else{
        return ((value * originalHeight)/LEGACY_HEIGHT);
    }

}

int FunctionTools::norm2Height(const int &value, bool setLegacyHeight, int newHeight) {
    //return static_cast<int>((static_cast<float>(value) * originalHeight) / LEGACY_HEIGHT);
    static int originalHeight = 0;

    if(setLegacyHeight) {
        originalHeight = newHeight;
        return 0;
    }
    else{
        return static_cast<int>(((value * originalHeight)/LEGACY_HEIGHT));
    }

}

int FunctionTools::norm2Width(const int &value, bool setLegacyWidth, int newWidth) {
    static int originalWidth = 0;

    if(setLegacyWidth) {
        originalWidth = newWidth;
        return 0;
    }
    else{
        return static_cast<int>(((value * originalWidth)/LEGACY_WIDTH));
    }
}

bool FunctionTools::increasePC2NextChar(const std::string *strBuffer, int *pCounter, const int lastIndex) {                                        //Returns false if no char found

    bool noOutOfSizeError = true;

    while(strBuffer->at(*pCounter) == ' '){
        (*pCounter)++;

        if(*pCounter >= lastIndex){
            noOutOfSizeError = false;
            break;
        }
    }

    return noOutOfSizeError;
}

bool FunctionTools::increasePC2NextSpace(const std::string *strBuffer, int *pCounter, const int lastIndex) {

    bool noOutOfSizeError = true;

    while(strBuffer->at(*pCounter) != ' '){
        (*pCounter)++;

        if(*pCounter >= lastIndex){
            noOutOfSizeError = false;
            break;
        }
    }

    return noOutOfSizeError;
}

bool FunctionTools::increasePC2NextSpaceOrLimit(const std::string *strBuffer, int *pCounter, const int lastIndex) {

    bool noOutOfSizeError = true;

    while(strBuffer->at(*pCounter) != ' ' && strBuffer->at(*pCounter) != ']' ){
        (*pCounter)++;

        if(*pCounter >= lastIndex){
            noOutOfSizeError = false;
            break;
        }
    }

    return noOutOfSizeError;
}

int FunctionTools::numberOfChars2SpaceOrCloseKey(const std::string &strBuffer, const int pCounter, const int lastIndex) {
    int numOfChars = 0;

    for(size_t i = (pCounter + 1); i <= lastIndex; i++){
        if(strBuffer.at(i) == ' '){
            numOfChars = i;
            break;
        }

        numOfChars = i;
    }

    numOfChars -= pCounter;


    return numOfChars;
}

bool FunctionTools::decodeColor(const std::string *strBuffer, int *pCounter, const int lastIndex, ImU32 *nColor, ImU32 *prevColor, const UI_Theme &uiTheme){
    bool isOk = false;

    typedef enum {NONE, TYPE_3, TYPE_6} HexType;
    HexType hexType = NONE;

    if(strBuffer->at(*pCounter) == '#'){

        int temp_pCounter = (*pCounter) + 1;
        if(increasePC2NextSpaceOrLimit(strBuffer, &temp_pCounter, (lastIndex + 1))){
            if((temp_pCounter - *pCounter) == 7) {
                hexType = TYPE_6;
                isOk = true;
            }
            else if((temp_pCounter - *pCounter) == 4) {
                hexType = TYPE_3;
                isOk = true;
            }
        }


        if(hexType != NONE){
            for(size_t i = (*pCounter + 1); i < temp_pCounter; i++){
                if(strBuffer->at(i) < 0x30 || (strBuffer->at(i) < 0x41 && strBuffer->at(i) > 0x39)
                   || (strBuffer->at(i) > 0x46 && strBuffer->at(i) < 0x61 )
                   || strBuffer->at(i) > 0x66){
                    hexType = NONE;
                    isOk = false;
                    break;
                }
            }
        }

        if(hexType != NONE && temp_pCounter != lastIndex){
            if(increasePC2NextChar(strBuffer, &temp_pCounter, (lastIndex + 1))){
                if(strBuffer->at(temp_pCounter) != ']')
                    isOk = false;
            }
            else
                isOk = false;
        }

        if(isOk){
            int rValue = 0;
            int gValue = 0;
            int bValue = 0;

            std::string intStr;

            switch(hexType){
                case TYPE_3:
                    intStr = strBuffer->substr((*pCounter + 1), 1);
                    rValue = std::stoi(intStr, 0, 16);
                    rValue = (rValue * 255)/15;

                    intStr = strBuffer->substr((*pCounter + 2), 1);
                    gValue = std::stoi(intStr, 0, 16);
                    gValue = (gValue * 255)/15;

                    intStr = strBuffer->substr((*pCounter + 3), 1);
                    bValue = std::stoi(intStr, 0, 16);
                    bValue = (bValue * 255)/15;

                    *prevColor = *nColor;
                    *nColor = IM_COL32(rValue, gValue, bValue, 255);

                    break;
                case TYPE_6:
                    intStr = strBuffer->substr((*pCounter + 1), 2);
                    rValue = std::stoi(intStr, 0, 16);

                    intStr = strBuffer->substr((*pCounter + 3), 2);
                    gValue = std::stoi(intStr, 0, 16);

                    intStr = strBuffer->substr((*pCounter + 5), 2);
                    bValue = std::stoi(intStr, 0, 16);

                    *prevColor = *nColor;
                    *nColor = IM_COL32(rValue, gValue, bValue, 255);

                    break;
                default:

                    break;
            }
        }
    }
    else if(strBuffer->at(*pCounter) != ' '){
        std::string colorStr = strBuffer->substr(*pCounter, (lastIndex - *pCounter));

        std::unordered_map<std::string, ImU32>::const_iterator mapIt;

        if(uiTheme == DARK){
            mapIt = darkColorMap.find(colorStr);

            if(mapIt != darkColorMap.end()){
                *prevColor = *nColor;
                *nColor = mapIt->second;
                isOk = true;
            }
        }
        else{
            mapIt = lightColorMap.find(colorStr);

            if(mapIt != lightColorMap.end()){
                *prevColor = *nColor;
                *nColor = mapIt->second;
                isOk = true;
            }
        }
    }


    return isOk;
}

bool FunctionTools::isStartDEC_format(const std::string &strBuffer, int &pCounter, const int lastIndex) {

    bool isDecFormat = true;

    int prePCounter = pCounter;

    for (size_t i = 0; i < 3; i++) {
        if (pCounter < lastIndex) {
            if (strBuffer.at(pCounter) < 0x30 || strBuffer.at(pCounter) > 0x39) {
                isDecFormat = false;
                break;
            }
        } else {
            isDecFormat = false;
            break;
        }

        pCounter++;
    }

    if(isDecFormat){

        int sumA = (static_cast<int>(strBuffer.at(prePCounter)) - 0x30) * 100;
        sumA += (static_cast<int>(strBuffer.at(prePCounter + 1)) - 0x30) * 10;
        sumA += static_cast<int>(strBuffer.at(prePCounter + 2)) - 0x30;

        if(sumA > 255)
            isDecFormat = false;
    }

    return isDecFormat;
}

bool FunctionTools::isDEC_format(const std::string &strBuffer, int &pCounter, const int lastIndex) {
    bool isDecFormat = false;

    int nextSpace = numberOfChars2SpaceOrCloseKey(strBuffer, pCounter, lastIndex);


    if(nextSpace > 0 && nextSpace < 4){
        isDecFormat = true;

        if(strBuffer.at(pCounter + nextSpace) == ' ' || strBuffer.at(pCounter + nextSpace) == ']'){

            for(size_t i = 0; i < nextSpace; i++){
                if (strBuffer.at(pCounter) < 0x30 || strBuffer.at(pCounter) > 0x39)
                    isDecFormat = false;

                pCounter++;
            }
        }
        else
            pCounter += nextSpace;

    }
    else
        pCounter += nextSpace;


    return isDecFormat;
}


bool FunctionTools::isStartOCT_format(const std::string &strBuffer, int &pCounter, const int lastIndex) {

    bool isOctFormat = false;

    if(strBuffer.at(pCounter) == '0'){
        pCounter++;

        if(pCounter < lastIndex && (strBuffer.at(pCounter) == 'o' || strBuffer.at(pCounter) == 'O')){
            int nextSpace = numberOfChars2SpaceOrCloseKey(strBuffer, pCounter, lastIndex);

            if(nextSpace < 5 && nextSpace > 1){
                isOctFormat = true;

                if(strBuffer.at(pCounter + nextSpace) == ' ' || strBuffer.at(pCounter + nextSpace) == ']'){
                    pCounter++;

                    for(size_t i = 0; i < (nextSpace - 1); i++){
                        if(strBuffer.at(pCounter) < 0x30 || strBuffer.at(pCounter) > 0x37)
                            isOctFormat = false;

                        pCounter++;
                    }
                }
            }


        }
    }

    return isOctFormat;
}

bool FunctionTools::isOCT_format(const std::string &strBuffer, int &pCounter, const int lastIndex) {
    bool isOctFormat = false;

    int nextSpace = numberOfChars2SpaceOrCloseKey(strBuffer, pCounter, lastIndex);


    if(nextSpace > 0 && nextSpace < 5){
        isOctFormat = true;

        if(strBuffer.at(pCounter + nextSpace) == ' ' || strBuffer.at(pCounter + nextSpace) == ']'){

            for(size_t i = 0; i < nextSpace; i++){
                if (strBuffer.at(pCounter) < 0x30 || strBuffer.at(pCounter) > 0x37)
                    isOctFormat = false;

                pCounter++;
            }
        }
        else
            pCounter += nextSpace;

    }
    else
        pCounter += nextSpace;


    return isOctFormat;
}

bool FunctionTools::isStartHEX_format(const std::string &strBuffer, int &pCounter, const int lastIndex) {

    bool isHexFormat = false;

    if(strBuffer.at(pCounter) == '0'){
        pCounter++;

        if(pCounter < lastIndex && (strBuffer.at(pCounter) == 'x' || strBuffer.at(pCounter) == 'X')){
            int nextSpace = numberOfChars2SpaceOrCloseKey(strBuffer, pCounter, lastIndex);

            if(nextSpace < 4 && nextSpace > 1){
                isHexFormat = true;

                if(strBuffer.at(pCounter + nextSpace) == ' ' || strBuffer.at(pCounter + nextSpace) == ']'){
                    pCounter++;

                    for(size_t i = 0; i < (nextSpace - 1); i++){
                        if(strBuffer.at(pCounter) < 0x30 || (strBuffer.at(pCounter) < 0x41 && strBuffer.at(pCounter) > 0x39)
                                                            || (strBuffer.at(pCounter) > 0x46 && strBuffer.at(pCounter) < 0x61 )
                                                            || strBuffer.at(pCounter) > 0x66)
                            isHexFormat = false;

                        pCounter++;
                    }
                }
            }


        }
    }

    return isHexFormat;
}

bool FunctionTools::isHEX_format(const std::string &strBuffer, int &pCounter, const int lastIndex) {

    bool isHexFormat = false;

    int nextSpace = numberOfChars2SpaceOrCloseKey(strBuffer, pCounter, lastIndex);

    if(nextSpace < 3 && nextSpace > 0){
        isHexFormat = true;

        if(strBuffer.at(pCounter + nextSpace) == ' ' || strBuffer.at(pCounter + nextSpace) == ']'){

            for(size_t i = 0; i < nextSpace; i++){
                if(strBuffer.at(pCounter) < 0x30 || (strBuffer.at(pCounter) < 0x41 && strBuffer.at(pCounter) > 0x39)
                                                    || (strBuffer.at(pCounter) > 0x46 && strBuffer.at(pCounter) < 0x61 )
                                                    || strBuffer.at(pCounter) > 0x66)
                    isHexFormat = false;

                pCounter++;
            }
        }
        else
            pCounter += nextSpace;
    }
    else
        pCounter += nextSpace;

    return isHexFormat;
}

bool FunctionTools::isBIN_format(const std::string *strBuffer, int *pCounter, const int lastIndex) {

    bool isBinFormat = true;

    return isBinFormat;
}

std::string FunctionTools::hexStr2ByteStr(const std::string &strBuffer) {
    std::string byteStr = std::string();

    int strSizeLimit = strBuffer.size();

    int pCounter = 3;

    int intValue = 0;


    if(increasePC2NextSpaceOrLimit(&strBuffer, &pCounter, strSizeLimit)) {

        std::string intStr = strBuffer.substr(3, (pCounter - 3));

        intValue = std::stoi(intStr, 0, 16);
        byteStr.push_back(static_cast<char>(intValue));

        if(increasePC2NextChar(&strBuffer, &pCounter, strSizeLimit)) {

            while (pCounter < (strSizeLimit - 1)) {

                int pCounterB = pCounter;

                if (increasePC2NextSpaceOrLimit(&strBuffer, &pCounterB, strSizeLimit)) {
                    intStr = strBuffer.substr(pCounter, (pCounterB - pCounter));

                    intValue = std::stoi(intStr, 0, 16);
                    byteStr.push_back(static_cast<char>(intValue));

                    pCounter = pCounterB;

                    if(!increasePC2NextChar(&strBuffer, &pCounter, strSizeLimit))
                        break;

                }
                else
                    break;
            }
        }

    }

    return byteStr;
}

std::string FunctionTools::decStr2ByteStr(const std::string &strBuffer) {
    std::string byteStr = std::string();
    std::string intStr = strBuffer.substr(1, 3);

    int strSizeLimit = strBuffer.size();

    int pCounter = 4;

    int intValue = std::stoi(intStr);

    byteStr.push_back(static_cast<char>(intValue));

    if(increasePC2NextChar(&strBuffer, &pCounter, strSizeLimit)) {

        while (pCounter < (strSizeLimit - 1)) {

            int pCounterB = pCounter;

            if(increasePC2NextSpaceOrLimit(&strBuffer, &pCounterB, strSizeLimit)){

                intStr = strBuffer.substr(pCounter, (pCounterB - pCounter));

                intValue = std::stoi(intStr);
                byteStr.push_back(static_cast<char>(intValue));

                pCounter = pCounterB;

                if(!increasePC2NextChar(&strBuffer, &pCounter, strSizeLimit))
                    break;
            }
            else
                break;

        }

    }

    return byteStr;
}

std::string FunctionTools::octStr2ByteStr(const std::string &strBuffer) {
    std::string byteStr = std::string();

    int strSizeLimit = strBuffer.size();

    int pCounter = 3;

    int intValue = 0;


    if(increasePC2NextSpaceOrLimit(&strBuffer, &pCounter, strSizeLimit)) {

        std::string intStr = strBuffer.substr(3, (pCounter - 3));

        intValue = std::stoi(intStr, 0, 8);
        byteStr.push_back(static_cast<char>(intValue));

        if(increasePC2NextChar(&strBuffer, &pCounter, strSizeLimit)) {

            while (pCounter < (strSizeLimit - 1)) {

                int pCounterB = pCounter;

                if (increasePC2NextSpaceOrLimit(&strBuffer, &pCounterB, strSizeLimit)) {
                    intStr = strBuffer.substr(pCounter, (pCounterB - pCounter));

                    intValue = std::stoi(intStr, 0, 8);
                    byteStr.push_back(static_cast<char>(intValue));

                    pCounter = pCounterB;

                    if(!increasePC2NextChar(&strBuffer, &pCounter, strSizeLimit))
                        break;

                }
                else
                    break;
            }
        }

    }

    return byteStr;
}



int FunctionTools::strMatchInBuffer(const char* innerBuffer, const int length, const std::string &strReference){
    int foundIndex = -1;

    size_t i = 0, j = -1;
    while(i < length){
        if(innerBuffer[i] == '>'){
            j = i + 1;
        }
        else if(innerBuffer[i] == ':' && j != -1){
            char* subStr = new char[i - j];
            //strReference.compare()

            delete[] subStr;
        }

        i++;
    }

    /*
    size_t j = 0;
    for(size_t i = 0; i < length; i++){
        if(innerBuffer[i] == '>'){

        }
    }*/

    return foundIndex;
}




