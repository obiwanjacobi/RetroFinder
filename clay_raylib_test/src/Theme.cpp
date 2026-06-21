#include "Theme.h"

bool Theme::LoadFont(const char* fontPath, uint16_t fontSize, TextureFilter textureFilter)
{
    if (fontSize = 0) { return false; }
    Font font = LoadFontEx(fontPath, fontSize, nullptr, 0);
    if (!IsFontValid(font)) { return false; }
    SetTextureFilter(font.texture, textureFilter);
    SetFont(font);
    SetFontSize(fontSize);
    return true;
}