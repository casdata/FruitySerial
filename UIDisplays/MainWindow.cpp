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

    selectedInputText = false;
    onInputTextBar = false;
    inputBarEnabled = false;
    showInputBarCaret = false;
    caretCurrentTime = 0;

    inputTextBarBuffer = "";
    iTextBarBufferPC = 0;
    iTextBarBufferPC2 = 0;
    caretXPos = 0;
    iBarOffsetX = 0;

    selectedInputP0_x = 0;
    selectedInputP1_x = 0;

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


    auto* subWin = new SubWindow(windowCount);
    subWindows.push_back(subWin);
    subWindows.back()->setWidthPercentage(1.0f);

    resizingWindowIndex = -1;

    undoRedo = new UndoRedo;
}



void MainWindow::update(const double &dt, AppData &appdata, const IOData &ioData, SerialManager *serialManager) {

    checkAndSplitWindow(appdata.windowCount);

    if(appdata.appState == IDLE) {
        checkClickAndFocus(ioData, appdata.appState);
        checkAndResizeSubWindows(appdata.cursorOverSubWinBorder, ioData);

        for(auto* subWin : subWindows)
            subWin->update(ioData, serialManager);
    }


    checkMouseInsideInputTextBar(appdata, ioData);

    checkInputTextBarIO(dt, appdata, ioData);


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

    delete undoRedo;

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
    ImGui::PopID();

    if(ImGui::IsItemVisible()) {


        const ImVec2 p0 = ImGui::GetItemRectMin();
        const ImVec2 p1 = ImGui::GetItemRectMax();
        static const float startSpaceX = FunctionTools::norm2HeightFloat(4);

        ImVec2 textPos = ImVec2(p0.x + iBarOffsetX + startSpaceX, p0.y);
        ImU32 tColor = IM_COL32_WHITE;
        ImDrawList *draw_list = ImGui::GetWindowDrawList();


        ImGui::PushClipRect(p0, p1, true);

        if (appData.uiTheme == DARK)
            tColor = DARK_INPUT_BG_COL;
        else
            tColor = LIGHT_INPUT_BG_COL;

        draw_list->AddRectFilled(p0, p1, tColor);

        if (selectedInputText) {

            if (appData.uiTheme == DARK)
                tColor = DARK_TEXT_SELECT;
            else
                tColor = LIGHT_TEXT_SELECT;

            ImVec2 rectP0 = ImVec2(p0.x + startSpaceX + iBarOffsetX + selectedInputP0_x, p0.y);
            ImVec2 rectP1 = ImVec2(p0.x + startSpaceX + iBarOffsetX + selectedInputP1_x, p1.y);

            draw_list->AddRectFilled(rectP0, rectP1, tColor);

        }



        std::vector<FormattedInputStr*> formattedStrList;

        int strPC = 0;
        int strBufferPC = 0;
        int tempIndex = 0;


        formattedStrList.push_back(new FormattedInputStr);

        while(strPC < inputTextBarBuffer.length()) {

            //formattedStrList.

            if (inputTextBarBuffer.at(strPC) == '[') {

                tempIndex = formattedStrList.size() - 1;
                formattedStrList.at(tempIndex)->str.assign(inputTextBarBuffer.substr(strBufferPC, (strPC - strBufferPC)));
                formattedStrList.at(tempIndex)->simpleTxt = true;
                formattedStrList.at(tempIndex)->mainStrIndex = strBufferPC;
                strBufferPC = strPC;

                formattedStrList.push_back(new FormattedInputStr);

                int startIndex = strPC++;

                int lastIndex = inputTextBarBuffer.find_first_of(']', strPC);

                if (lastIndex != std::string::npos) {

                    if (FunctionTools::increasePC2NextChar(&inputTextBarBuffer, &strPC, lastIndex)) {
                        bool binaryType = false;
                        bool decType = false;
                        bool hexType = false;
                        bool maskType = false;

                        if ((inputTextBarBuffer.at(strPC) > 0x2F && inputTextBarBuffer.at(strPC) < 0x3A) &&
                            (strPC + 1) < lastIndex) {
                            switch (inputTextBarBuffer.at(strPC + 1)) {
                                case 'b':
                                case 'B':
                                    binaryType = true;
                                    break;
                                case 'x':
                                case 'X':
                                    if (FunctionTools::isStartHEX_format(inputTextBarBuffer, strPC, lastIndex)) {
                                        maskType = true;
                                        hexType = true;
                                    }
                                    break;
                                default:
                                    if (FunctionTools::isStartDEC_format(inputTextBarBuffer, strPC, lastIndex)) {
                                        maskType = true;
                                        decType = true;
                                    }
                                    break;
                            }

                            if (maskType) {
                                if(strPC != lastIndex){
                                    while(strPC < lastIndex){
                                        if(FunctionTools::increasePC2NextChar(&inputTextBarBuffer, &strPC, lastIndex)){
                                            if(hexType) {
                                                if (!FunctionTools::isHEX_format(inputTextBarBuffer, strPC, lastIndex))
                                                    maskType = false;
                                            }
                                            if(decType){
                                                if(!FunctionTools::isDEC_format(inputTextBarBuffer, strPC, lastIndex))
                                                    maskType = false;
                                            }
                                        }
                                    }
                                }

                                if(maskType) {

                                    tempIndex = formattedStrList.size() - 1;
                                    formattedStrList.at(tempIndex)->str.assign(inputTextBarBuffer.substr(strBufferPC, (strPC - strBufferPC) + 1));
                                    formattedStrList.at(tempIndex)->simpleTxt = false;
                                    formattedStrList.at(tempIndex)->mainStrIndex = strBufferPC;
                                    strBufferPC = strPC;
                                    strBufferPC++;                                                                      //increase it to compensate the increase of strPC at the end off the while loop cycle

                                    formattedStrList.push_back(new FormattedInputStr);
                                }

                            }//END if(hexType)

                        }//END if

                    }

                }
            }

            strPC++;
        }

        if(strPC != strBufferPC){
            tempIndex = formattedStrList.size() - 1;
            formattedStrList.at(tempIndex)->str.assign(inputTextBarBuffer.substr(strBufferPC, (strPC - strBufferPC)));
            formattedStrList.at(tempIndex)->simpleTxt = true;
            formattedStrList.at(tempIndex)->mainStrIndex = strBufferPC;
        }

        ImVec2 tPos = textPos;

        for(size_t i = 0; i < formattedStrList.size(); i++){

            if(formattedStrList.at(i)->simpleTxt){
                if (appData.uiTheme == DARK)
                    tColor = IM_COL32_WHITE;
                else
                    tColor = IM_COL32_BLACK;

                if(formattedStrList.at(i)->mainStrIndex > 0){
                    tPos.x = textPos.x;
                    tPos.x += ImGui::CalcTextSize(inputTextBarBuffer.substr(0, (formattedStrList.at(i)->mainStrIndex)).c_str()).x;
                    draw_list->AddText(tPos, tColor, formattedStrList.at(i)->str.c_str());
                }
                else
                    draw_list->AddText(textPos, tColor, formattedStrList.at(i)->str.c_str());
            }

            else{

                if(appData.uiTheme == DARK)
                    tColor = DARK_SPECIAL_INPUT;
                else
                    tColor = LIGHT_SPECIAL_INPUT;

                if(formattedStrList.at(i)->mainStrIndex > 0){
                    tPos.x = textPos.x;
                    tPos.x += ImGui::CalcTextSize(inputTextBarBuffer.substr(0, (formattedStrList.at(i)->mainStrIndex)).c_str()).x;
                    draw_list->AddText(tPos, tColor, formattedStrList.at(i)->str.c_str());
                }
                else
                    draw_list->AddText(textPos, tColor, formattedStrList.at(i)->str.c_str());

            }


            delete formattedStrList.at(i);

        }




        /*
        if (appData.uiTheme == DARK)
            tColor = IM_COL32_WHITE;
        else
            tColor = IM_COL32_BLACK;

        draw_list->AddText(textPos, tColor, inputTextBarBuffer.c_str());
        */



        if (onInputTextBar) {
            caretCurrentTime += dt;
            if (caretCurrentTime > 700) {
                caretCurrentTime = 0;

                showInputBarCaret = !showInputBarCaret;
            }

            if (appData.uiTheme == DARK)
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

void MainWindow::deleteSelectedChars() {
    if(iTextBarBufferPC < iTextBarBufferPC2)
        inputTextBarBuffer.erase(inputTextBarBuffer.begin() + iTextBarBufferPC,
                                 inputTextBarBuffer.begin() + iTextBarBufferPC2);
    else {                                                                                                      //CRASH HERE??????
        inputTextBarBuffer.erase(inputTextBarBuffer.begin() + iTextBarBufferPC2,
                                 inputTextBarBuffer.begin() + iTextBarBufferPC);

        iTextBarBufferPC = iTextBarBufferPC2;
        caretXPos = selectedInputP1_x;
    }
}

void MainWindow::checkMouseInsideInputTextBar(AppData &appdata, const IOData &ioData) {
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
}

void MainWindow::checkInputTextBarIO(const double &dt, AppData &appdata, const IOData &ioData) {

    static bool moveChange = false;
    static bool inputBarPressed = false;
    static bool selectWord = false;
    static double dClickCurrentTime = 1000;
    static UndoRedoState undoRedoState = IDLE_UR;

    dClickCurrentTime += dt;
    if (dClickCurrentTime < 400)
        selectWord = true;
    else
        dClickCurrentTime = 1000;


    if(ioData.mouseBtnLeft == DOWN){
        if(appdata.cursorOverInputTextBar) {
            onInputTextBar = true;
            showInputBarCaret = true;
            caretCurrentTime = 0;
            selectedInputText = false;
            moveChange = false;
            inputBarPressed = true;

            float clickDistance = ioData.mouseCursorPositionRaw.x - inputBarPos.x;

            clickDistance -= iBarOffsetX;
            clickDistance -= FunctionTools::norm2HeightFloat(2);                //offset fix

            iTextBarBufferPC = 0;

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

            selectedInputP0_x = caretXPos;
            selectedInputP1_x = caretXPos;
            iTextBarBufferPC2 = iTextBarBufferPC;

            dClickCurrentTime = 0;

            if(selectWord){                                                                                             //double click over text
                dClickCurrentTime = 1000;
                if(!inputTextBarBuffer.empty()) {

                    if (iTextBarBufferPC >= inputTextBarBuffer.length())
                        iTextBarBufferPC = (inputTextBarBuffer.length() - 1);


                    iTextBarBufferPC2 = iTextBarBufferPC;

                    std::string::iterator it;

                    if (inputTextBarBuffer.at(iTextBarBufferPC) == ' ') {             //over space

                        bool onChar = false;

                        for (it = inputTextBarBuffer.begin() + (--iTextBarBufferPC);
                             it >= inputTextBarBuffer.begin(); it--) {
                            if (*it != ' ')
                                onChar = true;

                            if (onChar && *it == ' ')
                                break;


                            iTextBarBufferPC--;
                        }

                        iTextBarBufferPC++;

                        if (iTextBarBufferPC < 0)
                            iTextBarBufferPC = 0;


                        for (it = inputTextBarBuffer.begin() + (++iTextBarBufferPC2);
                             it != inputTextBarBuffer.end(); it++) {
                            if (*it != ' ')
                                break;

                            iTextBarBufferPC2++;
                        }

                    } else {                                                           //over character

                        bool notChar = false;

                        for (it = inputTextBarBuffer.begin() + iTextBarBufferPC;
                             it >= inputTextBarBuffer.begin(); it--) {
                            if (*it == ' ')
                                break;
                            iTextBarBufferPC--;
                        }

                        iTextBarBufferPC++;

                        if (iTextBarBufferPC < 0)
                            iTextBarBufferPC = 0;


                        for (it = inputTextBarBuffer.begin() + (++iTextBarBufferPC2);
                             it != inputTextBarBuffer.end(); it++) {

                            if (*it == ' ')
                                notChar = true;

                            if (notChar && *it != ' ')
                                break;


                            iTextBarBufferPC2++;
                        }

                    }

                    selectedInputText = true;

                    selectedInputP0_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC).c_str()).x;
                    selectedInputP1_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC2).c_str()).x;
                    caretXPos = selectedInputP1_x;

                    inputBarPressed = false;

                }
            }

            ImGui::PopFont();

        }
        else {
            onInputTextBar = false;
            inputBarPressed = false;
        }
    }
    else if(ioData.mouseBtnLeft == ON){

        if(inputBarPressed) {

            ImGui::PushFont(appdata.monoFont);

            if (ioData.mouseCursorPositionRaw.x < inputBarPos.x) {

                int deltaMove = static_cast<int>(inputBarPos.x - ioData.mouseCursorPositionRaw.x);

                int divMod = deltaMove % FunctionTools::norm2Height(8);

                if (divMod == 0 && !moveChange) {
                    moveChange = true;

                    iBarOffsetX += FunctionTools::norm2HeightFloat(32);
                    if (iBarOffsetX > 0)
                        iBarOffsetX = 0;


                    iTextBarBufferPC2 -= 4;
                    if (iTextBarBufferPC2 < 0)
                        iTextBarBufferPC2 = 0;

                    selectedInputP1_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC2).c_str()).x;

                    caretXPos = selectedInputP1_x;

                } else if (divMod != 0 && moveChange)
                    moveChange = false;


            } else if (ioData.mouseCursorPositionRaw.x > (inputBarPos.x + inputBarSize.x)) {
                int deltaMove = static_cast<int>(ioData.mouseCursorPositionRaw.x - (inputBarPos.x + inputBarSize.x));

                int divMod = deltaMove % FunctionTools::norm2Height(8);

                if (divMod == 0 && !moveChange) {
                    moveChange = true;

                    iBarOffsetX -= FunctionTools::norm2HeightFloat(32);

                    if(iBarOffsetX < ((ImGui::CalcTextSize(inputTextBarBuffer.c_str()).x - (inputBarSize.x * 0.5f)) * -1) )
                        iBarOffsetX = (ImGui::CalcTextSize(inputTextBarBuffer.c_str()).x - (inputBarSize.x * 0.5f)) * -1;

                    iTextBarBufferPC2 += 4;
                    if(iTextBarBufferPC2 > inputTextBarBuffer.length())
                        iTextBarBufferPC2 = inputTextBarBuffer.length();

                    selectedInputP1_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC2).c_str()).x;

                    caretXPos = selectedInputP1_x;

                } else if (divMod != 0 && moveChange)
                    moveChange = false;

            } else {

                float clickDistance = ioData.mouseCursorPositionRaw.x - inputBarPos.x;

                clickDistance -= iBarOffsetX;
                clickDistance -= FunctionTools::norm2HeightFloat(2);                //offset fix

                if (!inputTextBarBuffer.empty()) {
                    selectedInputP1_x = ImGui::CalcTextSize(inputTextBarBuffer.c_str()).x;

                    if (clickDistance > selectedInputP1_x)
                        iTextBarBufferPC2 = inputTextBarBuffer.length();
                    else {
                        for (size_t i = 0; i < inputTextBarBuffer.size(); i++) {
                            selectedInputP1_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, (i + 1)).c_str()).x;

                            if (clickDistance < selectedInputP1_x) {

                                float caretXPos0 = selectedInputP1_x;
                                selectedInputP1_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, (i)).c_str()).x;

                                float diffCaretXUp = caretXPos0 - clickDistance;
                                float diffCaretXDown = clickDistance - selectedInputP1_x;

                                if (diffCaretXUp < diffCaretXDown) {
                                    selectedInputP1_x = caretXPos0;
                                    iTextBarBufferPC2 = i + 1;
                                } else
                                    iTextBarBufferPC2 = i;

                                break;
                            }
                        }
                    }

                    caretXPos = selectedInputP1_x;
                }
            }

            ImGui::PopFont();


            if (iTextBarBufferPC != iTextBarBufferPC2)
                selectedInputText = true;
            else
                selectedInputText = false;

        }
    }
    else if(ioData.mouseBtnLeft == UP)
        inputBarPressed = false;

    selectWord = false;


    if(onInputTextBar){

        bool updateCaretPos = false;
        bool postEndUpdate = false;
        bool onlyUpdateOnIncrement = false;

        if(ioData.keyCopy == DOWN && selectedInputText){
            std::string sStr = "";

            if(iTextBarBufferPC < iTextBarBufferPC2)
                sStr = inputTextBarBuffer.substr(iTextBarBufferPC, (iTextBarBufferPC2 - iTextBarBufferPC));
            else
                sStr = inputTextBarBuffer.substr(iTextBarBufferPC2, (iTextBarBufferPC - iTextBarBufferPC2));

            ImGui::SetClipboardText(sStr.c_str());
        }

        if(!ioData.charBuffer.empty()) {

            if (selectedInputText) {
                selectedInputText = false;

                deleteSelectedChars();

                postEndUpdate = true;
                undoRedoState = NEW;
            }

            if(ioData.keyPaste == DOWN) {
                postEndUpdate = true;
                undoRedoState = NEW;
            }



            char lastChar;
            if(inputTextBarBuffer.empty())
                undoRedoState = NEW;

            if(iTextBarBufferPC - 1 > 0)
                lastChar = inputTextBarBuffer.at(iTextBarBufferPC - 1);
            else
                undoRedoState = NEW;


            int lastIndex = undoRedo->getLastCaretIndex();                                                              //Check if user move the caret to different place. If it was changed new undo redo data to add.
            if(lastIndex != -1 && lastIndex != iTextBarBufferPC){
                int startIndex = iTextBarBufferPC;

                if(lastIndex < startIndex){
                   int tempIndex = startIndex;
                   startIndex = lastIndex;
                   lastIndex = tempIndex;
                }


                if(lastIndex >= inputTextBarBuffer.size())
                    lastIndex = inputTextBarBuffer.size()-1;

                if(lastIndex < 0)
                    undoRedoState = NEW;
                else {
                    for (size_t i = startIndex; i < lastIndex; i++) {
                        if (inputTextBarBuffer.at(i) == ' ') {
                            undoRedoState = NEW;
                            break;
                        }
                    }
                }

            }



            for (size_t i = 0; i < ioData.charBuffer.size(); i++, iTextBarBufferPC++)
                inputTextBarBuffer.insert(inputTextBarBuffer.begin() + iTextBarBufferPC, ioData.charBuffer.at(i));



            if(undoRedoState == IDLE_UR){
                bool notSpace = true;

                if(lastChar == ' ')
                    notSpace = false;

                undoRedoState = UPDATE;

                for(size_t i = 0; i < ioData.charBuffer.size(); i++){
                    if(notSpace){
                        if(ioData.charBuffer.at(i) == ' '){
                            undoRedoState = NEW;
                            break;
                        }
                    }
                    else{
                        if(ioData.charBuffer.at(i) != ' '){
                            undoRedoState = NEW;
                            break;
                        }
                    }
                }
            }


            updateCaretPos = true;
            onlyUpdateOnIncrement = true;

        }

        if(ioData.keyUndo == DOWN){

            auto* myPtr = undoRedo->undo();

            if(myPtr != nullptr){
                inputTextBarBuffer.assign((*myPtr).dataLine);
                iTextBarBufferPC = (*myPtr).posIndex;
                caretXPos = (*myPtr).caretPos;
            }
        }
        else if(ioData.keyRedo == DOWN){
            auto* myPtr = undoRedo->redo();

            if(myPtr != nullptr){
                inputTextBarBuffer.assign((*myPtr).dataLine);
                iTextBarBufferPC = (*myPtr).posIndex;
                caretXPos = (*myPtr).caretPos;
            }
        }

        if(ioData.keyLeft == DOWN){
            caretCurrentTime = 0;
            showInputBarCaret = true;

            if(ioData.keyR_Shift == ON || ioData.keyL_Shift == ON){

                if(!selectedInputText){
                    selectedInputText = true;

                    iTextBarBufferPC2 = iTextBarBufferPC;
                }

                iTextBarBufferPC2--;
                if(iTextBarBufferPC2 < 0)
                    iTextBarBufferPC2 = 0;

                int caretMultiplier = static_cast<int>(caretXPos / inputBarSize.x);
                iBarOffsetX = static_cast<float>(caretMultiplier * inputBarSize.x);
                iBarOffsetX *= -1;

                if(iBarOffsetX > 0)
                    iBarOffsetX = 0;

                ImGui::PushFont(appdata.monoFont);
                selectedInputP0_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC).c_str()).x;
                selectedInputP1_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC2).c_str()).x;
                caretXPos = selectedInputP1_x;

                ImGui::PopFont();

            }
            else {
                if (selectedInputText) {
                    selectedInputText = false;

                    if (iTextBarBufferPC < iTextBarBufferPC2)
                        caretXPos = selectedInputP0_x;
                    else {
                        iTextBarBufferPC = iTextBarBufferPC2;
                        caretXPos = selectedInputP1_x;
                    }

                    int caretMulitplier = static_cast<int>(caretXPos / inputBarSize.x);

                    iBarOffsetX = static_cast<float>(caretMulitplier * inputBarSize.x);
                    iBarOffsetX *= -1;

                    if (iBarOffsetX > 0)
                        iBarOffsetX = 0;

                } else {
                    if (iTextBarBufferPC > 0) {
                        iTextBarBufferPC--;
                        updateCaretPos = true;
                    }
                }
            }
        }

        if(ioData.keyRight == DOWN){
            caretCurrentTime = 0;
            showInputBarCaret = true;

            if(ioData.keyR_Shift == ON || ioData.keyL_Shift == ON){

                if(!selectedInputText){
                    selectedInputText = true;

                    iTextBarBufferPC2 = iTextBarBufferPC;
                }

                iTextBarBufferPC2++;
                if(iTextBarBufferPC2 > inputTextBarBuffer.length())
                    iTextBarBufferPC2 = inputTextBarBuffer.length();

                int caretMulitplier = static_cast<int>(caretXPos / inputBarSize.x);

                iBarOffsetX = static_cast<float>(caretMulitplier * inputBarSize.x);
                iBarOffsetX *= -1;

                ImGui::PushFont(appdata.monoFont);
                selectedInputP0_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC).c_str()).x;
                selectedInputP1_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC2).c_str()).x;
                caretXPos = selectedInputP1_x;

                ImGui::PopFont();

            }
            else {

                if (selectedInputText) {
                    selectedInputText = false;

                    if (iTextBarBufferPC > iTextBarBufferPC2)
                        caretXPos = selectedInputP0_x;
                    else {
                        iTextBarBufferPC = iTextBarBufferPC2;
                        caretXPos = selectedInputP1_x;
                    }

                    int caretMulitplier = static_cast<int>(caretXPos / inputBarSize.x);

                    iBarOffsetX = static_cast<float>(caretMulitplier * inputBarSize.x);
                    iBarOffsetX *= -1;


                } else {
                    if (iTextBarBufferPC < inputTextBarBuffer.size()) {
                        iTextBarBufferPC++;
                        updateCaretPos = true;
                    }
                }
            }
        }

        if(ioData.keyHome == DOWN) {

            if(ioData.keyR_Shift == ON || ioData.keyL_Shift == ON){
                if(!selectedInputText){
                    selectedInputText = true;

                    iTextBarBufferPC2 = iTextBarBufferPC;
                }

                iTextBarBufferPC2 = 0;

                iBarOffsetX = 0;

                ImGui::PushFont(appdata.monoFont);
                selectedInputP0_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC).c_str()).x;
                selectedInputP1_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC2).c_str()).x;
                caretXPos = selectedInputP1_x;

                ImGui::PopFont();
            }
            else {
                selectedInputText = false;
                iTextBarBufferPC = 0;
                iBarOffsetX = 0;
                updateCaretPos = true;
            }
        }

        if(ioData.keyEnd == DOWN) {
            if(ioData.keyR_Shift == ON || ioData.keyL_Shift == ON){
                if(!selectedInputText){
                    selectedInputText = true;

                    iTextBarBufferPC2 = iTextBarBufferPC;
                }

                iTextBarBufferPC = inputTextBarBuffer.size();

                postEndUpdate = true;
                updateCaretPos = true;

                ImGui::PushFont(appdata.monoFont);
                selectedInputP0_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC).c_str()).x;
                selectedInputP1_x = ImGui::CalcTextSize(inputTextBarBuffer.substr(0, iTextBarBufferPC2).c_str()).x;
                caretXPos = selectedInputP1_x;

                ImGui::PopFont();
            }
            else {
                selectedInputText = false;
                iTextBarBufferPC = inputTextBarBuffer.size();
                postEndUpdate = true;
                updateCaretPos = true;
            }
        }

        if(ioData.keyBackspace == DOWN){

            if(selectedInputText){
                selectedInputText = false;

                deleteSelectedChars();
                updateCaretPos = true;

                undoRedoState = NEW;
            }
            else {
                if (iTextBarBufferPC > 0) {
                    iTextBarBufferPC--;

                    inputTextBarBuffer.erase(inputTextBarBuffer.begin() + iTextBarBufferPC);
                    updateCaretPos = true;
                }
            }
        }

        if(ioData.keyDel == DOWN){

            if(selectedInputText){
                selectedInputText = false;

                deleteSelectedChars();
                updateCaretPos = true;

                undoRedoState = NEW;
            }
            else {
                if (inputTextBarBuffer.size() > 0 && iTextBarBufferPC < inputTextBarBuffer.size()) {
                    inputTextBarBuffer.erase(inputTextBarBuffer.begin() + iTextBarBufferPC);
                    updateCaretPos = true;
                }
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


            switch(undoRedoState){
                case UPDATE:
                    undoRedo->updateUndoRedoData(inputTextBarBuffer, caretXPos, iTextBarBufferPC);
                    undoRedoState = IDLE_UR;
                    break;
                case IDLE_UR:
                    break;
                case NEW:
                    auto* undoRedoPtr = new UndoRedoData;
                    (*undoRedoPtr).dataLine = inputTextBarBuffer;
                    (*undoRedoPtr).caretPos = caretXPos;
                    (*undoRedoPtr).posIndex = iTextBarBufferPC;

                    undoRedo->addNewUndoRedoData(undoRedoPtr);
                    undoRedoState = IDLE_UR;
                    break;

            }

        }

    }

}




