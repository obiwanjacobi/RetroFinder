#include "Control.h"

void Control::Prepare(Theme* theme, BoxStyle& style)
{
    if (_hasBackgroundColor) {
        style.backgroundColor = _backgroundColor;
    } else {
        style.backgroundColor = theme->GetBackgroundColor();
    }
    if (_hasBorder) {
        style.border = _border;
    }
}

void Control::Declare(Theme* theme) {
    BoxStyle style = {};
    Prepare(theme, style);

    CLAY(_id, {
            .layout = _layout,
            .backgroundColor = style.backgroundColor,
            .overlayColor = style.overlayColor,
            .cornerRadius = style.cornerRadius,
            .aspectRatio = style.aspectRatio,
            .image = style.image,
            .floating = style.floating,
            .custom = style.custom,
            .clip = style.clip,
            .border = style.border,
        }) {
        DeclareContent(theme);
    }
}
