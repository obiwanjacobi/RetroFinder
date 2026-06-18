#pragma once
#include "project.h"
#include "Control.h"
#include <vector>

class Panel : public Control{
public:
    Panel() : _id(CLAY_ID("panel")) {}
    Panel(Clay_ElementId id) : _id(id) {}
    ~Panel() = default;

    Panel(const Panel&) = delete;
    Panel& operator=(const Panel&) = delete;
    Panel(Panel&&) = delete;
    Panel& operator=(Panel&&) = delete;

    void Declare(Theme* theme) override;

    Clay_LayoutConfig& Layout() { return _layout; }

    void Add(Control* control) { _controls.push_back(control); }

private:
    Clay_ElementId _id;
    Clay_LayoutConfig _layout;
    std::vector<Control*> _controls;
};
