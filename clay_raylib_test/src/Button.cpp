#include "Button.h"

void Button::DeclareContent(Theme* theme) {
    Border() = { .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(3) };
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