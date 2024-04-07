//
// Created by castdata on 1/15/23.
//

#ifndef MASTERSERIALMONITOR_HUD_H
#define MASTERSERIALMONITOR_HUD_H

#include <iostream>
#include <SDL.h>
//#include <SDL_image.h>
#include "imgui.h"
#include "DTypes.h"
#include "FunctionTools.h"
#include "MainWindow.h"
#include "SerialManager.h"
#include <fstream>
#include <vector>

//#include "imgui_impl_sdl.h"
//#include "imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif


class Hud {

public:
    explicit Hud();

    void menuBar(MenuData &menuData, AppData &appData, IOData &ioData);
    void statusBar(const double *deltaTime);
    bool closeAppDialog(AppData &appFlags);

    void update(const double &dt, const MenuData &menuFlags, AppData &appData, IOData &ioData, SerialManager *serialManager);

    void drawMainWin(const double &dt, AppData &appData, SerialManager *serialManager);

    virtual ~Hud();

private:
    void rewindSessionDialog(AppData &appData, IOData &ioData);

    MainWindow* mainWin;

    GLuint questionTexture = 0;
    GLuint menuDarkTexture = 0;
    GLuint menuLightTexture = 0;
    GLuint newBtnTexture = 0;
    GLuint openBtnTexture = 0;
    GLuint saveAsBtnTexture = 0;
    GLuint saveBtnTexture = 0;
    GLuint rewindTexture = 0;
    GLuint sunBtnTexture = 0;
    GLuint moonBtnTexture = 0;
    GLuint withBarTexture = 0;
    GLuint withoutBarTexture = 0;
    GLuint minimizeTexture = 0;
    GLuint maximizeTexture = 0;
    GLuint maximize2Texture = 0;
    GLuint closeTexture = 0;
    GLuint iconTexture = 0;

    GLuint uNewBtnTexture = 0;
    GLuint uOpenBtnTexture = 0;
    GLuint uSaveAsBtnTexture = 0;
    GLuint uSaveBtnTexture = 0;
    GLuint uEmptyBtnTexture = 0;

    ImVec2* questionSize;
    ImVec2* menuBarIconSize;
    ImVec2* cornerIconSize;
    //ImVec2*
    //SDL_Texture* qTexture = nullptr;
};


#endif //MASTERSERIALMONITOR_HUD_H
