#include "TitleBar.h"

TitleBar::TitleBar(Clay_ElementId id) : Panel(id),
    _pattern(CLAY_ID("titlebar_pattern")),
    _leftEdge(CLAY_ID("titlebar_left_edge")),
    _leftButton(CLAY_ID("titlebar_left_button")),
    _rightButton(CLAY_ID("titlebar_right_button")),
    _rightEdge(CLAY_ID("titlebar_right_edge")),
    _leftGap(CLAY_ID("titlebar_left_gap")),
    _rightGap(CLAY_ID("titlebar_right_gap")),
    _title(CLAY_ID("titlebar_title"))
{
    _leftButton.SetText(CLAY_STRING("-"));
    _rightButton.SetText(CLAY_STRING("x"));

    _pattern.Add(&_leftEdge);
    _pattern.Add(&_leftButton);
    _pattern.Add(&_leftGap);
    _pattern.Add(&_title);
    _pattern.Add(&_rightGap);
    _pattern.Add(&_rightButton);
    _pattern.Add(&_rightEdge);
    Add(&_pattern);
}

void TitleBar::DeclareContent(Theme* theme)
{
    Layout().padding = { .left = 8, .right = 8 };

    // 6 lines with equal line/gap thickness at 6x: (6 lines + 5 gaps) * 6 = 66px.
    _pattern.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(66) };
    _pattern.Layout().padding = { .left = 0, .right = 0, .top = 0, .bottom = 0 };
    _pattern.Layout().childGap = 0;
    _pattern.Layout().childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER };
    _pattern.Layout().layoutDirection = CLAY_LEFT_TO_RIGHT;
    _pattern.SetPatternStyle(PatternPanel::PatternStyle::HorizontalLines);
    _pattern.SetTileSize(12);
    _pattern.SetPixelScale(6);

    _leftButton.Layout().sizing = { .width = CLAY_SIZING_FIXED(60), .height = CLAY_SIZING_FIXED(60) };
    _leftButton.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    //_leftButton.Layout().padding = { .left = 16, .right = 16, .top = 16, .bottom = 16 };

    _rightButton.Layout().sizing = { .width = CLAY_SIZING_FIXED(60), .height = CLAY_SIZING_FIXED(60) };
    _rightButton.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    //_rightButton.Layout().padding = { .left = 16, .right = 16, .top = 16, .bottom = 16 };

    // Give the buttons a clearly visible inset from the outer edges.
    _leftEdge.Layout().sizing = { .width = CLAY_SIZING_FIXED(64), .height = CLAY_SIZING_FIXED(1) };
    _rightEdge.Layout().sizing = { .width = CLAY_SIZING_FIXED(64), .height = CLAY_SIZING_FIXED(1) };

    _leftGap.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(1) };
    _rightGap.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(1) };

    _title.Layout().padding = { .left = 16, .right = 16, .top = 16, .bottom = 16 };
    _title.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    _title.SetBackgroundColor(theme->GetBackgroundColor());

    // Window drag: press-and-hold on the pattern area, excluding buttons.
    bool overDragZone = _pattern.IsPointerOver()
                        && !_leftButton.IsPointerOver()
                        && !_rightButton.IsPointerOver();

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
