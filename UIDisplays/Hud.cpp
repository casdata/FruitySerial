//
// Created by castdata on 1/15/23.
//

#include "Hud.h"
#include "imgui_internal.h"


Hud::Hud() {

    int imageWidth = 0;
    int imageHeight = 0;

    bool ret = FunctionTools::loadTextureFromFile("../Assets/iconQ.png", &questionTexture, &imageWidth, &imageHeight); //Assets
    IM_ASSERT(ret);
    questionSize = new ImVec2(imageWidth, imageHeight);

    ret = FunctionTools::loadTextureFromFile("../Assets/menuLightUp.png", &menuLightTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);
    menuBarIconSize = new ImVec2(imageWidth, imageHeight);

    ret = FunctionTools::loadTextureFromFile("../Assets/menuDarkUp.png", &menuDarkTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret  = FunctionTools::loadTextureFromFile("../Assets/icon.png", &iconTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/newBtn.png", &newBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/openBtn.png", &openBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/saveAsBtn.png", &saveAsBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/saveBtn.png", &saveBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/rewindBtn.png", &rewindTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/uNewBtn.png", &uNewBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/uOpenBtn.png", &uOpenBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/uSaveBtn.png", &uSaveBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/uSaveAsBtn.png", &uSaveAsBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/uEmpty.png", &uEmptyBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/sun.png", &sunBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/moon.png", &moonBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/withBar.png", &withBarTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/withoutBar.png", &withoutBarTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/minimizeBtn.png", &minimizeTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/maximizeBtn.png", &maximizeTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/maximize2Btn.png", &maximize2Texture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/closeBtn.png", &closeTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);
    cornerIconSize = new ImVec2(imageWidth, imageHeight);

    mainWin = new MainWindow();

}




void Hud::menuBar(MenuData &menuData, AppData &appData, IOData &ioData) {

    static bool menuShown = false;

    float framePadding = static_cast<float>(FunctionTools::norm2Height(10));

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
    if(ImGui::BeginMenuBar()){

        GLuint texture;

        if(appData.uiTheme == LIGHT)
            texture = menuDarkTexture;
        else
            texture = menuLightTexture;

        //if(menuData.titleBar != TB_ENABLE)
            //texture = iconTexture;

        float iconSize = FunctionTools::norm2Height(static_cast<int>(menuBarIconSize->y));
        int imagePadding = FunctionTools::norm2Height(4);


        if(menuData.titleBar != TB_ENABLE) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_MenuBarBg));

            ImVec2 tempCursor0 = ImGui::GetCursorScreenPos();

            if(ImGui::ImageButton((void*)(intptr_t)iconTexture, ImVec2(iconSize,iconSize), ImVec2(0,0), ImVec2(1,1),imagePadding)){
                ImGui::OpenPopup("iconMenu");
            }

            ImGui::PopStyleColor();

            ImGui::SetNextWindowPos(ImVec2(tempCursor0.x + FunctionTools::norm2Height(4), ImGui::GetWindowViewport()->Pos.y + FunctionTools::norm2Height(31)));
            if(ImGui::BeginPopup("iconMenu")){
                if(ImGui::MenuItem("Restore")){

                }
                ImGui::Spacing();

                if(ImGui::MenuItem("Minimize")){

                }
                ImGui::Spacing();

                if(ImGui::MenuItem("Maximize")){

                }
                ImGui::Separator();

                if(ImGui::MenuItem("Close")){

                }

                ImGui::EndPopup();
            }

            ImGui::Spacing();

        }


        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_MenuBarBg));

        static bool fileBtnSelected = false;
        static bool editBtnSelected = false;
        static bool helpBtnSelected = false;

        if(menuShown) {

            bool fileState = false;
            bool editState = false;
            bool helpState = false;

            ImGui::PopStyleColor();

            ImVec2 tempPos = ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetWindowViewport()->Pos.y + FunctionTools::norm2Height(31));
            ImGui::SetNextWindowPos(tempPos);

            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_MenuBarBg));

            if(fileBtnSelected) {
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
            }

            if(ImGui::Button("File")){

            }

            if(ioData.mouseCursorPositionRaw.x > tempPos.x && ioData.mouseCursorPositionRaw.x < ImGui::GetCursorScreenPos().x
               && (ioData.mouseCursorPositionRaw.y > ImGui::GetWindowViewport()->Pos.y && ioData.mouseCursorPositionRaw.y < tempPos.y)){
                if(!fileState){
                    ImGui::CloseCurrentPopup();
                    ImGui::OpenPopup("fileMenu");
                }
            }


            if(ImGui::IsPopupOpen("fileMenu")) {
                fileState = true;
                fileBtnSelected = true;
                editBtnSelected = false;
                helpBtnSelected = false;
            }

            if(ImGui::BeginPopup("fileMenu")){

                ImGui::Spacing();

                int uIconSize = FunctionTools::norm2Height(11);

                ImGui::Image((void *) (intptr_t) uNewBtnTexture, ImVec2(uIconSize, uIconSize));
                ImGui::SameLine();

                if (ImGui::MenuItem("New", "Ctrl + n")) {

                }
                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Image((void *) (intptr_t) uOpenBtnTexture, ImVec2(uIconSize, uIconSize));
                ImGui::SameLine();
                if (ImGui::MenuItem("Open", "Ctrl + o")) {

                }
                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Image((void *) (intptr_t) uSaveBtnTexture, ImVec2(uIconSize, uIconSize));
                ImGui::SameLine();
                if (ImGui::MenuItem("Save", "Ctrl + s")) {

                }
                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Image((void *) (intptr_t) uSaveAsBtnTexture, ImVec2(uIconSize, uIconSize));
                ImGui::SameLine();
                if (ImGui::MenuItem("Save As", "Ctrl + Alt + s")) {

                }
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Image((void *) (intptr_t) uEmptyBtnTexture, ImVec2(uIconSize, uIconSize));
                ImGui::SameLine();
                if (ImGui::MenuItem("Exit", "Alt + F4")) {
                    menuData.exitApp = true;
                }

                ImGui::EndPopup();
            }

            tempPos = ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetWindowViewport()->Pos.y + FunctionTools::norm2Height(31));
            ImGui::SetNextWindowPos(tempPos);


            if(editBtnSelected) {
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
            }
            else {
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_MenuBarBg));
            }

            if(ImGui::Button("Edit")){

            }

            if(ioData.mouseCursorPositionRaw.x > tempPos.x && ioData.mouseCursorPositionRaw.x < ImGui::GetCursorScreenPos().x
               && (ioData.mouseCursorPositionRaw.y > ImGui::GetWindowViewport()->Pos.y && ioData.mouseCursorPositionRaw.y < tempPos.y)){
                if(!editState){
                    ImGui::CloseCurrentPopup();
                    ImGui::OpenPopup("editMenu");
                }
            }

            if(ImGui::IsPopupOpen("editMenu")) {
                editState = true;
                fileBtnSelected = false;
                editBtnSelected = true;
                helpBtnSelected = false;
            }

            if(ImGui::BeginPopup("editMenu")){

                if (ImGui::BeginMenu("Connections")) {

                    if (ImGui::MenuItem("One")) {

                    }
                    ImGui::Spacing();
                    ImGui::Spacing();
                    if (ImGui::MenuItem("Two")) {

                    }
                    ImGui::Spacing();

                    ImGui::EndMenu();
                }

                ImGui::EndPopup();
            }

            tempPos = ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetWindowViewport()->Pos.y + FunctionTools::norm2Height(31));
            ImGui::SetNextWindowPos(tempPos);

            if(helpBtnSelected) {
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
            }
            else {
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_MenuBarBg));
            }

            if(ImGui::Button("Help")){

            }

            ImGui::PopStyleColor();

            if(ioData.mouseCursorPositionRaw.x > tempPos.x && ioData.mouseCursorPositionRaw.x < ImGui::GetCursorScreenPos().x
               && (ioData.mouseCursorPositionRaw.y > ImGui::GetWindowViewport()->Pos.y && ioData.mouseCursorPositionRaw.y < tempPos.y)){
                if(!helpState){
                    ImGui::CloseCurrentPopup();
                    ImGui::OpenPopup("helpMenu");
                }
            }

            if(ImGui::IsPopupOpen("helpMenu")) {
                helpState = true;
                fileBtnSelected = false;
                editBtnSelected = false;
                helpBtnSelected = true;
            }

            if(ImGui::BeginPopup("helpMenu")){

                if (ImGui::MenuItem("Help")) {

                }
                ImGui::Spacing();
                ImGui::Spacing();
                if (ImGui::MenuItem("About")) {

                }

                ImGui::EndPopup();
            }

            if(!fileState && !editState && !helpState)
                menuShown = false;

            if(ImGui::IsKeyPressed(ImGuiKey_Escape)){
                ImGui::CloseCurrentPopup();
                menuShown = false;
            }

            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_MenuBarBg));

        }
        else{

            fileBtnSelected = false;
            editBtnSelected = false;
            helpBtnSelected = false;

            if(ImGui::ImageButton((void*)(intptr_t)texture, ImVec2(iconSize,iconSize), ImVec2(0,0), ImVec2(1,1), imagePadding)) {
                menuShown = true;
                ImGui::OpenPopup("fileMenu");
            }

            if(appData.appState == MENU)
                appData.appState = IDLE;

            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_MenuBarBg));

            if (ImGui::ImageButton((void *) (intptr_t) newBtnTexture, ImVec2(iconSize, iconSize), ImVec2(0, 0),
                                   ImVec2(1, 1), imagePadding)) {
                ImGui::OpenPopup("sergio");
            }

            if (ImGui::ImageButton((void *) (intptr_t) openBtnTexture, ImVec2(iconSize, iconSize), ImVec2(0, 0),
                                   ImVec2(1, 1), imagePadding)) {

            }


            if (ImGui::ImageButton((void *) (intptr_t) saveBtnTexture, ImVec2(iconSize, iconSize), ImVec2(0, 0),
                                   ImVec2(1, 1), imagePadding)) {

            }

            if (ImGui::ImageButton((void *) (intptr_t) rewindTexture, ImVec2(iconSize, iconSize), ImVec2(0,0),
                                   ImVec2(1,1), imagePadding)){
                appData.appState = REWIND;

                ioData.rewindPorts.clear();

            }

        }



        int spaceBetween = FunctionTools::norm2Height(8);
        float iconWidth = FunctionTools::norm2Height(static_cast<int>(cornerIconSize->x));
        float tempX = ImGui::GetWindowSize().x - iconWidth - spaceBetween;

        if(menuData.titleBar == TB_DISABLE) {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, CLOSE_BTN_OVER);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, CLOSE_BTN_PRESSED);
            ImGui::SetCursorPos(ImVec2(tempX, 0));
            if (ImGui::ImageButton((void *) (intptr_t) closeTexture, ImVec2(iconWidth, iconSize), ImVec2(0, 0),
                                   ImVec2(1, 1), imagePadding))
                menuData.exitApp = true;
            ImGui::PopStyleColor(2);

            tempX -= (spaceBetween + iconWidth);


            ImGui::SetCursorPos(ImVec2(tempX, 0));
            switch (menuData.maximize) {
                case MAXIMIZE:
                case SET_2MAXIMIZE:
                    if (ImGui::ImageButton((void *) (intptr_t) maximizeTexture, ImVec2(iconWidth, iconSize),
                                           ImVec2(0, 0), ImVec2(1, 1), imagePadding)) {
                        menuData.maximize = SET_2NORMAL;
                    }
                    break;
                case NORMAL:
                case SET_2NORMAL:
                    if (ImGui::ImageButton((void *) (intptr_t) maximize2Texture, ImVec2(iconWidth, iconSize),
                                           ImVec2(0, 0), ImVec2(1, 1), imagePadding)) {
                        menuData.maximize = SET_2MAXIMIZE;
                    }
                    break;
            }


            tempX -= (spaceBetween + iconWidth);


            ImGui::SetCursorPos(ImVec2(tempX, 0));
            if (ImGui::ImageButton((void *) (intptr_t) minimizeTexture, ImVec2(iconWidth, iconSize), ImVec2(0, 0),
                                   ImVec2(1, 1), imagePadding)) {

            }

            tempX -= ((spaceBetween * 2) + iconSize);

        }
        else
            tempX += spaceBetween;


        ImGui::SetCursorPos(ImVec2(tempX, 0));
        bool myBtnState = false;

        if(menuData.titleBar == TB_ENABLE) {
            if (ImGui::ImageButton((void *) (intptr_t) withoutBarTexture, ImVec2(iconSize, iconSize), ImVec2(0, 0),
                                   ImVec2(1, 1), imagePadding))
                myBtnState = true;
        }
        else{
            if (ImGui::ImageButton((void *) (intptr_t) withBarTexture, ImVec2(iconSize, iconSize), ImVec2(0, 0),
                                   ImVec2(1, 1), imagePadding))
                myBtnState = true;
        }

        if(myBtnState){

            char* temp = new char[1];

            if(menuData.titleBar == TB_ENABLE) {
                menuData.titleBar = TB_2DISABLE;
                *(temp) = (char)0;
            }
            else if(menuData.titleBar == TB_DISABLE) {
                menuData.titleBar = TB_2ENABLE;
                *(temp) = (char)1;
            }

            bool isOk = FunctionTools::replaceData2File(dataFileName, 2, 1, temp);

            delete[] temp;
        }

        tempX -= (spaceBetween + iconWidth);

        ImGui::SetCursorPos(ImVec2(tempX, 0));

        GLuint themeTexture;
        if(appData.uiTheme == DARK)
            themeTexture = moonBtnTexture;
        else
            themeTexture = sunBtnTexture;

        if(ImGui::ImageButton((void *)(intptr_t)themeTexture, ImVec2(iconSize, iconSize), ImVec2(0,0), ImVec2(1,1), imagePadding)) {

            appData.uiTheme = (appData.uiTheme == LIGHT) ? DARK : LIGHT;


            char* temp = new char[1];

            *(temp) = (char)(appData.uiTheme == LIGHT) ? 0 : 1;

            FunctionTools::replaceData2File(dataFileName, 1, 1, temp);

            delete[] temp;

        }



        ImGui::PopStyleColor();

        ImGui::EndMenuBar();
    }

    ImGui::PopStyleVar();


}


void Hud::statusBar() {

    ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;// | ImGuiWindowFlags_NoBackground;
    float height = ImGui::GetFrameHeight();

    if (ImGui::BeginViewportSideBar("##MainStatusBar", viewport, ImGuiDir_Down, height, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::Text("Happy status bar");
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
}


bool Hud::closeAppDialog(AppData &appFlags){
    bool closeApp = false;

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f,0.5f));


    if(ImGui::BeginPopupModal("Comfirm Close App", NULL, ImGuiWindowFlags_AlwaysAutoResize)){

        float imageSize = FunctionTools::norm2HeightFloat(40);

        ImGui::Image((void*)(intptr_t)questionTexture, ImVec2(imageSize, imageSize));
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::Text("Are you sure you want to close the App?");
        ImGui::Spacing();

        static bool dontAsk = false;

        ImGui::Checkbox("Don't ask again", &dontAsk);

        ImGui::EndGroup();

        ImGui::Separator();


        ImGuiStyle &style = ImGui::GetStyle();


        float posX = ImGui::GetWindowWidth() - style.ItemSpacing.x;
        posX -= (FunctionTools::norm2Height(180) + style.ItemSpacing.x);

        ImGui::SetCursorPosX(posX);

        int newXValue = FunctionTools::norm2Height(90);

        if(ImGui::Button("YES", ImVec2(newXValue, 0))){

            if(dontAsk == true && !appFlags.disableExitMessage){
                appFlags.disableExitMessage = true;

                char* temp = new char[1];
                *(temp) = (char)1;

                bool isOk = FunctionTools::replaceData2File(dataFileName, 0, 1, temp);

                delete[] temp;

            }


            ImGui::CloseCurrentPopup();
            closeApp = true;
        }


        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if(ImGui::Button("NO", ImVec2(newXValue, 0))){
            dontAsk = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();

    }

    return closeApp;
}


void Hud::update(const double &dt, const MenuData &menuFlags, AppData &appData, IOData &ioData, SerialManager *serialManager) {
    mainWin->update(dt, appData, ioData, serialManager);

    if(appData.appState == REWIND) {
        rewindSessionDialog(appData, ioData);
    }

}


void Hud::drawMainWin(const double &dt, AppData &appData, SerialManager *serialManager) {

    mainWin->draw(dt, appData, serialManager);

}

void Hud::rewindSessionDialog(AppData &appData, IOData &ioData){

    ImGui::OpenPopup("Rewind session");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f,0.5f));


    if(ImGui::BeginPopup("Rewind session", NULL)){

        float imageSize = FunctionTools::norm2HeightFloat(40);

        ImGui::Image((void*)(intptr_t)rewindTexture, ImVec2(imageSize, imageSize));
        ImGui::SameLine();
        ImGui::Spacing();
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::Text("Rewind to previous session?");
        ImGui::Spacing();

        for(std::string mystr : ioData.rewindPorts){
            ImGui::Text(mystr.c_str());
            ImGui::Spacing();
        }

        ImGui::EndGroup();

        ImGui::Separator();


        ImGuiStyle &style = ImGui::GetStyle();


        float posX = ImGui::GetWindowWidth() - style.ItemSpacing.x;
        posX -= (FunctionTools::norm2Height(180) + style.ItemSpacing.x);

        ImGui::SetCursorPosX(posX);

        int newXValue = FunctionTools::norm2Height(90);

        if(ImGui::Button("YES", ImVec2(newXValue, 0))){


            ImGui::CloseCurrentPopup();
        }


        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if(ImGui::Button("NO", ImVec2(newXValue, 0))){

            appData.appState = IDLE;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();

    }
}



Hud::~Hud() {

    delete questionSize;
    delete menuBarIconSize;
    delete cornerIconSize;

    delete mainWin;

    //SDL_DestroyTexture(qTexture);
    //qTexture = nullptr;
}







