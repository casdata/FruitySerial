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

    inputBarInfoWindow = false;

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

    send2SerialEnter = false;

    GLuint *texturePtr = &TabSerialWindow::timestampTexture;

    bool ret = FunctionTools::loadTextureFromFile("../Assets/timestamp.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    TabSerialWindow::textureSize = ImVec2(imageWidth, imageHeight);

    texturePtr = &TabSerialWindow::timestampDisLightTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/timestampDisableLight.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::timestampDisDarkTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/timestampDisableDark.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

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

    texturePtr = &TabSerialWindow::fruitsTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/orange.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::fruitsDisDarkTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/orangeDisableDark.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::fruitsDisLightTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/orangeDisableLight.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::darkBtn0RedTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/darkbtn0Red.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::darkBtn1RedTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/darkbtn1Red.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::darkBtn2RedTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/darkbtn2Red.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::darkBtn3RedTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/darkbtn3Red.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::lightBtn0RedTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/lightbtn0Red.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::lightBtn1RedTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/lightbtn1Red.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::lightBtn2RedTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/lightbtn2Red.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &TabSerialWindow::lightBtn3RedTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/lightbtn3Red.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);


    texturePtr = &darkSendTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/lightSend.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    sendTextureSize = ImVec2(imageWidth, imageHeight);

    texturePtr = &lightSendTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/lightSend.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &darkBtn0Texture;
    ret = FunctionTools::loadTextureFromFile("../Assets/darkbtn0.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &darkBtn1Texture;
    ret = FunctionTools::loadTextureFromFile("../Assets/darkbtn1.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &darkBtn2Texture;
    ret = FunctionTools::loadTextureFromFile("../Assets/darkbtn2.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &darkBtn3Texture;
    ret = FunctionTools::loadTextureFromFile("../Assets/darkbtn3.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);


    texturePtr = &lightBtn0Texture;
    ret = FunctionTools::loadTextureFromFile("../Assets/lightbtn0.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &lightBtn1Texture;
    ret = FunctionTools::loadTextureFromFile("../Assets/lightbtn1.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &lightBtn2Texture;
    ret = FunctionTools::loadTextureFromFile("../Assets/lightbtn2.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &lightBtn3Texture;
    ret = FunctionTools::loadTextureFromFile("../Assets/lightbtn3.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &sepTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/separator.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    texturePtr = &infoTexture;
    ret = FunctionTools::loadTextureFromFile("../Assets/info.png", texturePtr, &imageWidth, &imageHeight);
    IM_ASSERT(ret);


    auto* subWin = new SubWindow(windowCount);
    subWindows.push_back(subWin);
    subWindows.back()->setWidthPercentage(1.0f);

    resizingWindowIndex = -1;

    undoRedo = new UndoRedo;
    sendLog = new SendLog();
}



void MainWindow::update(const double &dt, AppData &appdata, const IOData &ioData, SerialManager *serialManager) {


    switch(appdata.appState){
        case IDLE:
            checkAndSplitWindow(appdata.windowCount);

            checkClickAndFocus(ioData, appdata.appState);
            checkAndResizeSubWindows(appdata.cursorOverSubWinBorder, ioData);

            for(auto* subWin : subWindows)
                subWin->update(ioData, serialManager);

            checkMouseInsideInputTextBar(appdata, ioData);

            checkInputTextBarIO(dt, appdata, ioData);

            break;
        case REWIND:

            break;
        case SAVE:

            break;
        case LOAD:

            break;
        case MENU:

            break;
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

    if(inputBarInfoWindow){

        ImGuiWindowFlags infoWinFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;

        //ImGui::SetScrollX()
        static float wSizeX = FunctionTools::norm2HeightFloat(760);
        static float wSizeY = FunctionTools::norm2HeightFloat(440);
        ImGui::SetNextWindowSize(ImVec2(wSizeX, wSizeY));

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f,0.5f));

        ImGui::Begin("Info: Send raw bytes", &inputBarInfoWindow, infoWinFlags);

            ImVec4 specialCol = ImVec4(1.0f, 0.4117f, 0.2823f, 1.0f);
            ImVec4 importantCol = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);//ImVec4(1.0f, 0.2862f, 0.1764f, 1.0f);
            ImVec4 greenCol = ImVec4(0.0509f, 0.6666f, 0.3882f, 1.0f);
            ImVec4 blueCol = ImVec4(0.0f, 0.0f, 0.8745f, 1.0f);
            ImVec4 purpleCol = ImVec4(0.5803f, 0.0f, 0.8745f, 1.0f);

            if(appData.uiTheme == DARK){
                specialCol = ImVec4(1.0f, 0.4117f, 0.2823f, 1.0f);
                greenCol = ImVec4(0.0509f, 0.8980f, 0.3882f, 1.0f);
                blueCol = ImVec4(0.0f, 0.5098f, 1.0f, 1.0f);
                purpleCol = ImVec4(0.5803f, 0.2666f, 1.0f, 1.0f);
            }

            ImGui::Bullet();
            ImGui::TextColored(blueCol ,"Decimal format");
            ImGui::Text("       Open square bracket ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(purpleCol, "+");
            ImGui::SameLine(0,0);
            ImGui::Text(" three numeric characters ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(purpleCol, "+");
            ImGui::SameLine(0,0);
            ImGui::Text(" close square bracket: [DDD]");


            ImGui::TextColored(greenCol, "        Single byte example:");
            ImGui::SameLine(0,0);
            ImGui::Text("    \"This is ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(specialCol, "[097]");
            ImGui::SameLine(0,0);
            ImGui::Text(" test\" => \"This is a test\"");

            ImGui::TextColored(greenCol, "        Multiple byte example:");
            ImGui::SameLine(0,0);
            ImGui::Text("  \"");
            ImGui::SameLine(0,0);
            ImGui::TextColored(specialCol, "[084 104 105 115 32]");
            ImGui::SameLine(0,0);
            ImGui::Text("is a test\" => \"This is a test\"");


            ImGui::NewLine();

            ImGui::TextColored(importantCol, "       Important: the first byte in decimal format has to be three digits, fill with");
            ImGui::TextColored(importantCol, "       zeros if the number is lower. Other bytes do not require zeros filling.");

            ImGui::NewLine();
            ImGui::NewLine();

            ImGui::Bullet();
            ImGui::TextColored(blueCol,"Octal format");
            ImGui::Text("       Open square bracket ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(purpleCol, "+");
            ImGui::SameLine(0,0);
            ImGui::Text(" zero ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(purpleCol, "+");
            ImGui::SameLine(0,0);
            ImGui::Text(" letter \"o\" ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(purpleCol, "+");
            ImGui::SameLine(0,0);
            ImGui::Text(" octal characters ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(purpleCol, "+");
            ImGui::SameLine(0,0);
            ImGui::Text(" close square bracket: [0oOOO]");


            ImGui::TextColored(greenCol, "        Single byte example:");
            ImGui::SameLine(0,0);
            ImGui::Text("    \"This is ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(specialCol, "[0o141]");
            ImGui::SameLine(0,0);
            ImGui::Text(" test\" => \"This is a test\"");

            ImGui::TextColored(greenCol, "        Multiple byte example:");
            ImGui::SameLine(0,0);
            ImGui::Text("  \"");
            ImGui::SameLine(0,0);
            ImGui::TextColored(specialCol, "[0o124 150 151 163 40]");
            ImGui::SameLine(0,0);
            ImGui::Text("is a test\" => \"This is a test\"");


            ImGui::NewLine();
            ImGui::NewLine();

            ImGui::Bullet();
            ImGui::TextColored(blueCol,"Hexadecimal format");
            ImGui::Text("       Open square bracket ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(purpleCol, "+");
            ImGui::SameLine(0,0);
            ImGui::Text(" zero ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(purpleCol, "+");
            ImGui::SameLine(0,0);
            ImGui::Text(" letter \"x\" ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(purpleCol, "+");
            ImGui::SameLine(0,0);
            ImGui::Text(" hex characters ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(purpleCol, "+");
            ImGui::SameLine(0,0);
            ImGui::Text(" close square bracket: [0xHH]");

            ImGui::TextColored(greenCol, "        Single byte example:");
            ImGui::SameLine(0,0);
            ImGui::Text("    \"This is ");
            ImGui::SameLine(0,0);
            ImGui::TextColored(specialCol, "[0x61]");
            ImGui::SameLine(0,0);
            ImGui::Text(" test\" => \"This is a test\"");

            ImGui::TextColored(greenCol, "        Multiple byte example:");
            ImGui::SameLine(0,0);
            ImGui::Text("  \"");
            ImGui::SameLine(0,0);
            ImGui::TextColored(specialCol, "[0x54 68 69 73 20]");
            ImGui::SameLine(0,0);
            ImGui::Text("is a test\" => \"This is a test\"");

            ImGui::NewLine();
            ImGui::NewLine();


            bool infoBtn = appData.disableInBarInfoBtn;

            ImGui::Checkbox("Hide information button", &appData.disableInBarInfoBtn);
            if(ImGui::IsItemHovered()){
                ImGui::BeginTooltip();
                ImGui::Text("Hide the information button that accesses this window.");
                ImGui::Text("Can be enabled again from the settings window.");
                ImGui::EndTooltip();
            }

            if(infoBtn != appData.disableInBarInfoBtn){
                char* temp = new char[1];

                *(temp) = static_cast<char>(appData.disableInBarInfoBtn);

                FunctionTools::replaceData2File(dataFileName, 3, 1, temp);

                delete[] temp;

            }



        ImGui::End();
    }

}

MainWindow::~MainWindow() {
    if(!subWindows.empty()){
        for(auto* serialW : subWindows)
            delete serialW;

        subWindows.clear();
    }

    delete undoRedo;

    delete sendLog;

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

    bool sendData2Serial = send2SerialEnter;

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


    inputBarPos = ImGui::GetCursorScreenPos();

    ImVec2 myCursorPos = ImGui::GetCursorPos();


    int comboItem = 0;

    if(inputBarEnabled)
        comboItem = static_cast<int>(serialPtr->getOutputEol());

    int currentItem = comboItem;

    float tempWidth = FunctionTools::norm2HeightFloat(18);
    float tempHeight = tempWidth;//static_cast<int>(sendTextureSize.y));//24);

    float tempX = ImGui::GetWindowSize().x - (myCursorPos.x * 2) - tempWidth;

    int imagePadding = FunctionTools::norm2Height(1);


    ImGui::SetCursorPos(ImVec2(tempX, myCursorPos.y));
    ImGui::SetNextItemWidth(tempWidth);

    GLuint tempTexture = lightBtn0Texture;

    if(appData.uiTheme == DARK){
        switch(currentItem){
            case 1:
                tempTexture = darkBtn1Texture;
                break;
            case 2:
                tempTexture = darkBtn2Texture;
                break;
            case 3:
                tempTexture = darkBtn3Texture;
                break;
            default:
                tempTexture = darkBtn0Texture;
                break;
        }
    }
    else{
        switch(currentItem){
            case 1:
                tempTexture = lightBtn1Texture;
                break;
            case 2:
                tempTexture = lightBtn2Texture;
                break;
            case 3:
                tempTexture = lightBtn3Texture;
                break;
            default:
                tempTexture = lightBtn0Texture;
                break;
        }
    }


    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_WindowBg));

    if(ImGui::ImageButton((void*)(intptr_t)tempTexture, ImVec2(tempWidth, tempHeight), ImVec2(0, 0), ImVec2(1, 1), imagePadding))
        ImGui::OpenPopup("eolOutputPopup");

    ImGui::PopStyleColor();

    if(ImGui::BeginPopup("eolOutputPopup")){
        ImGui::Text("EOL");
        ImGui::Separator();

        bool selected = false;

        for(size_t i = 0; i < IM_ARRAYSIZE(EOL_ITEMS); i++){
            selected = (i == comboItem);
            if(ImGui::Selectable(EOL_ITEMS[i], selected))
                currentItem = i;
        }

        if(currentItem != comboItem && inputBarEnabled)
            serialPtr->setOutputEol(static_cast<serialEndOfLine>(currentItem));

        ImGui::EndPopup();
    }


    //Separator
    tempWidth = FunctionTools::norm2HeightFloat(6);
    tempX -= (tempWidth + myCursorPos.x);

    ImGui::SetCursorPos(ImVec2(tempX, myCursorPos.y));
    ImGui::Image((void*)(intptr_t)sepTexture, ImVec2(tempWidth, tempHeight), ImVec2(0,0), ImVec2(1,1));


    //Info BTN
    if(!appData.disableInBarInfoBtn) {
        tempWidth = FunctionTools::norm2HeightFloat(18);
        tempX -= ((myCursorPos.x * 2) + tempWidth);

        ImGui::SetCursorPos(ImVec2(tempX, myCursorPos.y));

        if (inputBarInfoWindow)
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
        else
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_WindowBg));


        if (ImGui::ImageButton((void *) (intptr_t) infoTexture, ImVec2(tempWidth, tempHeight), ImVec2(0, 0),
                               ImVec2(1, 1), imagePadding))
            inputBarInfoWindow = true;

        ImGui::PopStyleColor();
    }


    //Send BTN

    tempWidth = FunctionTools::norm2HeightFloat(36);
    tempX -= ((myCursorPos.x * 2) + tempWidth);

    ImGui::SetCursorPos(ImVec2(tempX, myCursorPos.y));

    tempTexture = lightSendTexture;

    if(appData.uiTheme == DARK)
        tempTexture = darkSendTexture;

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_WindowBg));

    if(ImGui::ImageButton((void*)(intptr_t)tempTexture, ImVec2(tempWidth, tempHeight), ImVec2(0,0), ImVec2(1,1), imagePadding)){
        if(inputBarEnabled && !inputTextBarBuffer.empty()){

            sendData2Serial = true;
            //serialPtr->write2Port(inputTextBarBuffer);

            //if(inputBarEnabled)
                //comboItem = static_cast<int>(serialPtr->getOutputEol());

            //std::cout<<"-> "<<inputBarEnabled<<" "<<inputTextBarBuffer<<std::endl;
        }
    }

    ImGui::PopStyleColor();


    inputBarSize.x = tempX - (myCursorPos.x * 2);
    inputBarSize.y = FunctionTools::norm2HeightFloat(20);

    ImGui::SetCursorScreenPos(inputBarPos);

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

            if (inputTextBarBuffer.at(strPC) == '[') {

                tempIndex = formattedStrList.size() - 1;
                formattedStrList.at(tempIndex)->str.assign(inputTextBarBuffer.substr(strBufferPC, (strPC - strBufferPC)));
                formattedStrList.at(tempIndex)->strDataType = STR_TXT;
                formattedStrList.at(tempIndex)->mainStrIndex = strBufferPC;
                strBufferPC = strPC;

                formattedStrList.push_back(new FormattedInputStr);

                int startIndex = strPC++;

                int lastIndex = inputTextBarBuffer.find_first_of(']', strPC);

                if (lastIndex != std::string::npos) {

                    if (FunctionTools::increasePC2NextChar(&inputTextBarBuffer, &strPC, lastIndex)) {
                        bool binaryType = false;
                        bool octType = false;
                        bool decType = false;
                        bool hexType = false;
                        bool maskType = false;

                        if ((inputTextBarBuffer.at(strPC) > 0x2F && inputTextBarBuffer.at(strPC) < 0x3A) &&
                            (strPC + 1) < lastIndex) {
                            switch (inputTextBarBuffer.at(strPC + 1)) {
                                case 'o':
                                case 'O':
                                    if(FunctionTools::isStartOCT_format(inputTextBarBuffer, strPC, lastIndex)){
                                        maskType = true;
                                        octType = true;
                                    }
                                    break;
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
                                            if(octType){
                                                if(!FunctionTools::isOCT_format(inputTextBarBuffer, strPC, lastIndex))
                                                    maskType = false;
                                            }
                                        }
                                    }
                                }

                                if(maskType) {

                                    tempIndex = formattedStrList.size() - 1;
                                    formattedStrList.at(tempIndex)->str.assign(inputTextBarBuffer.substr(strBufferPC, (strPC - strBufferPC) + 1));

                                    if(hexType)
                                        formattedStrList.at(tempIndex)->strDataType = STR_HEX;
                                    else if(decType)
                                        formattedStrList.at(tempIndex)->strDataType = STR_DEC;
                                    else
                                        formattedStrList.at(tempIndex)->strDataType = STR_OCT;

                                    //formattedStrList.at(tempIndex)->simpleTxt = false;
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
            formattedStrList.at(tempIndex)->strDataType = STR_TXT;
            formattedStrList.at(tempIndex)->mainStrIndex = strBufferPC;
        }

        ImVec2 tPos = textPos;


        if(sendData2Serial){
            send2SerialEnter = false;

            std::string strByteBuffer = std::string();

            for(size_t i = 0; i < formattedStrList.size(); i++){

                switch(formattedStrList.at(i)->strDataType){
                    case STR_TXT:
                        strByteBuffer.append(formattedStrList.at(i)->str);
                        break;
                    case STR_HEX:
                        strByteBuffer.append(FunctionTools::hexStr2ByteStr(formattedStrList.at(i)->str));
                        break;
                    case STR_DEC:
                        strByteBuffer.append(FunctionTools::decStr2ByteStr(formattedStrList.at(i)->str));
                        break;
                    case STR_OCT:
                        strByteBuffer.append(FunctionTools::octStr2ByteStr(formattedStrList.at(i)->str));
                        break;
                    default:

                        break;
                }

            }

            serialPtr->write2Port(strByteBuffer);

            sendLog->addNewLog(inputTextBarBuffer);

            inputTextBarBuffer.clear();

            caretCurrentTime = 0;
            iTextBarBufferPC = 0;
            iTextBarBufferPC2 = 0;
            caretXPos = 0;
            iBarOffsetX = 0;

            selectedInputP0_x = 0;
            selectedInputP1_x = 0;
        }


        for(size_t i = 0; i < formattedStrList.size(); i++){

            if(formattedStrList.at(i)->strDataType == STR_TXT){
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
    else {                                                                                                              //CRASH HERE??????
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

    bool overdriveEndBtn = false;
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



            for (size_t i = 0; i < ioData.charBuffer.size(); i++, iTextBarBufferPC++) {
                if(ioData.charBuffer.at(i) == 0xD || ioData.charBuffer.at(i) == 0xA)                                //carriage or new line char
                    inputTextBarBuffer.insert(inputTextBarBuffer.begin() + iTextBarBufferPC, 0x20);                 //space char
                else
                    inputTextBarBuffer.insert(inputTextBarBuffer.begin() + iTextBarBufferPC, ioData.charBuffer.at(i));
            }



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

        if(ioData.keyUp == DOWN && sendLog->dataOnLogList()){
            inputTextBarBuffer.clear();
            inputTextBarBuffer.assign(*sendLog->getPreviousLog());
            overdriveEndBtn = true;
        }

        if(ioData.keyDown == DOWN && sendLog->dataOnLogList()){
            inputTextBarBuffer.clear();
            inputTextBarBuffer.assign(*sendLog->getNextLog());
            overdriveEndBtn = true;
        }

        if(ioData.keyLeft == DOWN){
            ImGui::SetKeyboardFocusHere();

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
            ImGui::SetKeyboardFocusHere();

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

        if(ioData.keyEnd == DOWN || overdriveEndBtn) {
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

        if(ioData.keyEnter == DOWN){
            if(inputBarEnabled && !inputTextBarBuffer.empty())
                send2SerialEnter = true;
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




