#include "TitleBar.h"

TitleBar::TitleBar(Clay_ElementId id) : Panel(id), _panel(CLAY_ID("titlebar_pattern")) {
    _panel.Add(&_title);
    Add(&_panel);
}

void TitleBar::Declare(Theme* theme) {

    Layout().padding = { .left = 40, .right = 40 };

    _panel.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(90) };
    _panel.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    _panel.Layout().layoutDirection = CLAY_LEFT_TO_RIGHT;
    _panel.SetPatternStyle(PatternPanel::PatternStyle::HorizontalLines);
    _panel.SetTileSize(16);

    Panel::Declare(theme);
}
