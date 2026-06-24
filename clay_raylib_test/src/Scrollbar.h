#pragma once
#include "project.h"
#include "Control.h"
#include "Theme.h"
#include "IconButton.h"
#include "Pattern.h"
#include "DragHandle.h"

class Scrollbar : public Control {
public:
    enum Orientation {
        HORIZONTAL,
        VERTICAL
    };

    explicit Scrollbar(Orientation orientation, Clay_ElementId id = CLAY_ID("scrollbar"))
        : Control(id),
          _orientation(orientation),
          _startButton(CLAY_IDI("scrollbar_btn_start", (uint32_t)orientation)),
          _endButton(CLAY_IDI("scrollbar_btn_end", (uint32_t)orientation)),
          _middlePattern(CLAY_IDI("scrollbar_pattern", (uint32_t)orientation)) {}

    ~Scrollbar() = default;

    Scrollbar(const Scrollbar&) = delete;
    Scrollbar& operator=(const Scrollbar&) = delete;
    Scrollbar(Scrollbar&&) = delete;
    Scrollbar& operator=(Scrollbar&&) = delete;

    void SetSize(uint32_t size) { _size = size; }
    uint32_t GetSize() const { return _size; }
    float GetThumbPercent() const { return _thumbPercent; }
    void SetScrollTarget(Clay_ElementId id) {
        _scrollTargetId = id;
        _hasScrollTarget = true;
    }

protected:
    void DeclareContent(Theme* theme) override {
        if (_orientation == VERTICAL) {
            DeclareVerticalScrollbar(theme);
        } else {
            DeclareHorizontalScrollbar(theme);
        }
    }

private:
    Orientation _orientation;
    uint32_t _size = 48;
    float _thumbPercent = 1.0f;
    Clay_ElementId _scrollTargetId = {};
    bool _hasScrollTarget = false;
    Clay_ElementId _thumbElementId = {};
    Clay_ElementId _trackElementId = {};
    DragHandle _thumbDragHandle;
    bool _thumbDragActive = false;
    float _thumbDragStartMouseAxis = 0.0f;
    float _thumbDragStartScrollPos = 0.0f;
    float _thumbDragCurrentMouseAxis = 0.0f;
    IconButton _startButton;
    IconButton _endButton;
    Pattern _middlePattern;

    void DeclareVerticalScrollbar(Theme* theme);
    void DeclareHorizontalScrollbar(Theme* theme);
    void DeclareTrackAndThumb(Theme* theme);
    void DeclareVerticalTrackAndThumb(Theme* theme, float trackAxis, float thumbAxis, float thumbOffset, float tailAxis, const Control::BoxStyle& trackStyle);
    void DeclareHorizontalTrackAndThumb(Theme* theme, float trackAxis, float thumbAxis, float thumbOffset, float tailAxis, const Control::BoxStyle& trackStyle);

    static void OnTrackClickCallback(Clay_ElementId elementId, Clay_PointerData pointerData, void* userData);
};
