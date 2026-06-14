#pragma once
#include "raylib.h"
#include "Theme.h"

class RenderContext {
public:
    RenderContext(int fps = 30) { SetTargetFPS(fps); }
    ~RenderContext() = default;

    RenderContext(const RenderContext&) = delete;
    RenderContext& operator=(const RenderContext&) = delete;
    RenderContext(RenderContext&&) = delete;
    RenderContext& operator=(RenderContext&&) = delete;

    void SetTargetFPS(int fps) { ::SetTargetFPS(fps); }
    float GetFrameTime() const { return ::GetFrameTime(); }
    double GetTime() const { return ::GetTime(); }
    int GetFPS() const { return ::GetFPS(); }
    void WaitTime(double seconds) { ::WaitTime(seconds); }

    void SwapScreenBuffer() { ::SwapScreenBuffer(); }

    void ShowCursor() { ::ShowCursor(); }
    void HideCursor() { ::HideCursor(); }
    bool IsCursorHidden() const { return ::IsCursorHidden(); }
    void EnableCursor() { ::EnableCursor(); }
    void DisableCursor() { ::DisableCursor(); }
    bool IsCursorOnScreen() const { return ::IsCursorOnScreen(); }

    void ClearBackground(Color color) { ::ClearBackground(color); }
    void BeginRender(Theme* theme) { ::BeginDrawing(); UseTheme(theme); }
    void EndRender() { ::EndDrawing(); }

    void DrawText(const char *text, Vector2 position) {
        if (_theme) {
            ::DrawTextEx(_theme->GetFont(), text, position, _theme->GetFontSize(), _theme->GetFontSpacing(), _theme->GetForegroundColor());
        } else {
            ::DrawTextEx(GetFontDefault(), text, position, 16, 0, BLACK);
        }
    }

    void DrawText(const char *text, Vector2 position, int fontSize, Color color) { ::DrawText(text, position.x, position.y, fontSize, color); }
    void DrawText(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint) { ::DrawTextEx(font, text, position, fontSize, spacing, tint); }

    void UseTheme(Theme* theme) {
        _theme = theme;
        if (_theme) {
            ClearBackground(_theme->GetBackgroundColor());
        }
    }
private:
    Theme* _theme = nullptr;
};
