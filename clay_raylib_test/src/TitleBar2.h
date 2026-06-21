#pragma once

#include "project.h"
#include "Button.h"
#include "Label.h"
#include "Panel.h"
#include "Pattern.h"
#include "Spacer.h"
#include "Theme.h"

#include <functional>

class TitleBar2 : public Panel {
public:
    TitleBar2() : TitleBar2(CLAY_ID("titlebar2")) {}
    explicit TitleBar2(Clay_ElementId id);
    ~TitleBar2() = default;

    TitleBar2(const TitleBar2&) = delete;
    TitleBar2& operator=(const TitleBar2&) = delete;
    TitleBar2(TitleBar2&&) = delete;
    TitleBar2& operator=(TitleBar2&&) = delete;

    void SetTitle(Clay_String title) { _title.SetText(title); }
    Clay_String GetTitle() { return _title.GetText(); }

    using ActionHandler = std::function<void()>;
    void SetOnMinimize(ActionHandler handler) { _leftButton.SetOnClick(std::move(handler)); }
    void ClearOnMinimize() { _leftButton.ClearOnClick(); }

    void SetOnClose(ActionHandler handler) { _rightButton.SetOnClick(std::move(handler)); }
    void ClearOnClose() { _rightButton.ClearOnClick(); }

    using DragHandler = std::function<void(Vector2 delta)>;
    void SetOnDrag(DragHandler handler) { _onDrag = std::move(handler); }
    void ClearOnDrag() { _onDrag = nullptr; }

protected:
    void DeclareContent(Theme* theme) override;

private:
    Pattern _patternLeft;
    Spacer _leftButtonGapOuter;
    Button _leftButton;
    Spacer _leftButtonGapInner;
    Pattern _patternLeftMid;
    Label _title;
    Pattern _patternRightMid;
    Spacer _rightButtonGapInner;
    Button _rightButton;
    Spacer _rightButtonGapOuter;
    Pattern _patternRight;

    DragHandler _onDrag;
    bool _isDragging = false;
    Vector2 _lastDragMouseScreen = { 0, 0 };
};
