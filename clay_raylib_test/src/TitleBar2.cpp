#include "TitleBar2.h"

TitleBar2::TitleBar2(Clay_ElementId id) : Panel(id),
    _patternLeft(CLAY_ID("titlebar2_pattern_left")),
    _leftButtonGapOuter(CLAY_ID("titlebar2_left_button_gap_outer")),
    _leftButton(CLAY_ID("titlebar2_left_button")),
    _leftButtonGapInner(CLAY_ID("titlebar2_left_button_gap_inner")),
    _patternLeftMid(CLAY_ID("titlebar2_pattern_left_mid")),
    _title(CLAY_ID("titlebar2_title")),
    _patternRightMid(CLAY_ID("titlebar2_pattern_right_mid")),
    _rightButtonGapInner(CLAY_ID("titlebar2_right_button_gap_inner")),
    _rightButton(CLAY_ID("titlebar2_right_button")),
    _rightButtonGapOuter(CLAY_ID("titlebar2_right_button_gap_outer")),
    _patternRight(CLAY_ID("titlebar2_pattern_right"))
{
    _leftButton.SetText(CLAY_STRING("-"));

    Add(&_patternLeft);
    Add(&_leftButtonGapOuter);
    Add(&_leftButton);
    Add(&_leftButtonGapInner);
    Add(&_patternLeftMid);
    Add(&_title);
    Add(&_patternRightMid);
    Add(&_rightButtonGapInner);
    Add(&_rightButton);
    Add(&_rightButtonGapOuter);
    Add(&_patternRight);
}

void TitleBar2::DeclareContent(Theme* theme)
{
    constexpr float barHeight = 66;
    constexpr float outerPatternWidth = 24;
    constexpr float buttonPatternGap = 8;
    constexpr float buttonWidth = 60;

    Layout().padding = { .left = 8, .right = 8, .top = 8, .bottom = 16 };
    Layout().childGap = 0;
    Layout().childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER };
    Layout().layoutDirection = CLAY_LEFT_TO_RIGHT;
    SetBorder({ .color = theme->GetForegroundColor(), .width = {0, 0, 0, 6, 0}});

    auto configurePattern = [&](Pattern& pattern, Clay_SizingAxis width) {
        pattern.Layout().sizing = { .width = width, .height = CLAY_SIZING_FIXED(barHeight) };
        pattern.SetPatternStyle(Pattern::PatternStyle::HorizontalLines);
        pattern.SetTileSize(12);
        pattern.SetPixelScale(6);
    };

    configurePattern(_patternLeft, CLAY_SIZING_FIXED(outerPatternWidth));
    configurePattern(_patternLeftMid, CLAY_SIZING_GROW(0));
    configurePattern(_patternRightMid, CLAY_SIZING_GROW(0));
    configurePattern(_patternRight, CLAY_SIZING_FIXED(outerPatternWidth));

    _leftButtonGapOuter.Layout().sizing = { .width = CLAY_SIZING_FIXED(buttonPatternGap), .height = CLAY_SIZING_FIXED(1) };
    _leftButtonGapInner.Layout().sizing = { .width = CLAY_SIZING_FIXED(buttonPatternGap), .height = CLAY_SIZING_FIXED(1) };
    _rightButtonGapInner.Layout().sizing = { .width = CLAY_SIZING_FIXED(buttonPatternGap), .height = CLAY_SIZING_FIXED(1) };
    _rightButtonGapOuter.Layout().sizing = { .width = CLAY_SIZING_FIXED(buttonPatternGap), .height = CLAY_SIZING_FIXED(1) };

    _leftButton.Layout().sizing = { .width = CLAY_SIZING_FIXED(buttonWidth), .height = CLAY_SIZING_FIXED(buttonWidth) };
    _leftButton.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    _leftButton.SetBorder({ .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(5) });

    _rightButton.Layout().sizing = { .width = CLAY_SIZING_FIXED(buttonWidth), .height = CLAY_SIZING_FIXED(buttonWidth) };
    _rightButton.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    _rightButton.SetBorder({ .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(5) });

    _title.Layout().padding = { .left = 16, .right = 16, .top = 16, .bottom = 16 };
    _title.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    _title.SetBackgroundColor(theme->GetBackgroundColor());

    bool overDragZone = (
        _patternLeft.IsPointerOver() ||
        _patternLeftMid.IsPointerOver() ||
        _title.IsPointerOver() ||
        _patternRightMid.IsPointerOver() ||
        _patternRight.IsPointerOver()) &&
        !_leftButton.IsPointerOver() &&
        !_rightButton.IsPointerOver();

    Vector2 windowPos = GetWindowPosition();
    Vector2 mouseLocal = GetMousePosition();
    Vector2 mouseScreen = { windowPos.x + mouseLocal.x, windowPos.y + mouseLocal.y };

    if (overDragZone && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        _isDragging = true;
        _lastDragMouseScreen = mouseScreen;
    }
    if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        _isDragging = false;
    }
    if (_isDragging && _onDrag) {
        Vector2 delta = {
            mouseScreen.x - _lastDragMouseScreen.x,
            mouseScreen.y - _lastDragMouseScreen.y,
        };
        _lastDragMouseScreen = mouseScreen;
        _onDrag(delta);
    }

    Panel::DeclareContent(theme);
}
