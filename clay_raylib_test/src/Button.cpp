#include "Button.h"

void Button::Prepare(Theme* theme, BoxStyle& style) {
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

void Button::DeclareContent(Theme* theme) {
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