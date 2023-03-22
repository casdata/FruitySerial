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
    //undock = false;
    //rename = false;
}

std::string TabSerialWindow::getTabName() {
    return tabName;
}

bool TabSerialWindow::isTabSelected() {
    bool preTabSelected = tabSelected;
    tabSelected = false;

    return preTabSelected;
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


    //ImGui:
     if(ImGui::BeginTabItem(tabName.c_str(), &open, ImGuiTabItemFlags_None)) {

         drawPopupTabMenu(multiTabs);

         drawTopBar();

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

             if(!serialConnection->getAutoScroll() && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                 serialConnection->setAutoScroll(true);
             else if(serialConnection->getAutoScroll() && ImGui::GetScrollY() < ImGui::GetScrollMaxY())
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

void TabSerialWindow::drawTopBar() {
    int baudItem = static_cast<int>(serialConnection->getBaudrate());

    int currentItem = baudItem;


    ImVec2 myCursorPos = ImGui::GetCursorPos();

    std::cout<<"-> "<<ImGui::GetWindowSize().x<<" "<<ImGui::GetItemRectSize().x<<std::endl;






    float tempWidth = FunctionTools::norm2HeightFloat(120);

    ImGui::SetNextItemWidth(tempWidth);

    ImGui::Combo("##baudrate_combo", &currentItem, BAUDRATE_ITEMS, IM_ARRAYSIZE(BAUDRATE_ITEMS));

    if(currentItem != baudItem)
        serialConnection->setBaudrate(static_cast<serialBaudrate>(currentItem));

    ImGui::SameLine();

    tempWidth = FunctionTools::norm2HeightFloat(18);

    int imagePadding = FunctionTools::norm2Height(1);

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_WindowBg));
    if(ImGui::ImageButton((void*)(intptr_t)settingsTexture, ImVec2(tempWidth, tempWidth), ImVec2(0,0), ImVec2(1,1), imagePadding)){

    }
    ImGui::PopStyleColor();

    ImGui::SameLine();

    float tempHeight = tempWidth;
    tempWidth = FunctionTools::norm2HeightFloat(5);

    ImGui::Image((void*)(intptr_t)separatorTexture, ImVec2(tempWidth, tempHeight), ImVec2(0,0), ImVec2(1,1));

    ImGui::SameLine();

    tempWidth = FunctionTools::norm2HeightFloat(18);

    if(serialConnection->isTimeStampEnabled())
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
    else
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_WindowBg));

    if(ImGui::ImageButton((void*)(intptr_t)timestampTexture, ImVec2(tempWidth, tempWidth), ImVec2(0,0), ImVec2(1,1), imagePadding))
        serialConnection->swapTimeStamp();

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




