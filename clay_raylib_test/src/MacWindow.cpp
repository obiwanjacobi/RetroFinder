#include "MacWindow.h"

MacWindow::MacWindow() : Window(ConfigFlags::FLAG_WINDOW_UNDECORATED), _screen(CLAY_ID("client")), _titleBar(CLAY_ID("titlebar")) {
    
    _titleBar.SetTitle(CLAY_STRING("Window Title"));
    _screen.Add(&_titleBar);
    Window::Screen().Add(&_screen);   
}
void MacWindow::Declare(Theme* theme)
{
    _titleBar.Layout().sizing = { .width = CLAY_SIZING_GROW(0) };

    _screen.Layout() = Clay_LayoutConfig { 
        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
        .padding = { 16, 16, 16, 16 }, 
        .childGap = 16,
        .childAlignment = { .x = CLAY_ALIGN_X_LEFT },
        .layoutDirection = CLAY_TOP_TO_BOTTOM
    };

    Window::Declare(theme);
}
