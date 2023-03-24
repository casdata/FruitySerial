//
// Created by castdata on 1/18/23.
//
#ifndef MASTERSERIALMONITOR_FUNCTIONTOOLS_H
#define MASTERSERIALMONITOR_FUNCTIONTOOLS_H


#include <fstream>
#include <iostream>
#include "stb_image.h"
#include "imgui.h"
#include "DTypes.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif


class FunctionTools {
public:
    static bool loadTextureFromFile(const char* filename, GLuint* outTexture, int* outWidth, int* outHeight);
    static bool replaceData2File(const char* fileName, const int position, const int bytesSize, const char* data);
    static void imGuiAlignForWidth(const float width, const float alignment = 0.5f);
    static ImVec2 getMouseRealPos(const int &windowPosX, const int &windowPosY);
    static int getRealValue(const int &rawValue, const int &referenceValue);
    static bool btnStateInsideArea(BtnState &&btnState, const BtnState &mouseBtn, const ImVec2 &cursorPos, ImVec2 &&areaXY, ImVec2 &&areaHW);
    static uint32_t baudrateEnum2int(serialBaudrate sBaudrate);
    static serialBaudrate int2BaudrateEnum(uint32_t baudrate);
    static std::string textEncodingEnum2charptr(TextEncoding tEncoding);
    static TextEncoding int2TextEncodingEnum(uint32_t tEncodingIndex);
    static void char2Utf8(char character, std::string& str);
    static void unicode2Utf8(unsigned int unicode, std::string& str);
    static void printSpecialUTF8(std::string&& name, const UI_Theme& uiTheme, bool& postBuffActive, std::string& postBuff);
    static float norm2HeightFloat(const int &value, bool setLegacyHeight = false, int newHeight = 0);
    static int norm2Width(const int &value, bool setLegacyWidth = false, int newWidth = 0);
    static int norm2Height(const int &value, bool setLegacyHeight = false, int newHeight = 0);
};


#endif //MASTERSERIALMONITOR_FUNCTIONTOOLS_H
