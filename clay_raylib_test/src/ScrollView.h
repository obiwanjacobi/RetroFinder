#pragma once
#include "project.h"
#include "ContainerControl.h"
#include "ScrollPanel.h"
#include "Scrollbar.h"
#include "DragHandle.h"
#include "Icon.h"

#include <functional>

class ScrollView : public ContainerControl {
public:
    ScrollView() : ScrollView(CLAY_ID("scrollview")) {}
    explicit ScrollView(Clay_ElementId id) : ContainerControl(id), 
        _scrollPanel(CLAY_IDI("scrollpanel_inner", 0)),
        _verticalScrollbar(Scrollbar::VERTICAL, CLAY_IDI("vscrollbar", 0)),
        _horizontalScrollbar(Scrollbar::HORIZONTAL, CLAY_IDI("hscrollbar", 0)),
        _resizeIcon(CLAY_IDI("scrollview_resize_icon", id.id)) {
        _verticalScrollbar.SetScrollTarget(_scrollPanel.Id());
        _horizontalScrollbar.SetScrollTarget(_scrollPanel.Id());
    }
    ~ScrollView() = default;

    ScrollView(const ScrollView&) = delete;
    ScrollView& operator=(const ScrollView&) = delete;
    ScrollView(ScrollView&&) = delete;
    ScrollView& operator=(ScrollView&&) = delete;

    // Add child controls to the scroll panel content area
    void Add(Control* control) { _scrollPanel.Add(control); }
    void Remove(Control* control) { _scrollPanel.Remove(control); }

    // Scrollbar visibility
    void ShowVerticalScrollbar(bool show) { _showVerticalScrollbar = show; }
    void ShowHorizontalScrollbar(bool show) { _showHorizontalScrollbar = show; }
    bool IsVerticalScrollbarVisible() const { return _showVerticalScrollbar; }
    bool IsHorizontalScrollbarVisible() const { return _showHorizontalScrollbar; }

    // Resize handle
    void ShowResizeHandle(bool show) { _showResizeHandle = show; }
    bool IsResizeHandleVisible() const { return _showResizeHandle; }
    
    void SetResizeOnlyWhenScrollable(bool flag) { _resizeOnlyWhenScrollable = flag; }
    bool GetResizeOnlyWhenScrollable() const { return _resizeOnlyWhenScrollable; }

    using ResizeHandler = std::function<void(Vector2 delta)>;
    void SetOnResize(ResizeHandler handler) { _onResize = std::move(handler); }
    void ClearOnResize() { _onResize = nullptr; }

    // Scrollbar access
    Scrollbar& GetVerticalScrollbar() { return _verticalScrollbar; }
    Scrollbar& GetHorizontalScrollbar() { return _horizontalScrollbar; }

    ScrollPanel& GetScrollPanel() { return _scrollPanel; }
    const ScrollPanel& GetScrollPanel() const { return _scrollPanel; }

protected:
    void DeclareContent(Theme* theme) override;

private:
    ScrollPanel _scrollPanel;
    Scrollbar _verticalScrollbar;
    Scrollbar _horizontalScrollbar;
    Icon _resizeIcon;
    bool _showVerticalScrollbar = true;
    bool _showHorizontalScrollbar = true;
    bool _showResizeHandle = false;
    bool _resizeOnlyWhenScrollable = false;
    ResizeHandler _onResize;
    Clay_ElementId _resizeHandleElementId = {};
    DragHandle _resizeDragHandle;
};
