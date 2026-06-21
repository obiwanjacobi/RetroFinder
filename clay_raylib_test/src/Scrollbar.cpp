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

    if (_hasScrollTarget) {
        const Clay_ElementId targetId = _scrollTargetId;
        const float step = (float)_size;

        _startButton.SetOnClick([targetId, step]() {
            Clay_ScrollContainerData data = Clay_GetScrollContainerData(targetId);
            if (data.found && data.scrollPosition) {
                const float maxScroll = data.contentDimensions.height - data.scrollContainerDimensions.height;
                if (maxScroll <= 0.0f) return;
                data.scrollPosition->y = CLAY__MIN(0.0f, data.scrollPosition->y + step);
            }
        });

        _endButton.SetOnClick([targetId, step]() {
            Clay_ScrollContainerData data = Clay_GetScrollContainerData(targetId);
            if (data.found && data.scrollPosition) {
                const float maxScroll = data.contentDimensions.height - data.scrollContainerDimensions.height;
                if (maxScroll <= 0.0f) return;
                data.scrollPosition->y = CLAY__MAX(-(maxScroll), data.scrollPosition->y - step);
            }
        });
    }

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

    if (_hasScrollTarget) {
        const Clay_ElementId targetId = _scrollTargetId;
        const float step = (float)_size;

        _startButton.SetOnClick([targetId, step]() {
            Clay_ScrollContainerData data = Clay_GetScrollContainerData(targetId);
            if (data.found && data.scrollPosition) {
                const float maxScroll = data.contentDimensions.width - data.scrollContainerDimensions.width;
                if (maxScroll <= 0.0f) return;
                data.scrollPosition->x = CLAY__MIN(0.0f, data.scrollPosition->x + step);
            }
        });

        _endButton.SetOnClick([targetId, step]() {
            Clay_ScrollContainerData data = Clay_GetScrollContainerData(targetId);
            if (data.found && data.scrollPosition) {
                const float maxScroll = data.contentDimensions.width - data.scrollContainerDimensions.width;
                if (maxScroll <= 0.0f) return;
                data.scrollPosition->x = CLAY__MAX(-(maxScroll), data.scrollPosition->x - step);
            }
        });
    }

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

    // Generate unique element IDs for this scrollbar
    _thumbElementId = CLAY_IDI("scrollbar_thumb", (_orientation << 16) | (uint32_t)_hasScrollTarget);
    _trackElementId = CLAY_IDI("scrollbar_track", (_orientation << 16) | (uint32_t)_hasScrollTarget);

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
    CLAY(_trackElementId, {
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
        if (_hasScrollTarget) {
            Clay_OnHover(OnTrackClickCallback, this);
        }

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
    CLAY(_trackElementId, {
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
        if (_hasScrollTarget) {
            Clay_OnHover(OnTrackClickCallback, this);
        }

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

void Scrollbar::OnTrackClickCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void* userData)
{
    if (pointerData.state != CLAY_POINTER_DATA_PRESSED_THIS_FRAME) return;

    Scrollbar* self = static_cast<Scrollbar*>(userData);
    if (elementId.id != self->_trackElementId.id) return;

    // Let the thumb drag handle its own interaction
    if (Clay_PointerOver(self->_thumbElementId)) return;

    Clay_ElementData trackData = Clay_GetElementData(self->_trackElementId);
    Clay_ElementData thumbData = Clay_GetElementData(self->_thumbElementId);
    if (!trackData.found || !thumbData.found) return;

    Clay_ScrollContainerData scrollData = Clay_GetScrollContainerData(self->_scrollTargetId);
    if (!scrollData.found || !scrollData.scrollPosition) return;

    const bool isVertical = (self->_orientation == VERTICAL);
    const float contentAxis   = isVertical ? scrollData.contentDimensions.height        : scrollData.contentDimensions.width;
    const float containerAxis = isVertical ? scrollData.scrollContainerDimensions.height : scrollData.scrollContainerDimensions.width;
    const float maxScroll = contentAxis - containerAxis;
    if (maxScroll <= 0.0f) return;

    const float trackStart = isVertical ? trackData.boundingBox.y      : trackData.boundingBox.x;
    const float trackSize  = isVertical ? trackData.boundingBox.height  : trackData.boundingBox.width;
    const float thumbSize  = isVertical ? thumbData.boundingBox.height  : thumbData.boundingBox.width;
    const float mousePos   = isVertical ? pointerData.position.y        : pointerData.position.x;

    // Center the thumb on the click position
    const float clickRelative = mousePos - trackStart - thumbSize * 0.5f;
    const float scrollT = CLAY__MAX(0.0f, CLAY__MIN(1.0f, clickRelative / (trackSize - thumbSize)));

    if (isVertical) {
        scrollData.scrollPosition->y = -(scrollT * maxScroll);
    } else {
        scrollData.scrollPosition->x = -(scrollT * maxScroll);
    }
}

