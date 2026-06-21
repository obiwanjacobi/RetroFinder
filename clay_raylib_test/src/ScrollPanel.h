#pragma once
#include "project.h"
#include "ContainerControl.h"

class ScrollPanel : public ContainerControl {
public:
    ScrollPanel() : ScrollPanel(CLAY_ID("scrollpanel")) {}
    explicit ScrollPanel(Clay_ElementId id) : ContainerControl(id) {}
    ~ScrollPanel() = default;

    ScrollPanel(const ScrollPanel&) = delete;
    ScrollPanel& operator=(const ScrollPanel&) = delete;
    ScrollPanel(ScrollPanel&&) = delete;
    ScrollPanel& operator=(ScrollPanel&&) = delete;

    void EnableScrollVertical(bool v) { _scrollVertical = v; }
    void EnableScrollHorizontal(bool h) { _scrollHorizontal = h; }
    bool IsScrollVerticalEnabled() const { return _scrollVertical; }
    bool IsScrollHorizontalEnabled() const { return _scrollHorizontal; }

protected:
    void Prepare(Theme* theme, BoxStyle& style) override;

private:
    bool _scrollVertical = true;
    bool _scrollHorizontal = true;
};
