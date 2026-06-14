#pragma once

#include "raylib.h"
#include "clay.h"
#include "ThemeColors.h"

class Theme {
public:
    Theme() : _font({0}) {}
    ~Theme() {
        if (_font.baseSize) {
            UnloadFont(_font);
        }
    };

    Theme(const Theme&) = delete;
    Theme& operator=(const Theme&) = delete;
    Theme(Theme&&) = delete;
    Theme& operator=(Theme&&) = delete;

    const Clay_Color& GetBackgroundColor() const { return _backgroundColor; }
    void SetBackgroundColor(Clay_Color color) { _backgroundColor = color; }
    const Clay_Color& GetForegroundColor() const { return _foregroundColor; }
    void SetForegroundColor(Clay_Color color) { _foregroundColor = color; }

    Font* GetFontArray() { return &_font; } // for Clay
    const Font& GetFont() const { return _font; }
    void SetFont(const Font font) {
        if (IsFontValid(_font)) {
            UnloadFont(_font);
        }
        _font = font;
    }
    bool LoadFont(const char* fontPath, uint16_t fontSize);
    
    void SetFontSize(uint16_t fontSize) { _fontSize = fontSize; }
    uint16_t GetFontSize() const { return _fontSize; }

    void SetFontSpacing(uint16_t fontSpacing) { _fontSpacing = fontSpacing; }
    uint16_t GetFontSpacing() const { return _fontSpacing; }

private:
    Clay_Color _backgroundColor = CLAY_COLOR_YELLOW;
    Clay_Color _foregroundColor = CLAY_COLOR_BLACK;
    Font _font;
    uint16_t _fontSize = 16;
    uint16_t _fontSpacing = 2;
};
