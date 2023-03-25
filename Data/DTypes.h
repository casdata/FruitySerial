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
    parity_mark = 3,
    parity_space = 4
} serialParity;

typedef enum {
    stopbits_one = 1,
    stopbits_two = 2,
    stopbits_one_point_five
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
    noLine,
    newLine,
    carrieReturn,
    crAndLf
} serialEndOfLine;

typedef enum {
    RAW_DEC = 0,
    RAW_HEX,
    UTF_8_RAW_DEC,
    UTF_8_RAW_HEX,
    UTF_8,
    UTF_8_SPECIAL,
    UTF_8_ESP_LOG
} TextEncoding;

typedef struct{
    bool exitApp;
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
    AppState    appState;
    bool        cursorOverSubWinBorder;
    ImFont*     monoFont;
} AppData;

typedef struct{
    BtnState    mouseBtnRight;
    BtnState    mouseBtnLeft;
    ImVec2      mouseCursorPosition;
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


const char* const dataFileName = "data.bin";

static const float LEGACY_WIDTH = 1920.0f;
static const float LEGACY_HEIGHT = 1080.0f;

static const int MENU_BAR_HIGHT = 32;
static const int RESIZE_OFFSET = 10;

static const ImU32 DARK_WINDOW_BG_COL       = IM_COL32(0,0,0,255);
static const ImU32 LIGHT_WINDOW_BG_COL      = IM_COL32(255,255,255,255);

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

static const ImU32 DARK_ESP_LOG_W_COL            = IM_COL32(249, 249, 0, 255);
static const ImU32 DARK_ESP_LOG_E_COL            = IM_COL32(255, 73, 89, 255);
static const ImU32 DARK_ESP_LOG_I_COL            = IM_COL32(13, 229, 99, 255);

static const ImU32 LIGHT_ESP_LOG_W_COL            = IM_COL32(215, 180, 0, 255);
static const ImU32 LIGHT_ESP_LOG_E_COL            = IM_COL32(255, 73, 45, 255);
static const ImU32 LIGHT_ESP_LOG_I_COL            = IM_COL32(13, 170, 99, 255);

static const ImU32 BTN_DOWN                 = IM_COL32(0, 100, 179, 255);

static const char* BAUDRATE_ITEMS[] = {"300 baud","1200 baud", "2400 baud", "4800 baud", "9600 baud", "19200 baud",
                                            "38400 baud", "57600 baud", "74880 baud", "115200 baud", "230400 baud",
                                            "250000 baud", "500000 baud", "1000000 baud", "2000000 baud"};


static const char* TXT_ENCODING_ITEMS[] = {"RAW-DEC", "RAW-HEX", "UTF-8 RAW-DEC", "UTF-8 RAW-HEX",
                                           "UTF-8", "UTF-8 RULES", "UTF-8 ESP-LOG"};
/*
 * RAW_DEC = 0,
    RAW_HEX,
    UTF_8_RAW_DEC,
    UTF_8_RAW_HEX,
    UTF_8,
    UTF_8_SPECIAL,
    UTF_8_ESP_LOG
 */


#endif //MASTERSERIALMONITOR_DTYPES_H
