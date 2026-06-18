#include "Panel.h"

void Panel::Declare(Theme* theme) {
    CLAY(_id, { 
            .layout = _layout, 
            .backgroundColor = theme->GetBackgroundColor(),
            .border = { .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(5) },
        }) {
        for (auto* control : _controls)
            control->Declare(theme);
    }
}
