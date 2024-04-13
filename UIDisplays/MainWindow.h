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
#include "UndoRedo.h"
#include "SendLog.h"
#include <vector>
#include <iostream>
#include <string>

class MainWindow {
public:
    explicit MainWindow();

    void update(const double &dt, AppData &appdata, const IOData &ioData, SerialManager *serialManager);

    void draw(const double &dt, AppData &appData, SerialManager *serialManager);

    virtual ~MainWindow();

private:

    int getFocusedSubWinIndex();
    void calculateSubWinSizes();

    void checkAndSplitWindow(int &windowCount);
    void checkClickAndFocus(const IOData &ioData, const AppState &appState);
    void checkAndResizeSubWindows(bool &cursorOverWinBorder, const IOData &ioData);
    void checkMouseInsideInputTextBar(AppData &appdata, const IOData &ioData);
    void checkInputTextBarIO(const double &dt, AppData &appdata, const IOData &ioData);

    void newSubWindow(int &windowCount, const int &subWindowIndex, TabSerialWindow *tabWindowPtr, TabSplitType tabSplitType);
    void moveSubWindow(const int &subWindowIndex, TabSerialWindow *tabWindowPtr, TabMoveType tabMoveType);
    void adjustSubWindowsWidth();

    void updateAndPrintInputBar(const double &dt, const AppData &appData);

    void calculateInputBarOffset(bool onlyIncrement);

    void deleteSelectedChars();

    enum UndoRedoState{IDLE_UR, NEW, UPDATE};

    std::vector<SubWindow*> subWindows;
    ImVec2 winSize;
    int resizingWindowIndex;

    ImVec2 inputBarPos;
    ImVec2 inputBarSize;
    bool inputBarEnabled;

    bool selectedInputText;
    bool onInputTextBar;
    bool showInputBarCaret;
    double caretCurrentTime;

    std::string inputTextBarBuffer;
    int iTextBarBufferPC;
    int iTextBarBufferPC2;
    float caretXPos;
    float iBarOffsetX;
    float selectedInputP0_x;
    float selectedInputP1_x;

    UndoRedo* undoRedo;
    SendLog* sendLog;

    GLuint lightSendTexture;
    GLuint darkSendTexture;
    ImVec2 sendTextureSize;

    GLuint darkBtn0Texture;
    GLuint darkBtn1Texture;
    GLuint darkBtn2Texture;
    GLuint darkBtn3Texture;

    GLuint lightBtn0Texture;
    GLuint lightBtn1Texture;
    GLuint lightBtn2Texture;
    GLuint lightBtn3Texture;

    GLuint sepTexture;
    GLuint infoTexture;

    bool inputBarInfoWindow;
    bool send2SerialEnter;

};


#endif //TASTYSERIAL_MAINWINDOW_H
