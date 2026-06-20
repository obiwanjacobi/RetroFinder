#pragma once
#include "project.h"
#include "Button.h"
#include "Label.h"
#include "Panel.h"
#include "PatternPanel.h"
#include "Spacer.h"
#include "Theme.h"

#include <functional>

class TitleBar : public Panel {
public:
    TitleBar() : TitleBar(CLAY_ID("titlebar")) {}
    TitleBar(Clay_ElementId id);
    ~TitleBar() = default;

    TitleBar(const TitleBar&) = delete;
    TitleBar& operator=(const TitleBar&) = delete;
    TitleBar(TitleBar&&) = delete;
    TitleBar& operator=(TitleBar&&) = delete;

    void DeclareContent(Theme* theme) override;

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

private:
    PatternPanel _pattern;
    Spacer _leftEdge;
    Button _leftButton;
    Button _rightButton;
    Spacer _rightEdge;
    Spacer _leftGap;
    Spacer _rightGap;
    Label _title;

    DragHandler _onDrag;
    bool _isDragging = false;
    Vector2 _lastDragMouseScreen = { 0, 0 };
};
