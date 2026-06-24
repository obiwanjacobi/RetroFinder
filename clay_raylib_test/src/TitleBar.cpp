#include "TitleBar.h"

TitleBar::TitleBar(Clay_ElementId id) : Panel(id),
    _pattern(CLAY_ID("titlebar_pattern")),
    _titlePanel(CLAY_ID("titlebar_title_panel")),
    _leftEdge(CLAY_ID("titlebar_left_edge")),
    _leftButton(CLAY_ID("titlebar_left_button")),
    _rightButton(CLAY_ID("titlebar_right_button")),
    _rightEdge(CLAY_ID("titlebar_right_edge")),
    _title(CLAY_ID("titlebar_title"))
{
    Add(&_pattern);
    _titlePanel.Add(&_title);
    Add(&_titlePanel);

    _leftButton.SetText(CLAY_STRING("-"));
    _leftEdge.Add(&_leftButton);
    _rightEdge.Add(&_rightButton);
}

void TitleBar::DeclareContent(Theme* theme)
{
    constexpr float barHeight = 66;
    constexpr float buttonWidth = 60;
    constexpr uint16_t buttonWhitespace = 8;
    constexpr float buttonPlateWidth = buttonWidth + (buttonWhitespace * 2.0f);

    Layout().padding = { .left = 8, .right = 8, .top = 8, .bottom = 16 };
    Border() = { .color = theme->GetForegroundColor(), .width = {0, 0, 0, 6, 0}};

    // 6 lines with equal line/gap thickness at 6x: (6 lines + 5 gaps) * 6 = 66px.
    _pattern.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(barHeight) };
    _pattern.Layout().padding = { .left = 0, .right = 0, .top = 0, .bottom = 0 };
    _pattern.Layout().childGap = 0;
    _pattern.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
    _pattern.Layout().layoutDirection = CLAY_LEFT_TO_RIGHT;
    _pattern.SetPatternStyle(PatternPanel::PatternStyle::HorizontalLines);
    _pattern.SetTileSize(12);
    _pattern.SetPixelScale(6);

    _titlePanel.SetBackgroundColor(Clay_Color { 0 });
    _titlePanel.Layout().padding = { .left = 0, .right = 0, .top = 0, .bottom = 0 };
    _titlePanel.Floating() = Clay_FloatingElementConfig {
        .offset = { 0, 0 },
        .expand = { 0, 0 },
        .parentId = _pattern.Id().id,
        .zIndex = 1,
        .attachPoints = { .element = CLAY_ATTACH_POINT_CENTER_CENTER, .parent = CLAY_ATTACH_POINT_CENTER_CENTER },
        .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
        .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID,
        .clipTo = CLAY_CLIP_TO_NONE,
    };
    
    _title.Layout().padding = { .left = 16, .right = 16, .top = 16, .bottom = 16 };


    if (_leftButton.HasOnClick())
    {
        _leftButton.Layout().sizing = { .width = CLAY_SIZING_FIXED(buttonWidth), .height = CLAY_SIZING_FIXED(buttonWidth) };
        _leftButton.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
        _leftButton.Border() = { .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(5) };

        _leftEdge.Layout().sizing = { .width = CLAY_SIZING_FIXED(buttonPlateWidth), .height = CLAY_SIZING_FIXED(buttonPlateWidth) };
        _leftEdge.Layout().padding = { .left = buttonWhitespace, .right = buttonWhitespace, .top = 0, .bottom = 0 };
        _leftEdge.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
        _leftEdge.SetBackgroundColor(theme->GetBackgroundColor());
        _leftEdge.Floating() = Clay_FloatingElementConfig {
            .offset = { 32, 4 },
            .expand = { 0, 0 },
            .parentId = 0,
            .zIndex = 0,
            .attachPoints = { .element = CLAY_ATTACH_POINT_LEFT_TOP, .parent = CLAY_ATTACH_POINT_LEFT_TOP },
            .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
            .attachTo = CLAY_ATTACH_TO_PARENT,
            .clipTo = CLAY_CLIP_TO_NONE,
        };
    }

    if (_rightButton.HasOnClick())
    {
        _rightButton.Layout().sizing = { .width = CLAY_SIZING_FIXED(buttonWidth), .height = CLAY_SIZING_FIXED(buttonWidth) };
        _rightButton.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
        _rightButton.Border() = { .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(5) };

        _rightEdge.Layout().sizing = { .width = CLAY_SIZING_FIXED(buttonPlateWidth), .height = CLAY_SIZING_FIXED(buttonPlateWidth) };
        _rightEdge.Layout().padding = { .left = buttonWhitespace, .right = buttonWhitespace, .top = 0, .bottom = 0 };
        _rightEdge.Layout().childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER };
        _rightEdge.SetBackgroundColor(theme->GetBackgroundColor());
        _rightEdge.Floating() = Clay_FloatingElementConfig {
            .offset = { -32, 4 },
            .expand = { 0, 0 },
            .parentId = 0,
            .zIndex = 0,
            .attachPoints = { .element = CLAY_ATTACH_POINT_RIGHT_TOP, .parent = CLAY_ATTACH_POINT_RIGHT_TOP },
            .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
            .attachTo = CLAY_ATTACH_TO_PARENT,
            .clipTo = CLAY_CLIP_TO_NONE,
        };
    }

    // Window drag: press-and-hold on the pattern area, excluding buttons.
    // Setup drag handle with predicate and callback
    _dragHandle.SetCanStartDragPredicate([this]() {
        return _pattern.IsPointerOver()
            && !_leftButton.IsPointerOver()
            && !_rightButton.IsPointerOver();
    });

    _dragHandle.SetOnDrag([this](Vector2 delta) {
        if (_onDrag) {
            _onDrag(delta);
        }
    });

    Vector2 windowPos = GetWindowPosition();
    Vector2 mouseLocal = GetMousePosition();
    Vector2 mouseScreen = { windowPos.x + mouseLocal.x, windowPos.y + mouseLocal.y };
    _dragHandle.Update(mouseScreen);

    Panel::DeclareContent(theme);
}
