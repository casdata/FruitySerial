//
// Created by castdata on 1/27/2023.
//

#include "MainWindow.h"

MainWindow::MainWindow() {
    int windowCount = 0;

    int imageWidth = 0;
    int imageHeight = 0;

    GLuint *texturePtr = &TabSerialWindow::timestampTexture;

    bool ret = FunctionTools::loadTextureFromFile("../Assets/timestamp.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    TabSerialWindow::textureSize = ImVec2(imageWidth, imageHeight);

    texturePtr = &TabSerialWindow::settingsTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/comSettings.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::separatorTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/separator.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    auto* subWin = new SubWindow(windowCount);
    subWindows.push_back(subWin);
    subWindows.back()->setWidthPercentage(1.0f);

    resizingWindowIndex = -1;
}



void MainWindow::update(AppData &appdata, const IOData &ioData, SerialManager *serialManager) {

    checkAndSplitWindow(appdata.windowCount);

    if(appdata.appState == IDLE) {
        checkClickAndFocus(ioData, appdata.appState);
        checkAndResizeSubWindows(appdata.cursorOverSubWinBorder, ioData);

        for(auto* subWin : subWindows)
            subWin->update(ioData, serialManager);
    }

}


void MainWindow::draw(AppData &appData, SerialManager *serialManager) {

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove
                                   | ImGuiWindowFlags_NoTitleBar
                                   | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Main Window", NULL, windowFlags);

        ImGuiWindowFlags wFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar;

        winSize = ImGui::GetContentRegionAvail();       //5tat leak?

        //std::cout<<"winSize: "<<winSize.x<<" "<<winSize.y<<std::endl;

        calculateSubWinSizes();

        for(auto* subWin : subWindows){
            subWin->draw(appData, serialManager);
            ImGui::SameLine(0,0);
        }

    ImGui::End();


}

MainWindow::~MainWindow() {
    if(!subWindows.empty()){
        for(auto* serialW : subWindows)
            delete serialW;

        subWindows.clear();
    }
}

/*
void MainWindow::addConnection(AppData &appData) {

    std::string tabName;
    tabName.assign("COM");
    tabName.append(std::to_string(appData.connectionCount));

    appData.connectionCount++;

    auto* subWin = subWindows.at(getFocusedSubWinIndex());
    subWin->addTabPortConnection(tabName);

}
*/

int MainWindow::getFocusedSubWinIndex() {

    int focusedWindow = 0;

    if(!subWindows.empty()){
        for(size_t i = 0; i < subWindows.size(); i++){
            if(subWindows.at(i)->isFocused()){
                focusedWindow = i;
                break;
            }
        }
    }

    return focusedWindow;
}

void MainWindow::calculateSubWinSizes() {

    for(auto* subWin : subWindows)
        subWin->setWinSize(winSize.x * subWin->getWidthPercentage(), winSize.y);

}

void MainWindow::checkAndSplitWindow(int &windowCount) {

    int winIndex = -1;

    for(size_t i = 0; i < subWindows.size(); i++){
        if(subWindows.at(i)->isWindowSplitting()){
            winIndex = i;
            break;
        }
    }

    if(winIndex > -1)
        newSubWindow(windowCount, winIndex);

}

void MainWindow::newSubWindow(int &windowCount, const int &subWindowIndex) {

    for(auto* sWin : subWindows)                                                                            //Desfocus all sub windows
        sWin->setFocused(false);

    auto* subWin = new SubWindow(windowCount);
    subWindows.insert(subWindows.begin() + subWindowIndex + 1, subWin);

    float percentage = 1/static_cast<float>(subWindows.size());


    for(auto* sWin : subWindows)
        sWin->setWidthPercentage(percentage);

}

void MainWindow::checkClickAndFocus(const IOData &ioData, const AppState &appState) {

    if((ioData.mouseBtnLeft == DOWN || ioData.mouseBtnRight == DOWN)) {

        for (size_t i = 0; i < subWindows.size(); i++) {
            if(subWindows.at(i)->isClickOverWin(ioData.mouseCursorPosition)){
                subWindows.at(i)->setFocused(true);

                for(size_t j = 0; j < subWindows.size(); j++){
                    if(j != i && subWindows.at(j)->isFocused()){
                        subWindows.at(j)->setFocused(false);
                        break;
                    }
                }
                break;
            }
        }

    }

}

void MainWindow::checkAndResizeSubWindows(bool &cursorOverWinBorder, const IOData &ioData) {

    static int min30PercentLimit = 0;
    static float referencePointX = 0;
    static float leftPercentage = 0;
    static float rightPercentage = 0;
    static float rightLeftLimitPosX = 0;

    if(resizingWindowIndex > -1){
        if(ioData.mouseBtnLeft == UP){

            resizingWindowIndex = -1;
            cursorOverWinBorder = false;
        }
        else if(ioData.mouseBtnLeft == ON){

            float diffPoint = referencePointX - ioData.mouseCursorPosition.x;

            if(diffPoint < 0)
                diffPoint *= -1;

            if(diffPoint > 5){

                if(ioData.mouseCursorPosition.x <= referencePointX) {                                                   //left window shrinks
                    diffPoint = ioData.mouseCursorPosition.x - subWindows.at(resizingWindowIndex)->getWinPos().x;
                    diffPoint /= winSize.x;

                    if(diffPoint < 0.25f)
                        diffPoint = 0.25f;

                    subWindows.at(resizingWindowIndex)->setWidthPercentage(diffPoint);


                    diffPoint = leftPercentage - diffPoint;

                    subWindows.at(resizingWindowIndex + 1)->setWidthPercentage((rightPercentage + diffPoint));

                }
                else{                                                                                                   //right window shrinks

                    diffPoint = rightLeftLimitPosX - ioData.mouseCursorPosition.x;
                    diffPoint /= winSize.x;

                    if(diffPoint < 0.25f)
                        diffPoint = 0.25f;


                    subWindows.at(resizingWindowIndex + 1)->setWidthPercentage(diffPoint);

                    diffPoint = rightPercentage - diffPoint;

                    subWindows.at(resizingWindowIndex)->setWidthPercentage(leftPercentage + diffPoint);

                }


            }
            else {
                subWindows.at(resizingWindowIndex)->setWidthPercentage(leftPercentage);
                subWindows.at(resizingWindowIndex + 1)->setWidthPercentage(rightPercentage);
            }

        }


    }
    else{

        if(ioData.mouseBtnLeft == DOWN) {

            for(size_t i = 1; i < subWindows.size(); i++){
                if(subWindows.at(i)->isCursorOverBorder(ioData.mouseCursorPosition)){
                    resizingWindowIndex = (i - 1);
                    min30PercentLimit = static_cast<int>(winSize.x * 0.3f);
                    referencePointX = ioData.mouseCursorPosition.x;
                    leftPercentage = subWindows.at(resizingWindowIndex)->getWidthPercentage();
                    rightPercentage = subWindows.at(i)->getWidthPercentage();
                    rightLeftLimitPosX = subWindows.at(i)->getWinPos().x + subWindows.at(i)->getWinSize().x;
                    break;
                }
            }

        }
        else if(ioData.mouseBtnLeft == OFF){

            bool noCursorOver = true;

            for(size_t i = 1; i < subWindows.size(); i++){
                if(subWindows.at(i)->isCursorOverBorder(ioData.mouseCursorPosition)){
                    noCursorOver = false;
                    cursorOverWinBorder = true;
                    break;
                }
            }

            if(noCursorOver && cursorOverWinBorder)
                cursorOverWinBorder = false;

        }
    }

}




