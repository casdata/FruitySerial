//
// Created by castdata on 1/25/2023.
//

#include "SubWindow.h"

SubWindow::SubWindow(int &windowCount){
    windowName.assign("Win");
    windowName.append(std::to_string(windowCount));
    windowCount++;

    winSize.x = 0;
    winSize.y = 0;

    focused = true;

}

void SubWindow::addTabPortConnection(SerialPortData *serialPortData, SerialConnection *serialConnection) {

    auto* tabSerialWin = new TabSerialWindow(serialPortData, serialConnection);
    tabSerialWindows.push_back(tabSerialWin);

}


void SubWindow::update(const IOData &ioData, SerialManager *serialManager) {

    check4ClosedTabsRequest(serialManager);

}


void SubWindow::draw(AppData &appData, SerialManager *serialManager) {

    ImGuiWindowFlags wFlags = ImGuiWindowFlags_None;//ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar;

    ImGui::BeginChild(windowName.c_str(), winSize, true, wFlags);


        setWinPos(appData.windowPosX, appData.windowPosY);

        std::string tabIdStr;
        tabIdStr.assign(windowName);
        tabIdStr.append("TabBar");

        ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;

        ImGui::BeginTabBar(tabIdStr.c_str(), tabBarFlags);

            if(focused && ImGui::TabItemButton(" + ", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
                serialManager->updatePortList();
                ImGui::OpenPopup("addPopup");
            }

            if(ImGui::BeginPopup("addPopup")){


                if(serialManager->portsReady2Use()) {

                    if (serialManager->anyPortAvailable()) {

                        bool selectIt = false;
                        int baudrateIndex = 4;
                        int textEncoding = 5;

                        size_t baudIndex = 0;
                        size_t textEnIndex = 0;
                        for (const auto& port: serialManager->getAvailablePortList()) {
                            if(ImGui::BeginMenu(port.c_str())){

                                if(ImGui::Selectable("Ok"))
                                    selectIt = true;

                                ImGui::Spacing();

                                if(ImGui::BeginMenu("Baudrate")){
                                    baudIndex = 0;
                                    for(auto baudItem : BAUDRATE_ITEMS){
                                        if(ImGui::BeginMenu(baudItem)){

                                            if(ImGui::Selectable("Ok")) {
                                                baudrateIndex = baudIndex;
                                                selectIt = true;
                                            }

                                            ImGui::Spacing();

                                            if(ImGui::BeginMenu("Text Encoding")){
                                                textEnIndex = 0;
                                                for(auto textEnco : TXT_ENCODING_ITEMS){
                                                    if(ImGui::Selectable(textEnco)){
                                                        baudrateIndex = baudIndex;
                                                        textEncoding = textEnIndex;
                                                        selectIt = true;
                                                    }
                                                    ImGui::Spacing();
                                                    textEnIndex++;
                                                }

                                                ImGui::EndMenu();
                                            }

                                            ImGui::EndMenu();
                                        }
                                        ImGui::Spacing();
                                        baudIndex++;
                                    }

                                    ImGui::EndMenu();
                                }

                                ImGui::EndMenu();

                                if(selectIt){
                                    std::cout<<"-> "<<baudrateIndex<<std::endl;
                                    SerialPortData *serialPtr = serialManager->getSerialPortDataByPortName(port);
                                    if(serialPtr != nullptr) {
                                        SerialConnection *serialConnection = serialManager->newSerialConnection(port);
                                        serialConnection->setBaudrate(static_cast<serialBaudrate>(baudrateIndex));
                                        serialConnection->setTextEnconding(static_cast<TextEncoding>(textEncoding));
                                        addTabPortConnection(serialPtr, serialConnection);

                                    }

                                    break;
                                }

                                /*
                                SerialPortData *serialPtr = serialManager->getSerialPortDataByPortName(port);
                                if(serialPtr != nullptr) {
                                    SerialConnection *serialConnection = serialManager->newSerialConnection(port);
                                    addTabPortConnection(serialPtr, serialConnection);
                                }

                                break;
                                */
                            }



                            /*
                            if (ImGui::Selectable(port.c_str())) {
                                SerialPortData *serialPtr = serialManager->getSerialPortDataByPortName(port);
                                if(serialPtr != nullptr) {
                                    SerialConnection *serialConnection = serialManager->newSerialConnection(port);
                                    addTabPortConnection(serialPtr, serialConnection);
                                }

                                break;
                            }
                            */
                            ImGui::Spacing();
                        }
                    }
                }
                else {
                    ImGui::Text("No ports");
                    ImGui::Spacing();
                }

                ImGui::Separator();
                ImGui::Spacing();

                if(ImGui::Selectable("Network")){

                }

                ImGui::EndPopup();
            }

            bool multiTabs = tabSerialWindows.size() > 1;

            for(auto* tabSerialWin : tabSerialWindows)
                tabSerialWin->draw(multiTabs, appData.monoFont, appData.uiTheme);

        ImGui::EndTabBar();

    ImGui::EndChild();

}


SubWindow::~SubWindow() {

    if(!tabSerialWindows.empty()){
        for(auto* tabSerialWin : tabSerialWindows) {
            tabSerialWin->clearDataBuffer();
            delete tabSerialWin;
        }

        tabSerialWindows.clear();
    }


    windowName.clear();
}

std::string SubWindow::getWinName() {
    return windowName;
}

void SubWindow::setWinSize(const float &w, const float &h) {
    winSize.x = w;
    winSize.y = h;
}


ImVec2 SubWindow::getWinSize() {
    return winSize;
}

ImVec2 SubWindow::getWinPos() {
    return winPos;
}

float SubWindow::getWidthPercentage() {
    return widthPercentage;
}

void SubWindow::setWidthPercentage(const float &percentage) {
    widthPercentage = percentage;
}

void SubWindow::setFocused(bool focus) {
    focused = focus;
}

bool SubWindow::isFocused() {
    return focused;
}

bool SubWindow::isWindowSplitting() {
    splitTabIndex = -1;

    for(size_t i = 0; i < tabSerialWindows.size(); i++){
        if(tabSerialWindows.at(i)->isTabSplitting()){
            splitTabIndex = i;
            break;
        }
    }

    return (splitTabIndex > -1);
}


void SubWindow::setWinPos(const int &x, const int &y) {

    winPos.x = FunctionTools::getRealValue(ImGui::GetWindowPos().x, x);
    winPos.y = FunctionTools::getRealValue(ImGui::GetWindowPos().y, y);

}

bool SubWindow::isClickOverWin(const ImVec2 &mousePos) {

    bool toReturn  = false;

    if(static_cast<int>(mousePos.x) >= winPos.x && static_cast<int>(mousePos.x) < (winPos.x + winSize.x)){
        if(static_cast<int>(mousePos.y) >= winPos.y && static_cast<int>(mousePos.y) < (winPos.y + winSize.y)){
            toReturn = true;
        }
    }

    return toReturn;
}

bool SubWindow::isCursorOverBorder(const ImVec2 &mousePos) {


    if(mousePos.y >= winPos.y && mousePos.y < (winPos.y + winSize.y)){
        if(mousePos.x >= winPos.x && mousePos.x < (winPos.x + 8)){
            return true;
        }
    }

    return false;
}

void SubWindow::check4ClosedTabsRequest(SerialManager *serialManager) {

    std::vector<TabSerialWindow*>::iterator it;

    for(it = tabSerialWindows.begin(); it != tabSerialWindows.end(); it++){
        if((*it)->isTabClosing()){
            (*it)->freePort();
            delete *it;
            tabSerialWindows.erase(it);
            break;
        }
    }

}

bool SubWindow::anySelectedTab() {

    if(tabSerialWindows.empty())
        return false;
    else{
        for(size_t i = 0; i < tabSerialWindows.size(); i++){
            if(tabSerialWindows.at(i)->isTabSelected()){
                focusedIndex = i;
                return true;
            }
        }

        return false;
    }

}

SerialConnection *SubWindow::getSerialConnection() {
    return tabSerialWindows.at(focusedIndex)->getSerialConnectionPtr();
}

