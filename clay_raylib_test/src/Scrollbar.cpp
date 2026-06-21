#include "Scrollbar.h"

namespace {
constexpr float kMinThumbSize = 18.0f;
}

void Scrollbar::DeclareVerticalScrollbar(Theme* theme)
{
    Layout().sizing = { .width = CLAY_SIZING_FIXED((float)_size), .height = CLAY_SIZING_GROW(0) };
    Layout().layoutDirection = CLAY_TOP_TO_BOTTOM;

    _startButton.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED((float)_size) };
    _startButton.SetText(CLAY_STRING("^"));
    _startButton.Declare(theme);

    DeclareTrackAndThumb(theme, true);

    _endButton.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED((float)_size) };
    _endButton.SetText(CLAY_STRING("v"));
    _endButton.Declare(theme);
}

void Scrollbar::DeclareHorizontalScrollbar(Theme* theme)
{
    Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED((float)_size) };
    Layout().layoutDirection = CLAY_LEFT_TO_RIGHT;

    _startButton.Layout().sizing = { .width = CLAY_SIZING_FIXED((float)_size), .height = CLAY_SIZING_GROW(0) };
    _startButton.SetText(CLAY_STRING("<"));
    _startButton.Declare(theme);

    DeclareTrackAndThumb(theme, false);

    _endButton.Layout().sizing = { .width = CLAY_SIZING_FIXED((float)_size), .height = CLAY_SIZING_GROW(0) };
    _endButton.SetText(CLAY_STRING(">"));
    _endButton.Declare(theme);
}

void Scrollbar::DeclareTrackAndThumb(Theme* theme, bool vertical)
{
    Clay_ScrollContainerData scrollData = {};
    if (_hasScrollTarget) {
        scrollData = Clay_GetScrollContainerData(_scrollTargetId);
    }

    const float containerAxis = vertical
        ? scrollData.scrollContainerDimensions.height
        : scrollData.scrollContainerDimensions.width;
    const float contentAxis = vertical
        ? scrollData.contentDimensions.height
        : scrollData.contentDimensions.width;

    float trackAxis = containerAxis - (float)_size * 2.0f;
    if (trackAxis <= 0.0f) {
        trackAxis = (float)_size;
    }

    float thumbAxis = trackAxis;
    float thumbOffset = 0.0f;

    if (scrollData.found && scrollData.scrollPosition != nullptr && contentAxis > containerAxis) {
        const float maxScroll = contentAxis - containerAxis;
        const float ratio = containerAxis / contentAxis;
        thumbAxis = CLAY__MAX(trackAxis * ratio, kMinThumbSize);
        thumbAxis = CLAY__MIN(thumbAxis, trackAxis);

        const float rawScroll = vertical ? -scrollData.scrollPosition->y : -scrollData.scrollPosition->x;
        const float clampedScroll = CLAY__MAX(0.0f, CLAY__MIN(rawScroll, maxScroll));
        const float scrollT = maxScroll > 0.0f ? (clampedScroll / maxScroll) : 0.0f;
        thumbOffset = (trackAxis - thumbAxis) * scrollT;
    }

    const float tailAxis = CLAY__MAX(trackAxis - thumbAxis - thumbOffset, 0.0f);

    _middlePattern.SetPatternStyle(Pattern::PatternStyle::Dots);
    if (vertical) {
        _middlePattern.SetRenderSize((float)_size, trackAxis);
    } else {
        _middlePattern.SetRenderSize(trackAxis, (float)_size);
    }
    Control::BoxStyle trackStyle = {};
    _middlePattern.ApplyToStyle(theme, trackStyle);
    _middlePattern.ClearRenderSize();

    if (vertical) {
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                .layoutDirection = CLAY_TOP_TO_BOTTOM
            },
            .backgroundColor = trackStyle.backgroundColor,
            .cornerRadius = { 4 },
            .image = trackStyle.image,
            .clip = trackStyle.clip,
            .border = {
                .color = theme->GetForegroundColor(),
                .width = CLAY_BORDER_OUTSIDE(2)
            }
        }) {
            if (thumbOffset > 0.0f) {
                CLAY_AUTO_ID({
                    .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(thumbOffset) } }
                }) {}
            }

            CLAY_AUTO_ID({
                .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(thumbAxis) } },
                .backgroundColor = theme->GetBackgroundColor(),
                .cornerRadius = { 4 },
                .border = {
                    .color = theme->GetForegroundColor(),
                    .width = CLAY_BORDER_OUTSIDE(2)
                }
            }) {}

            if (tailAxis > 0.0f) {
                CLAY_AUTO_ID({
                    .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(tailAxis) } }
                }) {}
            }
        }
    } else {
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                .layoutDirection = CLAY_LEFT_TO_RIGHT
            },
            .backgroundColor = trackStyle.backgroundColor,
            .cornerRadius = { 4 },
            .image = trackStyle.image,
            .clip = trackStyle.clip,
            .border = {
                .color = theme->GetForegroundColor(),
                .width = CLAY_BORDER_OUTSIDE(2)
            }
        }) {
            if (thumbOffset > 0.0f) {
                CLAY_AUTO_ID({
                    .layout = { .sizing = { .width = CLAY_SIZING_FIXED(thumbOffset), .height = CLAY_SIZING_GROW(0) } }
                }) {}
            }

            CLAY_AUTO_ID({
                .layout = { .sizing = { .width = CLAY_SIZING_FIXED(thumbAxis), .height = CLAY_SIZING_GROW(0) } },
                .backgroundColor = theme->GetBackgroundColor(),
                .cornerRadius = { 4 },
                .border = {
                    .color = theme->GetForegroundColor(),
                    .width = CLAY_BORDER_OUTSIDE(2)
                }
            }) {}

            if (tailAxis > 0.0f) {
                CLAY_AUTO_ID({
                    .layout = { .sizing = { .width = CLAY_SIZING_FIXED(tailAxis), .height = CLAY_SIZING_GROW(0) } }
                }) {}
            }
        }
    }
}
