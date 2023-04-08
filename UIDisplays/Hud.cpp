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

    ret = FunctionTools::loadTextureFromFile("../Assets/newBtn.png", &newBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/openBtn.png", &openBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/saveAsBtn.png", &saveAsBtnTexture, &imageWidth, &imageHeight);
    IM_ASSERT(ret);

    ret = FunctionTools::loadTextureFromFile("../Assets/saveBtn.png", &saveBtnTexture, &imageWidth, &imageHeight);
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




void Hud::menuBar(MenuData &menuData, AppData &appData, const IOData &ioData) {

    float framePadding = static_cast<float>(FunctionTools::norm2Height(10));

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
    if(ImGui::BeginMenuBar()){

        GLuint texture;

        if(appData.uiTheme == LIGHT)
            texture = menuDarkTexture;
        else
            texture = menuLightTexture;

        float iconSize = FunctionTools::norm2Height(static_cast<int>(menuBarIconSize->y));
        int imagePadding = FunctionTools::norm2Height(4);

        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_MenuBarBg));
        if(ImGui::ImageButton((void*)(intptr_t)texture, ImVec2(iconSize,iconSize), ImVec2(0,0), ImVec2(1,1), imagePadding)) {
            ImGui::OpenPopup("menuPopup");
        }

        ImGui::PopStyleColor();


        ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowViewport()->Pos.x + FunctionTools::norm2Height(8),ImGui::GetWindowViewport()->Pos.y + FunctionTools::norm2Height(31)));

        if(ImGui::BeginPopup("menuPopup")){

            if(appData.appState == MENU || appData.appState == IDLE)
                appData.appState = MENU;

            if(ImGui::BeginMenu("File")){

                ImGui::Spacing();

                int uIconSize = FunctionTools::norm2Height(11);

                ImGui::Image((void*)(intptr_t)uNewBtnTexture, ImVec2(uIconSize, uIconSize));
                ImGui::SameLine();

                if(ImGui::MenuItem("New", "Ctrl + n")){

                }
                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Image((void*)(intptr_t)uOpenBtnTexture, ImVec2(uIconSize, uIconSize));
                ImGui::SameLine();
                if(ImGui::MenuItem("Open", "Ctrl + o")){

                }
                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Image((void*)(intptr_t)uSaveBtnTexture, ImVec2(uIconSize, uIconSize));
                ImGui::SameLine();
                if(ImGui::MenuItem("Save", "Ctrl + s")){

                }
                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Image((void*)(intptr_t)uSaveAsBtnTexture, ImVec2(uIconSize, uIconSize));
                ImGui::SameLine();
                if(ImGui::MenuItem("Save As", "Ctrl + Alt + s")){

                }
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Image((void*)(intptr_t)uEmptyBtnTexture, ImVec2(uIconSize, uIconSize));
                ImGui::SameLine();
                if(ImGui::MenuItem("Exit", "Alt + F4")){
                    menuData.exitApp = true;
                }

                ImGui::EndMenu();
            }
            ImGui::Spacing();
            ImGui::Spacing();

            if(ImGui::BeginMenu("Edit")){
                if(ImGui::BeginMenu("Connections")){

                    if(ImGui::MenuItem("One")){

                    }
                    ImGui::Spacing();
                    ImGui::Spacing();
                    if(ImGui::MenuItem("Two")){

                    }
                    ImGui::Spacing();

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            ImGui::Spacing();
            ImGui::Spacing();

            if(ImGui::BeginMenu("Help")){
                if(ImGui::MenuItem("Help")){

                }
                ImGui::Spacing();
                ImGui::Spacing();
                if(ImGui::MenuItem("About")){

                }
                ImGui::EndMenu();
            }

            ImGui::Spacing();

            ImGui::EndPopup();
        }
        else{
            if(appData.appState == MENU)
                appData.appState = IDLE;
        }


        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_MenuBarBg));

        if(ImGui::ImageButton((void*)(intptr_t)newBtnTexture, ImVec2(iconSize,iconSize), ImVec2(0,0), ImVec2(1,1),imagePadding)){
            ImGui::OpenPopup("sergio");
        }

        if(ImGui::ImageButton((void*)(intptr_t)openBtnTexture, ImVec2(iconSize,iconSize), ImVec2(0,0), ImVec2(1,1), imagePadding)){

        }


        if(ImGui::ImageButton((void*)(intptr_t)saveBtnTexture, ImVec2(iconSize,iconSize), ImVec2(0,0), ImVec2(1,1), imagePadding)){

        }


        int spaceBetween = FunctionTools::norm2Height(8);
        float iconWidth = FunctionTools::norm2Height(static_cast<int>(cornerIconSize->x));//FunctionTools::norm2HeightFloat(38);
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
        bool barBtnState = false;

        if(menuData.titleBar == TB_ENABLE) {
            if (ImGui::ImageButton((void *) (intptr_t) withoutBarTexture, ImVec2(iconSize, iconSize), ImVec2(0, 0),
                                   ImVec2(1, 1), imagePadding))
                barBtnState = true;
        }
        else{
            if (ImGui::ImageButton((void *) (intptr_t) withBarTexture, ImVec2(iconSize, iconSize), ImVec2(0, 0),
                                   ImVec2(1, 1), imagePadding))
                barBtnState = true;
        }



        if(barBtnState){
            if(menuData.titleBar == TB_ENABLE)
                menuData.titleBar = TB_2DISABLE;
            else if(menuData.titleBar == TB_DISABLE)
                menuData.titleBar = TB_2ENABLE;
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

        if(dontAsk == true && !appFlags.disableExitMessage){
            appFlags.disableExitMessage = true;

            char* temp = new char[1];
            *(temp) = (char)1;

            bool isOk = FunctionTools::replaceData2File(dataFileName, 0, 1, temp);

            delete[] temp;

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
            closeApp = true;
        }


        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if(ImGui::Button("NO", ImVec2(newXValue, 0))){
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();

    }

    return closeApp;
}


void Hud::update(const MenuData &menuFlags, AppData &appData, const IOData &ioData, SerialManager *serialManager) {
    mainWin->update(appData, ioData, serialManager);
}


void Hud::drawMainWin(const double &dt, AppData &appData, SerialManager *serialManager) {

    mainWin->draw(dt, appData, serialManager);

}



Hud::~Hud() {

    delete questionSize;
    delete menuBarIconSize;
    delete cornerIconSize;

    delete mainWin;

    //SDL_DestroyTexture(qTexture);
    //qTexture = nullptr;
}







