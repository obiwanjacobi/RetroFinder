#include "IconButton.h"

void IconButton::SetIcon(IconType icon, uint16_t logicalSize, uint16_t pixelScale) {
    _icon.SetIcon(icon, logicalSize, pixelScale);
}

void IconButton::ClearIcon() {
    _icon.ClearIcon();
}

void IconButton::DeclareContent(Theme* theme) {
    if (_icon.HasIcon()) {
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER }
            }
        }) {
            const float iconSize = _icon.GetPixelSize();
            _icon.Layout().sizing = {
                .width = CLAY_SIZING_FIXED(iconSize),
                .height = CLAY_SIZING_FIXED(iconSize)
            };
            _icon.Declare(theme);
        }
        return;
    }

    Button::DeclareContent(theme);
}