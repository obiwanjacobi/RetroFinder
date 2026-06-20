#pragma once

#include "project.h"
#include "Control.h"
#include "Theme.h"

#include <functional>

class Button : public Control {
public:
    using ClickHandler = std::function<void()>;

    Button() : Button(CLAY_ID("button")) {}
    explicit Button(Clay_ElementId id) : Control(id) {}
    ~Button() override = default;

    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    Button(Button&&) = delete;
    Button& operator=(Button&&) = delete;

    void SetText(Clay_String text) { _text = text; }
    const Clay_String& GetText() const { return _text; }

    void SetOnClick(ClickHandler handler) { _onClick = std::move(handler); }
    void ClearOnClick() { _onClick = nullptr; }

protected:
    void Prepare(Theme* theme, BoxStyle& style) override {
        Control::Prepare(theme, style);

        if (!HasBackgroundColor()) {
            style.backgroundColor = theme->GetBackgroundColor();
        }
        if (!HasBorder()) {
            style.border = Clay_BorderElementConfig {
                .color = theme->GetForegroundColor(),
                .width = CLAY_BORDER_OUTSIDE(3)
            };
        }
    }

    void DeclareContent(Theme* theme) override {
        if (_onClick) {
            Clay_OnHover(OnHoverCallback, this);
        }
        CLAY_TEXT(_text, CLAY_TEXT_CONFIG({
            .userData = (void*)theme->GetFontArray(),
            .textColor = theme->GetForegroundColor(),
            .fontId = 0,
            .fontSize = theme->GetFontSize(),
            .letterSpacing = theme->GetFontSpacing(),
        }));
    }

private:
    static void OnHoverCallback(Clay_ElementId, Clay_PointerData pointerData, void* userData) {
        if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
            static_cast<Button*>(userData)->_onClick();
        }
    }

    Clay_String _text = CLAY_STRING("");
    ClickHandler _onClick;
};