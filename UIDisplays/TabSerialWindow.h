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
    inline static GLuint settingsTexture;
    inline static GLuint separatorTexture;
    inline static ImVec2 textureSize;

    explicit TabSerialWindow(SerialPortData *serialPortData, SerialConnection *serialConnection);

    std::string getTabName();

    bool isTabSelected();
    bool isTabClosing();
    bool isTabSplitting();
    bool isUndocking();
    bool isRenaming();

    void freePort();

    void draw(bool multiTabs, ImFont* monoFont, const UI_Theme& uiTheme);

    void add2DataBuffer(const std::string &dataText);
    void clearDataBuffer();

private:
    void drawPopupTabMenu(bool multiTabs);
    void drawTopBar(const UI_Theme& uiTheme);

    std::string tabName;

    bool tabSelected;
    bool open;
    bool splitRight;
    bool undock;
    bool rename;

    bool serialSettings;

    SerialPortData *tabSerialPortData;
    SerialConnection *serialConnection;

    ImVector<std::string> dataBuffer;
};


#endif //TASTYSERIAL_TABSERIALWINDOW_H
