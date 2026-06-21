#include "ScrollView.h"

void ScrollView::Prepare(Theme* theme, BoxStyle& style) {
    ContainerControl::Prepare(theme, style);
    
    if (!HasBackgroundColor()) {
        style.backgroundColor = theme->GetBackgroundColor();
    }
}

void ScrollView::DeclareContent(Theme* theme) {
    const bool showV = _showVerticalScrollbar;
    const bool showH = _showHorizontalScrollbar;
    const bool reserveCorner = showV && showH;

    // Setup resize handle drag
    if (_showResizeHandle && reserveCorner) {
        _resizeHandleElementId = CLAY_IDI("scrollview_resize_handle", Id().id);

        _resizeDragHandle.SetCanStartDragPredicate([this]() {
            // Start drag only when pressing on the resize handle.
            return Clay_PointerOver(_resizeHandleElementId);
        });

        _resizeDragHandle.SetOnDrag([this](Vector2 delta) {
            if (!_onResize) {
                return;
            }

            Vector2 clampedDelta = delta;
            // resize constraints
            if (_resizeOnlyWhenScrollable) {
                constexpr float kThumbFull = 0.999f;

                // Stop enlarging each axis independently once that axis fully fits.
                if (clampedDelta.x > 0.0f && _horizontalScrollbar.GetThumbPercent() >= kThumbFull) {
                    clampedDelta.x = 0.0f;
                }
                if (clampedDelta.y > 0.0f && _verticalScrollbar.GetThumbPercent() >= kThumbFull) {
                    clampedDelta.y = 0.0f;
                }
            }

            if (clampedDelta.x == 0.0f && clampedDelta.y == 0.0f) {
                return;
            }

            _onResize(clampedDelta);
        });

        Vector2 mouseScreenPos = { GetMousePosition().x + GetWindowPosition().x, GetMousePosition().y + GetWindowPosition().y };
        _resizeDragHandle.Update(mouseScreenPos);
    }

    // Main container
    CLAY_AUTO_ID({
        .layout = {
            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        }
    }) {
        // Row 1: scroll content + optional vertical scrollbar
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                .layoutDirection = CLAY_LEFT_TO_RIGHT
            }
        }) {
            _scrollPanel.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) };
            _scrollPanel.Declare(theme);

            if (showV) {
                _verticalScrollbar.Declare(theme);
            }
        }

        // Row 2: optional horizontal scrollbar + optional resize corner
        if (showH) {
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED((float)_horizontalScrollbar.GetSize()) },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT
                }
            }) {
                _horizontalScrollbar.Declare(theme);

                if (reserveCorner) {
                    CLAY_AUTO_ID({
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_FIXED((float)_horizontalScrollbar.GetSize()),
                                .height = CLAY_SIZING_FIXED((float)_horizontalScrollbar.GetSize())
                            }
                        },
                        .backgroundColor = theme->GetBackgroundColor()
                    }) {
                        if (_showResizeHandle) {
                            CLAY(_resizeHandleElementId, {
                                .layout = {
                                    .sizing = {
                                        .width = CLAY_SIZING_GROW(0),
                                        .height = CLAY_SIZING_GROW(0)
                                    }
                                },
                                .backgroundColor = theme->GetBackgroundColor()
                            }) {
                                // Resize handle corner
                            }
                        }
                    }
                }
            }
        }
    }
}
