#pragma once
#include "project.h"
#include "Theme.h"

class Control {
public:
    Control() = default;
    virtual ~Control() = default;

    Control(const Control&) = delete;
    Control& operator=(const Control&) = delete;
    Control(Control&&) = delete;
    Control& operator=(Control&&) = delete;

    virtual void Declare(Theme* theme) = 0;
};
