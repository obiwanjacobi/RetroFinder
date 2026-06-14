#pragma once
#include "raylib.h"
#include "Control.h"
#include "Theme.h"

class Label : public Control{
public:
    Label() = default;
    ~Label() = default;

    Label(const Label&) = delete;
    Label& operator=(const Label&) = delete;
    Label(Label&&) = delete;
    Label& operator=(Label&&) = delete;

    void Layout(Theme* theme) override {
        CLAY_TEXT(_text, CLAY_TEXT_CONFIG({
            .userData = (void*)theme->GetFontArray(),
            .textColor = theme->GetForegroundColor(),
            .fontId = 0,
            .fontSize = theme->GetFontSize(),
            .letterSpacing = theme->GetFontSpacing(),
        }));
    }

    void SetText(Clay_String text) { _text = text; }
    const Clay_String& GetText() const { return _text; }

private:
    Clay_String _text;
};
