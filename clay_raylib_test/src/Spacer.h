#pragma once

#include "project.h"
#include "Control.h"

class Spacer : public Control {
public:
    Spacer() : Spacer(CLAY_ID("spacer")) {}
    explicit Spacer(Clay_ElementId id) : Control(id) {}
    ~Spacer() override = default;

    Spacer(const Spacer&) = delete;
    Spacer& operator=(const Spacer&) = delete;
    Spacer(Spacer&&) = delete;
    Spacer& operator=(Spacer&&) = delete;

protected:
    void Prepare(Theme* theme, BoxStyle& style) override {
        (void)theme;
        style.backgroundColor = Clay_Color { 0 };
        style.border = Clay_BorderElementConfig {};
    }

    void DeclareContent(Theme* theme) override {
        (void)theme;
    }
};