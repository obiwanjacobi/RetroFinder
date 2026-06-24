#include "MenuItem.h"

void MenuItem::Prepare(Theme* theme, BoxStyle& style)
{
    Control::Prepare(theme, style);

    if (_enabled && IsPointerOver()) {
        style.backgroundColor = theme->GetForegroundColor();
    }

    style.border = {};
}

void MenuItem::DeclareContent(Theme* theme)
{
    Layout().layoutDirection = CLAY_LEFT_TO_RIGHT;
    Layout().childGap = 56;
    Layout().padding = { .left = 12, .right = 12, .top = 12, .bottom = 12 };
    Layout().childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER };

    if (_enabled && _onClick) {
        Clay_OnHover(OnHoverCallback, this);
    }

    Clay_Color textColor = theme->GetForegroundColor();
    if (!_enabled) { textColor = CLAY_COLOR_GRAY_600; }
    else if (IsPointerOver()) { textColor = theme->GetBackgroundColor(); }

    CLAY_TEXT(_text, CLAY_TEXT_CONFIG({
        .userData = (void*)theme->GetFontArray(),
        .textColor = textColor,
        .fontId = 0,
        .fontSize = theme->GetFontSize(),
        .letterSpacing = theme->GetFontSpacing(),
    }));

    if (_shortcutText.length > 0) {
        CLAY_TEXT(_shortcutText, CLAY_TEXT_CONFIG({
            .userData = (void*)theme->GetFontArray(),
            .textColor = textColor,
            .fontId = 0,
            .fontSize = theme->GetFontSize(),
            .letterSpacing = theme->GetFontSpacing(),
        }));
    }
}
