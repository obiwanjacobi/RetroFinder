#pragma once
#include "clay_renderer_raylib.h"

#include "Theme.h"
#include "Control.h"

#include <vector>

class Window {
public:
    Window(ConfigFlags configFlags);
    ~Window() { Clay_Raylib_Close(); }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    void AddControl(Control* control) { _controls.push_back(control); }

    bool ShouldClose() const { return ::WindowShouldClose(); }
    bool IsReady() const { return ::IsWindowReady(); }
    bool IsFullscreen() const { return ::IsWindowFullscreen(); }
    bool IsHidden() const { return ::IsWindowHidden(); }
    bool IsMinimized() const { return ::IsWindowMinimized(); }
    bool IsMaximized() const { return ::IsWindowMaximized(); }
    bool IsFocused() const { return ::IsWindowFocused(); }
    bool IsResized() const { return ::IsWindowResized(); }

    bool IsState(ConfigFlags flag) const { return ::IsWindowState(flag); }
    void SetState(ConfigFlags flags) { ::SetWindowState(flags); }
    void ClearState(ConfigFlags flags) { ::ClearWindowState(flags); }

    void Maximize() { ::MaximizeWindow(); }
    void Minimize() { ::MinimizeWindow(); }
    void Restore() { ::RestoreWindow(); }

    void SetIcon(Image image) { ::SetWindowIcon(image); }
    void SetIcons(Image *images, int count) { ::SetWindowIcons(images, count); }
    void SetTitle(const char *title) { ::SetWindowTitle(title); }
    void SetPosition(int x, int y) { ::SetWindowPosition(x, y); }

    void SetMinSize(int width, int height) { ::SetWindowMinSize(width, height); }
    void SetMaxSize(int width, int height) { ::SetWindowMaxSize(width, height); }
    void SetSize(int width, int height) { ::SetWindowSize(width, height); }
    void SetOpacity(float opacity) { ::SetWindowOpacity(opacity); }
    void SetFocused() { ::SetWindowFocused(); }
    void* GetHandle() { return ::GetWindowHandle(); }

    Vector2 GetPosition() const { return ::GetWindowPosition(); }
    Vector2 GetScaleDPI() const { return ::GetWindowScaleDPI(); }

    void Render();

    void SetTheme(Theme* theme) { _theme = theme; }
    Theme* GetTheme() const { return _theme; }
private:
    Theme* _theme = nullptr;
    std::vector<Control*> _controls;
};