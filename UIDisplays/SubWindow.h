//
// Created by castdata on 1/25/2023.
//

#ifndef TASTYSERIAL_SUBWINDOW_H
#define TASTYSERIAL_SUBWINDOW_H

#include <vector>
#include <string>
#include <iostream>
#include "imgui.h"
#include "DTypes.h"
#include "FunctionTools.h"
#include "TabSerialWindow.h"
#include "SerialManager.h"

class SubWindow {
public:
    explicit SubWindow(int &windowCount);
    explicit SubWindow(int &windowCount, TabSerialWindow *tabSerialWinPtr);
    void addTabPortConnection(SerialPortData *serialPortData, SerialConnection *serialConnection);
    void addTab(TabSerialWindow *tabSerialWindow);

    void update(const IOData &ioData, SerialManager *serialManager);

    void draw(AppData &appData, SerialManager *serialManager, const WinPos *windowPosPtr, const bool moreThanOne);

    std::string getWinName();
    void setWinSize(const float &w, const float &h);
    ImVec2 getWinSize();
    ImVec2 getWinPos();

    void setWidthPercentage(const float &percentage);
    float getWidthPercentage();

    void setFocused(bool focus);
    bool isFocused();

    TabSplitType isWindowSplitting();
    TabMoveType isWindowMoving();

    bool isClickOverWin(const ImVec2 &mousePos);
    bool isCursorOverBorder(const ImVec2 &mousePos);

    bool noTabs();
    bool anySelectedTab();
    SerialConnection* getSerialConnection();
    TabSerialWindow* getTabSerialWin();

    virtual ~SubWindow();
private:
    void setWinPos(const int &x, const int &y);
    void check4ClosedTabsRequest(SerialManager *serialManager);


    std::vector<TabSerialWindow*> tabSerialWindows;

    std::string windowName;
    ImVec2 winSize;
    ImVec2 winPos;
    float widthPercentage;
    int splitTabIndex;

    bool focused;
    int focusedIndex;

};


#endif //TASTYSERIAL_SUBWINDOW_H
