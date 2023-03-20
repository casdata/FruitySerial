//
// Created by castdata on 1/27/2023.
//

#ifndef TASTYSERIAL_MAINWINDOW_H
#define TASTYSERIAL_MAINWINDOW_H

#include "FunctionTools.h"
#include "SubWindow.h"
#include "imgui.h"
#include "DTypes.h"
#include "SerialManager.h"
#include <vector>
#include <iostream>
#include <string>

class MainWindow {
public:
    explicit MainWindow();

    void update(AppData &appdata, const IOData &ioData, SerialManager *serialManager);

    void draw(AppData &appData, SerialManager *serialManager);

    virtual ~MainWindow();

private:

    int getFocusedSubWinIndex();
    void calculateSubWinSizes();

    void checkAndSplitWindow(int &windowCount);
    void checkClickAndFocus(const IOData &ioData, const AppState &appState);
    void checkAndResizeSubWindows(bool &cursorOverWinBorder, const IOData &ioData);

    void newSubWindow(int &windowCount, const int &subWindowIndex);


    std::vector<SubWindow*> subWindows;
    ImVec2 winSize;
    int resizingWindowIndex;


};


#endif //TASTYSERIAL_MAINWINDOW_H
