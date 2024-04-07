//
// Created by castdata on 1/28/2023.
//

#include "TabSerialWindow.h"

TabSerialWindow::TabSerialWindow(SerialPortData *serialPortData, SerialConnection *serialConnection) {

    tabName.assign(serialPortData->portInfo.port);

    tabSerialPortData = serialPortData;
    tabSerialPortData->used = true;

    this->serialConnection = serialConnection;

    splitRight = false;
    open = true;
    serialSettings = false;
    //undock = false;
    //rename = false;
}

std::string TabSerialWindow::getTabName() {
    return tabName;
}

bool TabSerialWindow::isTabSelected() {
    return tabSelected;
    /*
    bool preTabSelected = tabSelected;
    tabSelected = false;

    return preTabSelected;
    */
}

bool TabSerialWindow::isTabClosing(){
    return !open;
}

bool TabSerialWindow::isTabSplitting(){
    bool postSplit = splitRight;
    splitRight = false;

    return postSplit;
}

bool TabSerialWindow::isUndocking(){
    bool postUndock = undock;
    undock = false;

    return postUndock;
}

bool TabSerialWindow::isRenaming(){
    bool postRename = rename;
    rename = false;

    return postRename;
}

void TabSerialWindow::freePort() {
    tabSerialPortData->used = false;
    tabSerialPortData->updateIt = true;
}

void TabSerialWindow::draw(bool multiTabs, ImFont* monoFont, const UI_Theme& uiTheme) {

    static const float offset30 = FunctionTools::norm2HeightFloat(30);

     if(ImGui::BeginTabItem(tabName.c_str(), &open, ImGuiTabItemFlags_None)) {

         drawPopupTabMenu(multiTabs);

         drawTopBar(uiTheme);

         std::string childID;
         childID.append(tabName);
         childID.append("ScrollingArea");

         if(uiTheme == DARK)
             ImGui::PushStyleColor(ImGuiCol_ChildBg, DARK_WINDOW_BG_COL);
         else
             ImGui::PushStyleColor(ImGuiCol_ChildBg, LIGHT_WINDOW_BG_COL);

         //const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
         if(ImGui::BeginChild(childID.c_str(), ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar)){
             ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(FunctionTools::norm2Height(4), 1));

             ImGui::PushFont(monoFont);

             serialConnection->printLines(uiTheme);

             ImGui::PopFont();

             if(serialConnection->getScroll2Bottom())
                 ImGui::SetScrollHereY(1.0f);


             if(!serialConnection->getAutoScroll() && (ImGui::GetScrollY() + offset30) >= ImGui::GetScrollMaxY())
                 serialConnection->setAutoScroll(true);
             else if(serialConnection->getAutoScroll() && (ImGui::GetScrollY() + offset30) < ImGui::GetScrollMaxY())
                 serialConnection->setAutoScroll(false);


             ImGui::PopStyleVar();
         }
         ImGui::EndChild();

         ImGui::PopStyleColor();

         ImGui::EndTabItem();

         tabSelected = true;
     }
     else
         tabSelected = false;



     if(serialSettings){

         std::string nameId;

         nameId.assign("Settings: ");
         nameId.append(tabSerialPortData->portInfo.port);

         ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;

         //ImGui::SetScrollX()
         static float wSizeX = FunctionTools::norm2HeightFloat(330);
         static float wSizeY = FunctionTools::norm2HeightFloat(266);
         ImGui::SetNextWindowSize(ImVec2(wSizeX, wSizeY));

         ImVec2 center = ImGui::GetMainViewport()->GetCenter();
         ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f,0.5f));

         ImGui::Begin(nameId.c_str(), &serialSettings, windowFlags);


            nameId.assign("table:");
            nameId.append(tabSerialPortData->portInfo.port);

            if(ImGui::BeginTable(nameId.c_str(), 2, ImGuiTableFlags_SizingFixedFit)){

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Device:");
                ImGui::TableSetColumnIndex(1);

                static float wrap_width =  FunctionTools::norm2HeightFloat(240.0f);
                static float wrap_offset = FunctionTools::norm2HeightFloat(10);
                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
                ImVec2 marker_max = ImVec2(pos.x + wrap_width + wrap_offset, pos.y + ImGui::GetTextLineHeight());
                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
                    ImGui::Text("%s", tabSerialPortData->portInfo.hardware_id.c_str());
                ImGui::PopTextWrapPos();



                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Port:");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", tabSerialPortData->portInfo.port.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("About:");
                ImGui::TableSetColumnIndex(1);

                pos = ImGui::GetCursorScreenPos();
                marker_min = ImVec2(pos.x + wrap_width, pos.y);
                marker_max = ImVec2(pos.x + wrap_width + wrap_offset, pos.y + ImGui::GetTextLineHeight());
                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
                    ImGui::Text("%s", tabSerialPortData->portInfo.description.c_str());
                ImGui::PopTextWrapPos();


                ImGui::NewLine();
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Byte size:");
                ImGui::TableSetColumnIndex(1);


                nameId.assign("##bitSize:");
                nameId.append(tabSerialPortData->portInfo.port);

                int preItemIndex = static_cast<int>(serialConnection->getByteSize());
                int itemIndex = preItemIndex - 5;

                ImGui::SetNextItemWidth(ImGui::GetColumnWidth(1));
                ImGui::Combo(nameId.c_str(), &itemIndex, BYTE_SIZE_ITEMS, IM_ARRAYSIZE(BYTE_SIZE_ITEMS));

                itemIndex += 5;

                if(itemIndex != preItemIndex)
                    serialConnection->setBytesize(static_cast<serialByteSize>(itemIndex));



                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Parity:");
                ImGui::TableSetColumnIndex(1);

                preItemIndex = static_cast<int>(serialConnection->getParity());
                itemIndex = preItemIndex;

                nameId.assign("##parity:");
                nameId.append(tabSerialPortData->portInfo.port);

                ImGui::SetNextItemWidth(ImGui::GetColumnWidth(1));
                ImGui::Combo(nameId.c_str(), &itemIndex, PARITY_ITEMS, IM_ARRAYSIZE(PARITY_ITEMS));

                if(itemIndex != preItemIndex)
                    serialConnection->setParity(static_cast<serialParity>(itemIndex));



                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Stop bits:");
                ImGui::TableSetColumnIndex(1);

                preItemIndex = static_cast<int>(serialConnection->getStopbits());
                itemIndex = preItemIndex;

                nameId.assign("##stopb:");
                nameId.append(tabSerialPortData->portInfo.port);

                ImGui::SetNextItemWidth(ImGui::GetColumnWidth(1));
                ImGui::Combo(nameId.c_str(), &itemIndex, STOP_BITS_ITEMS, IM_ARRAYSIZE(STOP_BITS_ITEMS));

                if(itemIndex != preItemIndex)
                    serialConnection->setStopbits(static_cast<serialStopbits>(itemIndex));


                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Flow control:");
                ImGui::TableSetColumnIndex(1);

                preItemIndex = static_cast<int>(serialConnection->getFlowcontrol());
                itemIndex = preItemIndex;

                nameId.assign("##flowc:");
                nameId.append(tabSerialPortData->portInfo.port);

                ImGui::SetNextItemWidth(ImGui::GetColumnWidth(1));
                ImGui::Combo(nameId.c_str(), &itemIndex, FLOW_CONTROL_ITEMS, IM_ARRAYSIZE(FLOW_CONTROL_ITEMS));

                if(itemIndex != preItemIndex)
                    serialConnection->setFlowcontrol(static_cast<serialFlowcontrol>(itemIndex));


                ImGui::EndTable();
            }

            ImGui::NewLine();

            ImVec2 curPos = ImGui::GetCursorPos();

            ImVec2 textSizes = ImGui::CalcTextSize("Restore defaults");

            ImGui::SetCursorPos(ImVec2(wSizeX - textSizes.x - FunctionTools::norm2HeightFloat(16), curPos.y));

            if(ImGui::Button("Restore defaults")){
                serialConnection->setStopbits(stopbits_one);
                serialConnection->setBytesize(eigthbits);
                serialConnection->setParity(parity_none);
                serialConnection->setFlowcontrol(flowcontrol_none);
            }


         ImGui::End();


     }


}


void TabSerialWindow::drawPopupTabMenu(bool multiTabs) {
    if(ImGui::BeginPopupContextItem()){

        ImGui::Spacing();

        if(ImGui::Selectable("Close"))
            open = false;

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if(multiTabs) {

            if (ImGui::Selectable("Split Right"))
                splitRight = true;

            ImGui::Spacing();
            ImGui::Spacing();

            if (ImGui::Selectable("Undock"))
                undock = true;

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }

        if(ImGui::Selectable("Rename Tab"))
            rename = true;

        ImGui::Spacing();

        ImGui::EndPopup();
    }
}

void TabSerialWindow::drawTopBar(const UI_Theme& uiTheme) {
    int comboItem = static_cast<int>(serialConnection->getBaudrate());

    int currentItem = comboItem;

    ImVec2 myCursorPos = ImGui::GetCursorPos();

    //Setting BTN
    float tempWidth = FunctionTools::norm2HeightFloat(18);

    float tempX = ImGui::GetWindowSize().x - (myCursorPos.x * 2) - tempWidth;
    float tempY = myCursorPos.y;

    ImGui::SetCursorPosX(tempX);

    int imagePadding = FunctionTools::norm2Height(1);

    if(serialSettings)
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
    else
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_WindowBg));

    if(ImGui::ImageButton((void*)(intptr_t)settingsTexture, ImVec2(tempWidth, tempWidth), ImVec2(0,0), ImVec2(1,1), imagePadding))
        serialSettings = true;


    ImGui::PopStyleColor();

    //Baudrate combo
    tempWidth = FunctionTools::norm2HeightFloat(120);
    tempX -= (tempWidth + myCursorPos.x);

    ImGui::SetCursorPos(ImVec2(tempX, tempY));

    ImGui::SetNextItemWidth(tempWidth);

    std::string nameId;
    nameId.assign("##baudr_c:");
    nameId.append(tabName);

    ImGui::Combo(nameId.c_str(), &currentItem, BAUDRATE_ITEMS, IM_ARRAYSIZE(BAUDRATE_ITEMS));

    if(currentItem != comboItem)
        serialConnection->setBaudrate(static_cast<serialBaudrate>(currentItem));

    //Separator
    tempWidth = FunctionTools::norm2HeightFloat(6);
    float tempHeight = FunctionTools::norm2HeightFloat(18);
    tempX -= (tempWidth + myCursorPos.x);

    ImGui::SetCursorPos(ImVec2(tempX, tempY));
    ImGui::Image((void*)(intptr_t)separatorTexture, ImVec2(tempWidth, tempHeight), ImVec2(0,0), ImVec2(1,1));


    //Input EOL
    comboItem = static_cast<int>(serialConnection->getInputEol());
    currentItem = comboItem;

    tempWidth = tempHeight;
    tempX -= (tempWidth + myCursorPos.x);
    ImGui::SetNextItemWidth(tempWidth);
    ImGui::SetCursorPos(ImVec2(tempX, tempY));

    GLuint tempTexture = lightBtn0RedTexture;

    if(uiTheme == DARK){
        switch(currentItem){
            case 1:
                tempTexture = darkBtn1RedTexture;
                break;
            case 2:
                tempTexture = darkBtn2RedTexture;
                break;
            case 3:
                tempTexture = darkBtn3RedTexture;
                break;
            default:
                tempTexture = darkBtn0RedTexture;
        }
    }
    else{
        switch(currentItem){
            case 1:
                tempTexture = lightBtn1RedTexture;
                break;
            case 2:
                tempTexture = lightBtn2RedTexture;
                break;
            case 3:
                tempTexture = lightBtn3RedTexture;
                break;
            default:
                tempTexture = lightBtn0RedTexture;
        }
    }

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_WindowBg));
    if(ImGui::ImageButton((void*)(intptr_t)tempTexture, ImVec2(tempWidth, tempWidth), ImVec2(0,0), ImVec2(1,1), imagePadding))
        ImGui::OpenPopup("eolInputPopup");

    ImGui::PopStyleColor();

    if(ImGui::BeginPopup("eolInputPopup")){
        ImGui::Text("EOL");
        ImGui::Separator();

        bool selected = false;

        for(size_t i = 0; i < IM_ARRAYSIZE(EOL_ITEMS); i++){
            selected = (i == comboItem);
            if(ImGui::Selectable(EOL_ITEMS[i], selected))
                currentItem = i;
        }

        if(currentItem != comboItem)
            serialConnection->setInputEol(static_cast<serialEndOfLine>(currentItem));

        ImGui::EndPopup();
    }


    //Encoding buttons
    int arrowPressed = -1;

    tempWidth = tempHeight;
    tempX -= (tempWidth + (myCursorPos.x));
    ImGui::SetNextItemWidth(tempWidth);
    ImGui::SetCursorPos(ImVec2(tempX, tempY));

    if(ImGui::ArrowButton("encoRight", ImGuiDir_Right))
        arrowPressed = 1;

    float tempLittleSpace = FunctionTools::norm2HeightFloat(6);
    tempX -= (tempWidth + tempLittleSpace);
    ImGui::SetNextItemWidth(tempWidth);
    ImGui::SetCursorPos(ImVec2(tempX, tempY));

    if(ImGui::ArrowButton("encoLeft", ImGuiDir_Left))
        arrowPressed = 2;

    if(arrowPressed == 1){                                                                                              //Right

        int encodingIndex = static_cast<int>(serialConnection->getTextEncoding());

        if(++encodingIndex < IM_ARRAYSIZE(TXT_ENCODING_ITEMS))
            serialConnection->setTextEnconding(static_cast<TextEncoding>(encodingIndex));
    }
    else if(arrowPressed == 2){                                                                                         //Left
        int encodingIndex = static_cast<int>(serialConnection->getTextEncoding());

        if(--encodingIndex > -1)
            serialConnection->setTextEnconding(static_cast<TextEncoding>(encodingIndex));
    }


    //Encoding format
    comboItem = static_cast<int>(serialConnection->getTextEncoding());

    currentItem = comboItem;

    ImVec2 txtSize = ImGui::CalcTextSize(TXT_ENCODING_ITEMS[comboItem]);

    tempWidth = txtSize.x;
    tempX -= (tempWidth + tempLittleSpace * 2);

    ImGui::SetCursorPos(ImVec2(tempX, tempY));
    ImGui::SetNextItemWidth(tempWidth);

    ImVec2 realCursorPos = ImGui::GetCursorScreenPos();

    if(uiTheme == DARK) {
        ImGui::PushStyleColor(ImGuiCol_Button, DARK_COMBO_BG_COL);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, DARK_COMBO_BG_OVER_COL);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, DARK_COMBO_BG_PRESS_COL);
    }
    else{
        ImGui::PushStyleColor(ImGuiCol_Button, LIGHT_COMBO_BG_COL);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, LIGHT_COMBO_BG_OVER_COL);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, LIGHT_COMBO_BG_PRESS_COL);
    }

    if(ImGui::Button(TXT_ENCODING_ITEMS[comboItem]))
        ImGui::OpenPopup("encodingPopup");

    ImGui::PopStyleColor(3);

    realCursorPos.y += ImGui::GetFrameHeight();
    ImGui::SetNextWindowPos(realCursorPos);

    if(ImGui::BeginPopup("encodingPopup")){
        ImGui::Text("Text Encoding");
        ImGui::Separator();

        bool selected = false;

        for(size_t i = 0; i < IM_ARRAYSIZE(TXT_ENCODING_ITEMS); i++){
            selected = (i == comboItem);
            if(ImGui::Selectable(TXT_ENCODING_ITEMS[i], selected))
                currentItem = i;
        }

        if(currentItem != comboItem)
            serialConnection->setTextEnconding(static_cast<TextEncoding>(currentItem));

        ImGui::EndPopup();
    }


    //Play/Pause btn
    tempWidth = FunctionTools::norm2HeightFloat(18);
    tempX = (myCursorPos.x * 2);

    ImGui::SetCursorPos(ImVec2(tempX, tempY));

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_WindowBg));

    GLuint *texturePtr = &playTexture;

    if(serialConnection->isConnectionOpen())
        texturePtr = &pauseTexture;

    if(ImGui::ImageButton((void*)(intptr_t)*texturePtr, ImVec2(tempWidth, tempWidth), ImVec2(0,0), ImVec2(1,1), imagePadding)){
        //serialConnection->swapListening();

        if(serialConnection->isConnectionOpen())
            serialConnection->closeConnection();
        else
            serialConnection->openConnection();

    }

    ImGui::PopStyleColor();


    //Separator
    tempX += (tempWidth + myCursorPos.x);
    tempHeight = tempWidth;
    tempWidth = FunctionTools::norm2HeightFloat(5);

    ImGui::SetCursorPos(ImVec2(tempX, tempY));

    ImGui::Image((void*)(intptr_t)separatorTexture, ImVec2(tempWidth, tempHeight), ImVec2(0,0), ImVec2(1,1));


    tempX += (tempWidth + myCursorPos.x);
    tempWidth = tempHeight;

    ImGui::SetCursorPos(ImVec2(tempX, tempY));

    if(serialConnection->isTimeStampEnabled())
        texturePtr = &timestampTexture;
    else{
        if(uiTheme == DARK)
            texturePtr = &timestampDisDarkTexture;
        else
            texturePtr = &timestampDisLightTexture;
    }

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_WindowBg));

    if(ImGui::ImageButton((void*)(intptr_t)*texturePtr, ImVec2(tempWidth, tempWidth), ImVec2(0,0), ImVec2(1,1), imagePadding))
        serialConnection->swapTimeStamp();

    ImGui::PopStyleColor();

    tempX += (tempWidth + myCursorPos.x);

    ImGui::SetCursorPos(ImVec2(tempX, tempY));


    if(serialConnection->areFruitsEnabled())
        texturePtr = &fruitsTexture;
    else{
        if(uiTheme == DARK)
            texturePtr = &fruitsDisDarkTexture;
        else
            texturePtr = &fruitsDisLightTexture;
    }

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_WindowBg));


    if(ImGui::ImageButton((void*)(intptr_t)*texturePtr, ImVec2(tempWidth, tempWidth), ImVec2(0,0), ImVec2(1,1), imagePadding))
        serialConnection->swapFruits();


    ImGui::PopStyleColor();


}

void TabSerialWindow::add2DataBuffer(const std::string &dataText) {
    for(size_t i = 0; i < dataText.length(); i++){
        char character = dataText.at(i);

        if(character == (char)10){                              //NL new line

        }
    }
}

void TabSerialWindow::clearDataBuffer() {
    for(size_t i = 0; i < dataBuffer.size(); i++)
        dataBuffer[i].clear();

    dataBuffer.clear();
}

SerialConnection *TabSerialWindow::getSerialConnectionPtr() {
    return serialConnection;
}




