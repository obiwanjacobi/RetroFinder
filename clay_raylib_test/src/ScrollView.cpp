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
                    .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(_horizontalScrollbar.GetSize()) },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT
                }
            }) {
                _horizontalScrollbar.Declare(theme);

                if (reserveCorner) {
                    CLAY_AUTO_ID({
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_FIXED(_horizontalScrollbar.GetSize()),
                                .height = CLAY_SIZING_FIXED(_horizontalScrollbar.GetSize())
                            }
                        },
                        .backgroundColor = theme->GetBackgroundColor()
                    }) {
                        if (_showResizeHandle) {
                            CLAY_AUTO_ID({
                                .layout = {
                                    .sizing = {
                                        .width = CLAY_SIZING_GROW(0),
                                        .height = CLAY_SIZING_GROW(0)
                                    }
                                },
                                .backgroundColor = theme->GetForegroundColor()
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
