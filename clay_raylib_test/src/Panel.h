#pragma once
#include "clay_renderer_raylib.h"
#include "Control.h"
#include <vector>

class Panel : public Control{
public:
    Panel(Clay_ElementId id) : _id(id) {}
    ~Panel() = default;

    Panel(const Panel&) = delete;
    Panel& operator=(const Panel&) = delete;
    Panel(Panel&&) = delete;
    Panel& operator=(Panel&&) = delete;

    inline void Declare(Theme* theme) override {
        CLAY(_id, { .layout = _layout, .backgroundColor = theme->GetBackgroundColor() }); {
            for (auto* control : _controls)
                control->Declare(theme);
        }
    }

    Clay_LayoutConfig& Layout() { return _layout; }

    void Add(Control* control) { _controls.push_back(control); }

private:
    Clay_ElementId _id;
    Clay_LayoutConfig _layout;
    std::vector<Control*> _controls;
};
