#include "TitleBar.h"

TitleBar::TitleBar(Clay_ElementId id)
        : Panel(id),
            _panel(CLAY_ID("titlebar_pattern")),
            _leftButton(CLAY_ID("titlebar_left_button")),
            _rightButton(CLAY_ID("titlebar_right_button")),
            _leftGap(CLAY_ID("titlebar_left_gap")),
            _rightGap(CLAY_ID("titlebar_right_gap")),
            _title(CLAY_ID("titlebar_title")) {
    _panel.Add(&_leftButton);
    _panel.Add(&_leftGap);
    _panel.Add(&_title);
    _panel.Add(&_rightGap);
    _panel.Add(&_rightButton);
    Add(&_panel);
}

void TitleBar::DeclareContent(Theme* theme) {

    Layout().padding = { .left = 16, .right = 16 };

    _panel.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(90) };
    _panel.Layout().padding = { .left = 12, .right = 12, .top = 12, .bottom = 12 };
    _panel.Layout().childGap = 12;
    _panel.Layout().childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER };
    _panel.Layout().layoutDirection = CLAY_LEFT_TO_RIGHT;
    _panel.SetPatternStyle(PatternPanel::PatternStyle::HorizontalLines);
    _panel.SetTileSize(16);

    _leftButton.Layout().sizing = { .width = CLAY_SIZING_FIXED(48), .height = CLAY_SIZING_FIXED(48) };
    _leftButton.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    _leftButton.SetBackgroundColor(theme->GetBackgroundColor());
    _leftButton.SetBorder(Clay_BorderElementConfig { .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(3) });

    _rightButton.Layout().sizing = { .width = CLAY_SIZING_FIXED(48), .height = CLAY_SIZING_FIXED(48) };
    _rightButton.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    _rightButton.SetBackgroundColor(theme->GetBackgroundColor());
    _rightButton.SetBorder(Clay_BorderElementConfig { .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(3) });

    _leftGap.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(1) };
    _rightGap.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(1) };

    _title.Layout().padding = { .left = 16, .right = 16, .top = 8, .bottom = 8 };
    _title.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    _title.SetBackgroundColor(theme->GetBackgroundColor());

    Panel::DeclareContent(theme);
}
