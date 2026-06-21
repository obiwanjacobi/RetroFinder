#pragma once
#include "project.h"
#include "PatternPanel.h"
#include "ScrollPanel.h"
#include "TitleBar.h"
#include "Window.h"

class MacWindow : public Window
{
public:
    MacWindow();
    ~MacWindow() = default;

    MacWindow(const MacWindow&) = delete;
    MacWindow& operator=(const MacWindow&) = delete;
    MacWindow(MacWindow&&) = delete;
    MacWindow& operator=(MacWindow&&) = delete;

    ScrollPanel& Screen() { return _screen; }

    void Declare(Theme* theme) override;
    
private:
    ScrollPanel _screen;
    TitleBar _titleBar;
};
