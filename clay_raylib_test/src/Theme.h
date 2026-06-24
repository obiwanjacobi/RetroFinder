#pragma once

#include "project.h"
#include "ThemeColors.h"

class Theme {
public:
    Theme() : _font({0}) {}
    ~Theme() {
        if (IsFontValid(_font)) {
            UnloadFont(_font);
        }
    };

    Theme(const Theme&) = delete;
    Theme& operator=(const Theme&) = delete;
    Theme(Theme&&) = delete;
    Theme& operator=(Theme&&) = delete;

    // default background fill color
    const Clay_Color& GetBackgroundColor() const { return _backgroundColor; }
    void SetBackgroundColor(Clay_Color color) { _backgroundColor = color; }
    // default foreground color
    const Clay_Color& GetForegroundColor() const { return _foregroundColor; }
    void SetForegroundColor(Clay_Color color) { _foregroundColor = color; }
    // Draw focus to an screen element
    const Clay_Color& GetFocusColor() const { return _focusColor; }
    void SetFocusColor(Clay_Color color) { _focusColor = color; }

    Font* GetFontArray() { return &_font; } // for Clay
    const Font& GetFont() const { return _font; }
    void SetFont(const Font font) {
        if (IsFontValid(_font)) { UnloadFont(_font); }
        _font = font;
    }
    bool LoadFont(const char* fontPath, uint16_t fontSize, TextureFilter textureFilter = TextureFilter::TEXTURE_FILTER_POINT);
    
    void SetFontSize(uint16_t fontSize) { _fontSize = fontSize; }
    uint16_t GetFontSize() const { return _fontSize; }

    void SetFontSpacing(uint16_t fontSpacing) { _fontSpacing = fontSpacing; }
    uint16_t GetFontSpacing() const { return _fontSpacing; }

private:
    Clay_Color _backgroundColor = CLAY_COLOR_WHITE;
    Clay_Color _foregroundColor = CLAY_COLOR_BLACK;
    Clay_Color _focusColor = CLAY_COLOR_BLUE;
    Font _font;
    uint16_t _fontSize = 16;
    uint16_t _fontSpacing = 2;
};
