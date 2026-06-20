#include "Panel.h"

void Panel::Prepare(Theme* theme, BoxStyle& style) {
    ContainerControl::Prepare(theme, style);
    
    if (!HasBackgroundColor()) {
        style.backgroundColor = theme->GetBackgroundColor();
    }
    // if (!HasBorder()) {
    //     style.border = Clay_BorderElementConfig { .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(5) };
    // }
}
