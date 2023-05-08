//
// Created by castdata on 1/15/23.
//
#pragma once
#ifndef MASTERSERIALMONITOR_DTYPES_H
#define MASTERSERIALMONITOR_DTYPES_H

#include "serial/serial.h"

typedef enum {DARK, CLASSIC, LIGHT} UI_Theme;
typedef enum {OFF, DOWN, UP, ON} BtnState;
typedef enum {IDLE, SAVE, LOAD, MENU} AppState;
typedef enum {DISABLE, EN_TIME} TimeStamp;
typedef enum {TB_2DISABLE, TB_DISABLE, TB_2ENABLE, TB_ENABLE} TitleBar;
typedef enum {SET_2MAXIMIZE, MAXIMIZE, SET_2NORMAL, NORMAL} MaximizeState;

typedef enum {
    fivebits = 5,
    sixbits = 6,
    sevenbits = 7,
    eigthbits = 8
} serialByteSize;

typedef enum {
    parity_none = 0,
    parity_odd = 1,
    parity_even = 2,
    parityCount
} serialParity;

typedef enum {
    stopbits_one = 0,
    stopbits_one_point_five = 1,
    stopbits_two

} serialStopbits;

typedef enum {
    flowcontrol_none = 0,
    flowcontrol_software,
    flowcontrol_hardware
} serialFlowcontrol;

typedef enum {
    baud_300 = 0,
    baud_1200,
    baud_2400,
    baud_4800,
    baud_9600,
    baud_19200,
    baud_38400,
    baud_57600,
    baud_74880,
    baud_115200,
    baud_230400,
    baud_250000,
    baud_500000,
    baud_1000000,
    baud_2000000
} serialBaudrate;

typedef enum {
    noLine = 0,
    newLine,
    carrieReturn,
    crAndLf
} serialEndOfLine;

typedef enum {
    RAW_DEC = 0,
    RAW_HEX,
    UTF_8,
    UTF_8_RAW_DEC,
    UTF_8_RAW_HEX,
    UTF_8_SPECIAL,
    UTF_8_ESP_LOG
} TextEncoding;

typedef struct{
    bool exitApp;
    MaximizeState maximize;
    bool minimize;
    TitleBar titleBar;
} MenuData;

typedef struct{
    bool        disableExitMessage;
    int         windowCount;
    int         connectionCount;
    UI_Theme    uiTheme;
    int         windowPosX;
    int         windowPosY;
    int         windowSizeW;
    int         windowSizeH;
    bool        winNotMinimized;
    AppState    appState;
    bool        cursorOverSubWinBorder;
    bool        cursorOverInputTextBar;
    ImFont*     monoFont;
} AppData;

typedef struct{
    BtnState    mouseBtnRight;
    BtnState    mouseBtnLeft;
    BtnState    keyLeft;
    BtnState    keyRight;
    BtnState    keyUp;
    BtnState    keyDown;
    BtnState    keyL_Ctrl;
    BtnState    keyR_Ctrl;
    BtnState    keyL_Shift;
    BtnState    keyR_Shift;
    BtnState    keyL_Alt;
    BtnState    keyR_Alt;
    BtnState    keyTab;
    BtnState    keyBackspace;
    BtnState    keyDel;
    BtnState    keyEnter;
    BtnState    keyEsc;
    BtnState    keyHome;
    BtnState    keyEnd;
    BtnState    keyCopy;
    BtnState    keyPaste;
    BtnState    keyCut;
    BtnState    keyUndo;
    BtnState    keyRedo;
    std::string charBuffer;
    std::string copyBuffer;
    ImVec2      mouseCursorPosition;
    ImVec2      mouseCursorPositionRaw;
} IOData;


typedef struct{
    serial::PortInfo    portInfo;
    bool                used;
    bool                enable;
    bool                updateIt;
} SerialPortData;

typedef struct{
    BtnState    mouseBtnLeft;
    int         windowSizeW;
    int         windowSizeH;
} HitTestData;

typedef struct{
    std::string     dataLine;
    float           caretPos;
    int             posIndex;
} UndoRedoData;

typedef struct{
    std::string str;
    bool        simpleTxt;
    int         mainStrIndex;
} FormattedInputStr;




const char* const dataFileName = "data.bin";

static const float LEGACY_WIDTH = 1920.0f;
static const float LEGACY_HEIGHT = 1080.0f;

static const int MENU_BAR_HIGHT = 32;
static const int RESIZE_OFFSET = 10;

static const ImU32 DARK_WINDOW_BG_COL       = IM_COL32(0,0,0,255);
static const ImU32 LIGHT_WINDOW_BG_COL      = IM_COL32(255,255,255,255);

static const ImU32 DARK_INPUT_BG_COL        = IM_COL32(29, 47, 73, 255);
static const ImU32 LIGHT_INPUT_BG_COL       = LIGHT_WINDOW_BG_COL;

static const ImU32 DARK_BRACKET_COL         = IM_COL32(0, 255, 255, 255);
static const ImU32 DARK_SPECIAL_UTF8_COL    = IM_COL32(255, 0, 0, 255);

static const ImU32 LIGHT_BRACKET_COL        = IM_COL32(0, 100, 255, 255);
static const ImU32 LIGHT_SPECIAL_UTF8_COL   = IM_COL32(255, 0, 0, 255);

static const ImU32 DARK_COMBO_BG_COL        = IM_COL32(61, 61, 61, 255);
static const ImU32 DARK_COMBO_BG_OVER_COL   = IM_COL32(77, 77, 77, 255);
static const ImU32 DARK_COMBO_BG_PRESS_COL  = IM_COL32(106, 106, 106, 255);

static const ImU32 LIGHT_COMBO_BG_COL       = IM_COL32(215, 215, 215, 255);
static const ImU32 LIGHT_COMBO_BG_OVER_COL  = IM_COL32(137, 137, 137, 255);
static const ImU32 LIGHT_COMBO_BG_PRESS_COL = IM_COL32(168, 168, 168, 255);

static const ImU32 DARK_ESP_LOG_W_COL       = IM_COL32(249, 249, 0, 255);
static const ImU32 DARK_ESP_LOG_E_COL       = IM_COL32(255, 73, 89, 255);
static const ImU32 DARK_ESP_LOG_I_COL       = IM_COL32(13, 229, 99, 255);
static const ImU32 DARK_ESP_LOG_D_COL       = IM_COL32(0, 130, 255, 255);
static const ImU32 DARK_ESP_LOG_V_COL       = IM_COL32(148, 68, 255, 255);

static const ImU32 LIGHT_ESP_LOG_W_COL      = IM_COL32(215, 180, 0, 255);
static const ImU32 LIGHT_ESP_LOG_E_COL      = IM_COL32(255, 73, 45, 255);
static const ImU32 LIGHT_ESP_LOG_I_COL      = IM_COL32(13, 170, 99, 255);
static const ImU32 LIGHT_ESP_LOG_D_COL      = IM_COL32(0, 0, 223, 255);
static const ImU32 LIGHT_ESP_LOG_V_COL      = IM_COL32(148, 0, 223, 255);

static const ImU32 CLOSE_BTN_OVER           = IM_COL32(231, 28, 28, 255);
static const ImU32 CLOSE_BTN_PRESSED        = IM_COL32(214, 98, 98, 255);
static const ImU32 CORNER_BTN_OVER          = IM_COL32(221, 221, 221, 255);

static const ImU32 DARK_TEXT_SELECT         = IM_COL32(42, 83, 135, 255);
static const ImU32 LIGHT_TEXT_SELECT        = IM_COL32(189, 218, 253, 255);

//static const ImU32 DARK_SPECIAL_INPUT       = IM_COL32(255, 157, 5, 255);
//static const ImU32 LIGHT_SPECIAL_INPUT      = IM_COL32(255, 114, 5, 255);

static const ImU32 DARK_SPECIAL_INPUT       = IM_COL32(232, 155, 68, 255);
static const ImU32 LIGHT_SPECIAL_INPUT      = IM_COL32(255, 105, 72, 255);

static const char* TXT_ENCODING_ITEMS[]     = {"RAW-DEC", "RAW-HEX", "UTF-8", "UTF-8 RAW-DEC", "UTF-8 RAW-HEX", "UTF-8 SPECIAL", "UTF-8 ESP-LOG"};

static const char* BAUDRATE_ITEMS[]         = {"300 baud","1200 baud", "2400 baud", "4800 baud", "9600 baud", "19200 baud",
                                            "38400 baud", "57600 baud", "74880 baud", "115200 baud", "230400 baud",
                                            "250000 baud", "500000 baud", "1000000 baud", "2000000 baud"};
static const char* BYTE_SIZE_ITEMS[]         = {"5", "6", "7", "8"};
static const char* PARITY_ITEMS[]           = {"none", "odd", "even"};
static const char* STOP_BITS_ITEMS[]        = {"1", "1.5", "2"};
static const char* FLOW_CONTROL_ITEMS[]     = {"none", "software", "hardware"};

//static const char* EOL_ITEMS[]              = {"no line", "new line", "carriage return", "cr & lf"};
static const char* EOL_ITEMS[]              = {"0: no line", "1: LF - new line", "2: CR - carriage return", "3: CR + LF"};

/*
 * noLine,
    newLine,
    carrieReturn,
    crAndLf
 */
/*
 * RAW_DEC = 0,
    RAW_HEX,
    UTF_8,
    UTF_8_RAW_DEC,
    UTF_8_RAW_HEX,
    UTF_8_SPECIAL,
    UTF_8_ESP_LOG
 */


#endif //MASTERSERIALMONITOR_DTYPES_H
