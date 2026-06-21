#include "Scrollbar.h"

namespace {
constexpr float kMinThumbSize = 18.0f;
constexpr uint16_t kIconScale = 3;
constexpr uint16_t kIconSize = 15;
}

void Scrollbar::DeclareVerticalScrollbar(Theme* theme)
{
    Layout().sizing = { .width = CLAY_SIZING_FIXED((float)_size), .height = CLAY_SIZING_GROW(0) };
    Layout().layoutDirection = CLAY_TOP_TO_BOTTOM;

    _startButton.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED((float)_size) };
    _startButton.SetIcon(IconButton::IconType::ArrowUp, kIconSize, kIconScale);
    _startButton.Declare(theme);

    DeclareTrackAndThumb(theme);

    _endButton.Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED((float)_size) };
    _endButton.SetIcon(IconButton::IconType::ArrowDown, kIconSize, kIconScale);
    _endButton.Declare(theme);
}

void Scrollbar::DeclareHorizontalScrollbar(Theme* theme)
{
    Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED((float)_size) };
    Layout().layoutDirection = CLAY_LEFT_TO_RIGHT;

    _startButton.Layout().sizing = { .width = CLAY_SIZING_FIXED((float)_size), .height = CLAY_SIZING_GROW(0) };
    _startButton.SetIcon(IconButton::IconType::ArrowLeft, kIconSize, kIconScale);
    _startButton.Declare(theme);

    DeclareTrackAndThumb(theme);

    _endButton.Layout().sizing = { .width = CLAY_SIZING_FIXED((float)_size), .height = CLAY_SIZING_GROW(0) };
    _endButton.SetIcon(IconButton::IconType::ArrowRight, kIconSize, kIconScale);
    _endButton.Declare(theme);
}

void Scrollbar::DeclareTrackAndThumb(Theme* theme)
{
    bool vertical = _orientation == VERTICAL;

    // If no scroll target, render empty track without thumb
    if (!_hasScrollTarget) {
        _thumbPercent = 1.0f;
        Control::BoxStyle trackStyle = {};
        if (vertical) {
            DeclareVerticalTrackAndThumb(theme, (float)_size, (float)_size, 0.0f, 0.0f, trackStyle);
        } else {
            DeclareHorizontalTrackAndThumb(theme, (float)_size, (float)_size, 0.0f, 0.0f, trackStyle);
        }
        return;
    }

    Clay_ScrollContainerData scrollData = Clay_GetScrollContainerData(_scrollTargetId);

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
    _thumbPercent = trackAxis > 0.0f ? (thumbAxis / trackAxis) : 1.0f;
    _thumbPercent = CLAY__MAX(0.0f, CLAY__MIN(_thumbPercent, 1.0f));

    // Generate a unique thumb ID for this scrollbar
    _thumbElementId = CLAY_IDI("scrollbar_thumb", (_orientation << 16) | (uint32_t)_hasScrollTarget);

    _middlePattern.SetPatternStyle(Pattern::PatternStyle::Dots);
    if (vertical) {
        _middlePattern.SetRenderSize((float)_size, trackAxis);
    } else {
        _middlePattern.SetRenderSize(trackAxis, (float)_size);
    }
    Control::BoxStyle trackStyle = {};
    _middlePattern.ApplyToStyle(theme, trackStyle);
    _middlePattern.ClearRenderSize();

    // Setup drag handle for thumb interactivity
    {
        const bool isVertical = vertical;
        const float trackSize = trackAxis;
        const float thumbSize = thumbAxis;
        const float maxThumbOffset = trackSize - thumbSize;
        const Clay_ElementId targetId = _scrollTargetId;

        _thumbDragHandle.SetCanStartDragPredicate([this]() {
            return Clay_PointerOver(_thumbElementId);
        });

        _thumbDragHandle.SetOnDrag([this, isVertical, trackSize, thumbSize, maxThumbOffset, contentAxis, containerAxis, targetId](
            Vector2 delta) {
            // Re-fetch scroll data each frame since we can't capture a local reference
            Clay_ScrollContainerData currentScrollData = Clay_GetScrollContainerData(targetId);
            if (!currentScrollData.found || !currentScrollData.scrollPosition) {
                return;
            }

            const float dragDelta = isVertical ? delta.y : delta.x;
            const float maxScroll = contentAxis - containerAxis;
            if (maxScroll <= 0.0f) {
                return;
            }

            const float thumbT = maxThumbOffset > 0.0f ? (dragDelta / maxThumbOffset) : 0.0f;
            const float scrollDelta = thumbT * maxScroll;
            const float newScroll = isVertical
                ? currentScrollData.scrollPosition->y - scrollDelta
                : currentScrollData.scrollPosition->x - scrollDelta;

            const float minScroll = -maxScroll;
            const float clampedScroll = CLAY__MAX(CLAY__MIN(newScroll, 0.0f), minScroll);

            if (isVertical) {
                currentScrollData.scrollPosition->y = clampedScroll;
            } else {
                currentScrollData.scrollPosition->x = clampedScroll;
            }
        });
    }

    Vector2 mouseScreenPos = { GetMousePosition().x + GetWindowPosition().x, GetMousePosition().y + GetWindowPosition().y };
    _thumbDragHandle.Update(mouseScreenPos);

    if (vertical) {
        DeclareVerticalTrackAndThumb(theme, trackAxis, thumbAxis, thumbOffset, tailAxis, trackStyle);
    } else {
        DeclareHorizontalTrackAndThumb(theme, trackAxis, thumbAxis, thumbOffset, tailAxis, trackStyle);
    }
}

void Scrollbar::DeclareVerticalTrackAndThumb(Theme* theme, float trackAxis, float thumbAxis, float thumbOffset, float tailAxis, const Control::BoxStyle& trackStyle)
{
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

        if (_hasScrollTarget) {
            CLAY(_thumbElementId, {
                .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(thumbAxis) } },
                .backgroundColor = theme->GetBackgroundColor(),
                .cornerRadius = { 4 },
                .border = {
                    .color = theme->GetForegroundColor(),
                    .width = CLAY_BORDER_OUTSIDE(2)
                }
            }) {}
        }

        if (tailAxis > 0.0f) {
            CLAY_AUTO_ID({
                .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(tailAxis) } }
            }) {}
        }
    }
}

void Scrollbar::DeclareHorizontalTrackAndThumb(Theme* theme, float trackAxis, float thumbAxis, float thumbOffset, float tailAxis, const Control::BoxStyle& trackStyle)
{
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

        if (_hasScrollTarget) {
            CLAY(_thumbElementId, {
                .layout = { .sizing = { .width = CLAY_SIZING_FIXED(thumbAxis), .height = CLAY_SIZING_GROW(0) } },
                .backgroundColor = theme->GetBackgroundColor(),
                .cornerRadius = { 4 },
                .border = {
                    .color = theme->GetForegroundColor(),
                    .width = CLAY_BORDER_OUTSIDE(2)
                }
            }) {}
        }

        if (tailAxis > 0.0f) {
            CLAY_AUTO_ID({
                .layout = { .sizing = { .width = CLAY_SIZING_FIXED(tailAxis), .height = CLAY_SIZING_GROW(0) } }
            }) {}
        }
    }
}
