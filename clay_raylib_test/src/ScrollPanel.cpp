#include "ScrollPanel.h"

void ScrollPanel::Prepare(Theme* theme, BoxStyle& style) {
    ContainerControl::Prepare(theme, style);
    style.clip = { .horizontal = _scrollHorizontal, .vertical = _scrollVertical };
    
    // Set default layout direction to top-to-bottom if not already set
    if (Layout().layoutDirection == 0) {
        Layout().layoutDirection = CLAY_TOP_TO_BOTTOM;
    }
}
