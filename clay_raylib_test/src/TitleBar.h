#pragma once
#include "project.h"
#include "Button.h"
#include "Label.h"
#include "Panel.h"
#include "PatternPanel.h"
#include "Spacer.h"
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

    void DeclareContent(Theme* theme) override;

    void SetTitle(Clay_String title) { _title.SetText(title); }
    Clay_String GetTitle() { return _title.GetText(); }
    
private:
    PatternPanel _panel;
    Button _leftButton;
    Button _rightButton;
    Spacer _leftGap;
    Spacer _rightGap;
    Label _title;
};
