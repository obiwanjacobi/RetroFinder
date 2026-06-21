#pragma once
#include "project.h"
#include "PatternPanel.h"
#include "ScrollView.h"
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

    ScrollView& Screen() { return _screen; }

    void SetMinSize(int width, int height) { _minWidth = width; _minHeight = height; }

    void Declare(Theme* theme) override;
    
private:
    ScrollView _screen;
    TitleBar _titleBar;
    int _minWidth = 300;
    int _minHeight = 200;
};
