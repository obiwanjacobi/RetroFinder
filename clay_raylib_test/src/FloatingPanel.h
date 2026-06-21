#pragma once

#include "Panel.h"

class FloatingPanel : public Panel {
public:
    FloatingPanel() : FloatingPanel(CLAY_ID("floating_panel")) {}
    explicit FloatingPanel(Clay_ElementId id);
    ~FloatingPanel() override = default;

    FloatingPanel(const FloatingPanel&) = delete;
    FloatingPanel& operator=(const FloatingPanel&) = delete;
    FloatingPanel(FloatingPanel&&) = delete;
    FloatingPanel& operator=(FloatingPanel&&) = delete;

    Clay_FloatingElementConfig& Floating() { return _floating; }
    const Clay_FloatingElementConfig& Floating() const { return _floating; }

protected:
    void Prepare(Theme* theme, BoxStyle& style) override;

private:
    Clay_FloatingElementConfig _floating = {};
};
