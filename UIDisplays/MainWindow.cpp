//
// Created by castdata on 1/27/2023.
//

#include "MainWindow.h"

MainWindow::MainWindow() {
    int windowCount = 0;

    int imageWidth = 0;
    int imageHeight = 0;

    inputBarPos = ImVec2(0,0);
    inputBarSize = ImVec2(0,0);

    onInputTextBar = false;
    inputBarEnabled = false;
    showInputBarCaret = false;
    caretCurrentTime = 0;

    inputTextBarBuffer = "";
    iTextBarBufferPC = 0;
    caretXPos = 0;
    iBarOffsetX = 0;

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

    texturePtr = &TabSerialWindow::playTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/play.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::pauseTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/pause.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &caretTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/caret.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    TabSerialWindow::caretSize = ImVec2(imageWidth, imageHeight);

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


    if(appdata.winNotMinimized && inputBarEnabled){

        bool inside = false;

        if(ioData.mouseCursorPositionRaw.y > inputBarPos.y &&
            ioData.mouseCursorPositionRaw.y < (inputBarPos.y + inputBarSize.y)) {
            if (ioData.mouseCursorPositionRaw.x > inputBarPos.x &&
                ioData.mouseCursorPositionRaw.x < (inputBarPos.x + inputBarSize.x)) {
                inside = true;
            }
        }

        if(inside && !appdata.cursorOverInputTextBar)
            appdata.cursorOverInputTextBar = true;
        else if(!inside && appdata.cursorOverInputTextBar)
            appdata.cursorOverInputTextBar = false;

    }
    else{
        if(appdata.cursorOverInputTextBar)
            appdata.cursorOverInputTextBar = false;
    }


    if(ioData.mouseBtnLeft == DOWN){
        if(appdata.cursorOverInputTextBar) {
            onInputTextBar = true;
            showInputBarCaret = true;
            caretCurrentTime = 0;

            float clickDistance = ioData.mouseCursorPositionRaw.x - inputBarPos.x;
            clickDistance -= iBarOffsetX;
            clickDistance -= FunctionTools::norm2HeightFloat(2);                //offset fix

            iTextBarBufferPC = 0;

            std::string tString;
            ImGui::PushFont(appdata.monoFont);

            if(inputTextBarBuffer.empty())
                caretXPos =  0;
            else{
                caretXPos = ImGui::CalcTextSize(inputTextBarBuffer.c_str()).x;

                if(clickDistance > caretXPos)
                    iTextBarBufferPC = inputTextBarBuffer.length();
                else{

                    for(size_t i = 0; i < inputTextBarBuffer.size(); i++){
                        caretXPos = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, (i + 1)).c_str()).x;

                        if(clickDistance < caretXPos){

                            float caretXPos0 = caretXPos;
                            caretXPos = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, (i)).c_str()).x;

                            float diffCaretXUp = caretXPos0 - clickDistance;
                            float diffCaretXDown = clickDistance - caretXPos;


                            if(diffCaretXUp < diffCaretXDown){
                                caretXPos = caretXPos0;
                                iTextBarBufferPC = i + 1;
                            }
                            else
                                iTextBarBufferPC = i;

                            break;
                        }
                    }

                }

            }

            ImGui::PopFont();

        }
        else
            onInputTextBar = false;
    }

    if(onInputTextBar){

        bool updateCaretPos = false;
        bool postEndUpdate = false;
        bool onlyUpdateOnIncrement = false;

        if(!ioData.charBuffer.empty()){

            for(size_t i = 0; i < ioData.charBuffer.size(); i++, iTextBarBufferPC++)
                inputTextBarBuffer.insert(inputTextBarBuffer.begin() + iTextBarBufferPC, ioData.charBuffer.at(i));

            updateCaretPos = true;
            onlyUpdateOnIncrement = true;
        }

        if(ioData.keyLeft == DOWN){
            if(iTextBarBufferPC > 0) {
                iTextBarBufferPC--;
                updateCaretPos = true;
            }
        }

        if(ioData.keyRight == DOWN){
            if(iTextBarBufferPC < inputTextBarBuffer.size()){
                iTextBarBufferPC++;
                updateCaretPos = true;
            }
        }

        if(ioData.keyHome == DOWN) {
            iTextBarBufferPC = 0;
            iBarOffsetX = 0;
            updateCaretPos = true;
        }

        if(ioData.keyEnd == DOWN) {
            iTextBarBufferPC = inputTextBarBuffer.size();
            postEndUpdate = true;
            updateCaretPos = true;
        }

        if(ioData.keyBackspace == DOWN){
            if(iTextBarBufferPC > 0) {
                iTextBarBufferPC--;

                inputTextBarBuffer.erase(inputTextBarBuffer.begin() + iTextBarBufferPC);
                updateCaretPos = true;
            }
        }

        if(ioData.keyDel == DOWN){
            if(inputTextBarBuffer.size() > 0 && iTextBarBufferPC < inputTextBarBuffer.size()){
               inputTextBarBuffer.erase(inputTextBarBuffer.begin() + iTextBarBufferPC);
               updateCaretPos = true;
            }
        }


        if(updateCaretPos) {
            ImGui::PushFont(appdata.monoFont);
            caretXPos = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC).c_str()).x;
            ImGui::PopFont();

            if(postEndUpdate){
                int caretMulitplier = static_cast<int>(caretXPos / inputBarSize.x);

                iBarOffsetX = static_cast<float>(caretMulitplier * inputBarSize.x);
                iBarOffsetX *= -1;
            }

            calculateInputBarOffset(onlyUpdateOnIncrement);
        }
    }
}


void MainWindow::draw(const double &dt, AppData &appData, SerialManager *serialManager) {

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove
                                   | ImGuiWindowFlags_NoTitleBar
                                   | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Main Window", NULL, windowFlags);

        ImGuiWindowFlags wFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar;

        winSize = ImGui::GetContentRegionAvail();       //5tat leak?

        calculateSubWinSizes();

        for(auto* subWin : subWindows){
            subWin->draw(appData, serialManager);
            ImGui::SameLine(0,0);
        }

    ImGui::End();

    updateAndPrintInputBar(dt, appData);

}

MainWindow::~MainWindow() {
    if(!subWindows.empty()){
        for(auto* serialW : subWindows)
            delete serialW;

        subWindows.clear();
    }
}

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

void MainWindow::updateAndPrintInputBar(const double &dt, const AppData &appData) {

    static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove
                                            | ImGuiWindowFlags_NoTitleBar
                                            | ImGuiWindowFlags_NoCollapse;

    SerialConnection *serialPtr = nullptr;
    inputBarEnabled = false;

    for(auto* subWin : subWindows){
        if(subWin->isFocused() && subWin->anySelectedTab()){
            serialPtr = subWin->getSerialConnection();
            if(serialPtr != nullptr)
                inputBarEnabled = true;
            break;
        }
    }



    ImGui::Begin("InputBar", NULL, windowFlags);

    inputBarSize.x = winSize.x * 0.8f;
    inputBarSize.y = FunctionTools::norm2HeightFloat(20);

    inputBarPos = ImGui::GetCursorScreenPos();


    ImGui::PushFont(appData.monoFont);
    ImGui::PushID("iClipper");
    ImGui::InvisibleButton("##iCanvas", inputBarSize);
    /*
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        offset.x += ImGui::GetIO().MouseDelta.x;
        offset.y += ImGui::GetIO().MouseDelta.y;
    }
    */
    ImGui::PopID();

    if(ImGui::IsItemVisible()){


        const ImVec2 p0 = ImGui::GetItemRectMin();
        const ImVec2 p1 = ImGui::GetItemRectMax();
        static const float startSpaceX = FunctionTools::norm2HeightFloat(4);

        ImVec2 textPos = ImVec2(p0.x + iBarOffsetX + startSpaceX, p0.y);
        ImU32 tColor            = IM_COL32_WHITE;
        ImDrawList* draw_list   = ImGui::GetWindowDrawList();


        ImGui::PushClipRect(p0, p1, true);

        if(appData.uiTheme == DARK)
            tColor = DARK_INPUT_BG_COL;
        else
            tColor = LIGHT_INPUT_BG_COL;

        draw_list->AddRectFilled(p0, p1, tColor);

        if(appData.uiTheme == DARK)
            tColor = IM_COL32_WHITE;
        else
            tColor = IM_COL32_BLACK;

        draw_list->AddText(textPos, tColor, inputTextBarBuffer.c_str());


        if(onInputTextBar) {
            caretCurrentTime += dt;
            if (caretCurrentTime > 700) {
                caretCurrentTime = 0;

                showInputBarCaret = !showInputBarCaret;
            }

            if(appData.uiTheme == DARK)
                tColor = IM_COL32_WHITE;
            else
                tColor = IM_COL32_BLACK;

            if (showInputBarCaret) {
                draw_list->AddLine(ImVec2(p0.x + startSpaceX + caretXPos + iBarOffsetX, p0.y + startSpaceX),
                                   ImVec2(p0.x + startSpaceX + caretXPos + iBarOffsetX, p1.y - startSpaceX),
                                   tColor, 1);
            }
        }


        ImGui::PopClipRect();


    }

    ImGui::PopFont();

    /*
    if(appData.uiTheme == DARK)
        ImGui::PushStyleColor(ImGuiCol_ChildBg, DARK_INPUT_BG_COL);
    else
        ImGui::PushStyleColor(ImGuiCol_ChildBg, LIGHT_INPUT_BG_COL);


    if(ImGui::BeginChild("inputBar", inputBarSize, false)){
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(FunctionTools::norm2Height(4), 1));

        ImGui::PushFont(appData.monoFont);

        */
        //ImVec2 innerPos = ImGui::GetCursorPos();
        //ImGui::SetCursorPos(innerPos);


        /*
        if(!inputTextBarBuffer.empty()) {
            ImGuiListClipper iClipper;

            iClipper.Begin(1);

            while (iClipper.Step()) {
                for (int i = iClipper.DisplayStart; i < iClipper.DisplayEnd; i++) {
                    ImGui::TextUnformatted(inputTextBarBuffer.c_str());
                }
            }

            iClipper.End();
        }
        */

        //ImGui::Text(inputTextBarBuffer.c_str());

    /*
        if(onInputTextBar) {
            caretCurrentTime += dt;
            if (caretCurrentTime > 700) {
                caretCurrentTime = 0;

                showInputBarCaret = !showInputBarCaret;
            }

            if (showInputBarCaret) {
                float tempWidth = FunctionTools::norm2HeightFloat(2);
                float tempHeight = FunctionTools::norm2HeightFloat(20);


                ImGui::SetCursorPos(ImVec2(caretXPos,0));

                ImGui::Image((void *) (intptr_t) caretTexture, ImVec2(tempWidth, tempHeight), ImVec2(0, 0),
                             ImVec2(1, 1));
            }
        }


        ImGui::PopFont();

        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    */

    //ImGui::PopStyleColor();

    ImGui::SameLine();

    static char buf[12] = "";
    ImGui::SetNextItemWidth(120);
    ImGui::InputText("##inputBar", buf, sizeof(buf));

    ImGui::SameLine();

    ImGui::Button("Send");


    ImGui::End();

}

void MainWindow::calculateInputBarOffset(bool onlyIncrement) {


    if(onlyIncrement){
        if ((caretXPos + iBarOffsetX) > (inputBarSize.x * 0.95f))
            iBarOffsetX -= (inputBarSize.x * 0.1f);
    }
    else {
        if ((caretXPos + iBarOffsetX) > (inputBarSize.x * 0.95f))
            iBarOffsetX -= (inputBarSize.x * 0.1f);
        else if ((caretXPos + iBarOffsetX) < (inputBarSize.x * 0.15f)) {
            iBarOffsetX += (inputBarSize.x * 0.1f);

            if (iBarOffsetX > 0)
                iBarOffsetX = 0;
        }
    }
}




