#pragma once
#include "project.h"
#include "Label.h"
#include "Panel.h"
#include "PatternPanel.h"
#include "Theme.h"

class TitleBar : public Panel {
public:
    TitleBar() : TitleBar(CLAY_ID("titlebar")) {}
    TitleBar(Clay_ElementId id);
    ~TitleBar() = default;

    TitleBar(const TitleBar&) = delete;
    TitleBar& operator=(const TitleBar&) = delete;
    TitleBar(TitleBar&&) = delete;
    TitleBar& operator=(TitleBar&&) = delete;

    void Declare(Theme* theme) override;

private:
    PatternPanel _panel;
    Label _title;
};
