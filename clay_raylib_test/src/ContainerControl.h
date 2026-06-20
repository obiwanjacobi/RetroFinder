#pragma once

#include "Control.h"

#include <algorithm>
#include <vector>

class ContainerControl : public Control {
public:
    ContainerControl() : ContainerControl(CLAY_ID("container")) {}
    explicit ContainerControl(Clay_ElementId id) : Control(id) {}
    ~ContainerControl() override = default;

    ContainerControl(const ContainerControl&) = delete;
    ContainerControl& operator=(const ContainerControl&) = delete;
    ContainerControl(ContainerControl&&) = delete;
    ContainerControl& operator=(ContainerControl&&) = delete;

    void Add(Control* control) { _controls.push_back(control); }
    void Remove(Control* control) {
        _controls.erase(std::remove(_controls.begin(), _controls.end(), control), _controls.end());
    }

protected:
    void DeclareContent(Theme* theme) override {
        for (auto* control : _controls) {
            control->Declare(theme);
        }
    }

private:
    std::vector<Control*> _controls;
};