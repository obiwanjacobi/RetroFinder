#include "MacWindow.h"

#include <cmath>

MacWindow::MacWindow() : Window(ConfigFlags::FLAG_WINDOW_UNDECORATED), 
    _screen(CLAY_ID("client")), _titleBar(CLAY_ID("titlebar"))
{
    _titleBar.SetTitle(CLAY_STRING("Window Title"));
    // _titleBar.SetOnMinimize([this]() {
    //     Minimize();
    // });
    _titleBar.SetOnClose([this]() {
        Close();
    });
    _titleBar.SetOnDrag([this](Vector2 delta) {
        Vector2 pos = GetPosition();
        SetPosition((int)roundf(pos.x + delta.x), (int)roundf(pos.y + delta.y));
    });
    
    Window::Screen().Add(&_titleBar);
    Window::Screen().Add(&_screen);
}

void MacWindow::Declare(Theme* theme)
{
    _titleBar.Layout().sizing = { .width = CLAY_SIZING_GROW(0) };

    _screen.Layout() = Clay_LayoutConfig { 
        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
        .childAlignment = { .x = CLAY_ALIGN_X_LEFT },
        .layoutDirection = CLAY_TOP_TO_BOTTOM
    };

    Window::Declare(theme);
}
