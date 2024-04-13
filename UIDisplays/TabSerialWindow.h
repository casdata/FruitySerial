//
// Created by castdata on 1/28/2023.
//

#ifndef TASTYSERIAL_TABSERIALWINDOW_H
#define TASTYSERIAL_TABSERIALWINDOW_H

#include <vector>
#include <string>
#include <iostream>
#include "imgui.h"
#include "DTypes.h"
#include "FunctionTools.h"
#include "SerialConnection.h"

class TabSerialWindow {
public:
    inline static GLuint playTexture;
    inline static GLuint pauseTexture;
    inline static GLuint timestampTexture;
    inline static GLuint timestampDisLightTexture;
    inline static GLuint timestampDisDarkTexture;
    inline static GLuint fruitsTexture;
    inline static GLuint fruitsDisLightTexture;
    inline static GLuint fruitsDisDarkTexture;
    inline static GLuint settingsTexture;
    inline static GLuint separatorTexture;

    inline static GLuint darkBtn0RedTexture;
    inline static GLuint darkBtn1RedTexture;
    inline static GLuint darkBtn2RedTexture;
    inline static GLuint darkBtn3RedTexture;

    inline static GLuint lightBtn0RedTexture;
    inline static GLuint lightBtn1RedTexture;
    inline static GLuint lightBtn2RedTexture;
    inline static GLuint lightBtn3RedTexture;

    inline static ImVec2 textureSize;

    explicit TabSerialWindow(SerialPortData *serialPortData, SerialConnection *serialConnection);

    std::string getTabName();

    bool isTabSelected();
    bool isTabClosing();
    TabSplitType isTabSplitting();
    TabMoveType isTabMoving();
    bool isUndocking();
    bool need2Rename();

    void freePort();

    void draw(bool multiTabs, ImFont* monoFont, const UI_Theme& uiTheme, const WinPos *winPosPtr, const bool moreThanOne);

    void add2DataBuffer(const std::string &dataText);
    void clearDataBuffer();

    SerialConnection* getSerialConnectionPtr();

private:
    void drawPopupTabMenu(const UI_Theme &uiTheme, bool multiTabs, const WinPos *winPosPtr, const bool moreThanOneWin);
    void drawTopBar(const UI_Theme& uiTheme);

    std::string tabName;

    bool tabSelected;
    bool open;
    TabSplitType splitType;
    TabMoveType moveType;
    bool undock;
    bool rename;

    bool serialSettings;

    SerialPortData *tabSerialPortData;
    SerialConnection *serialConnection;

    ImVector<std::string> dataBuffer;
};


#endif //TASTYSERIAL_TABSERIALWINDOW_H
