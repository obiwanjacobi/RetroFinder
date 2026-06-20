#pragma once
#include "project.h"
#include "Control.h"
#include "Theme.h"

class Label : public Control {
public:
    Label() : Label(CLAY_ID("label")) {}
    explicit Label(Clay_ElementId id) : Control(id) {}
    ~Label() = default;

    Label(const Label&) = delete;
    Label& operator=(const Label&) = delete;
    Label(Label&&) = delete;
    Label& operator=(Label&&) = delete;

protected:
    void DeclareContent(Theme* theme) override {
        CLAY_TEXT(_text, CLAY_TEXT_CONFIG({
            .userData = (void*)theme->GetFontArray(),
            .textColor = theme->GetForegroundColor(),
            .fontId = 0,
            .fontSize = theme->GetFontSize(),
            .letterSpacing = theme->GetFontSpacing(),
        }));
    }

public:
    void SetText(Clay_String text) { _text = text; }
    const Clay_String& GetText() const { return _text; }

private:
    Clay_String _text = CLAY_STRING("");
};
