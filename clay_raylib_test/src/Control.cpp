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
            .clip = Clay_ClipElementConfig {
                .horizontal = style.clip.horizontal,
                .vertical = style.clip.vertical,
                .childOffset = (style.clip.horizontal || style.clip.vertical) ? Clay_GetScrollOffset() : style.clip.childOffset,
            },
            .border = style.border,
        }) {
        DeclareContent(theme);
    }
}
