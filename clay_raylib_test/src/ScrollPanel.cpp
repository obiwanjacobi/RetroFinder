#include "ScrollPanel.h"

void ScrollPanel::Prepare(Theme* theme, BoxStyle& style) {
    ContainerControl::Prepare(theme, style);
    style.clip = { .horizontal = _scrollHorizontal, .vertical = _scrollVertical };
}
