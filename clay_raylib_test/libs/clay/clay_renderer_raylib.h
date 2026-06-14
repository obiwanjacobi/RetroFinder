#pragma once
#include "raylib.h"
#include "clay.h"

#ifdef __cplusplus
extern "C" {
#endif

Clay_Dimensions Raylib_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);

void Clay_Raylib_Initialize(int width, int height, const char *title, unsigned int flags);
void Clay_Raylib_Close();

void Clay_Raylib_Render(Clay_RenderCommandArray renderCommands, Font* fonts);

inline Clay_StringSlice ClayStringToSlice(Clay_String str) {
    return Clay_StringSlice{ str.length, str.chars, str.chars };
}
const char* ClayStringToCString(Clay_StringSlice str);
Color ClayColorToRaylibColor(Clay_Color color);

#ifdef __cplusplus
}
#endif