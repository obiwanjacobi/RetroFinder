#pragma once
#include "project.h"
#include "Theme.h"

class Control {
public:
    Control() : Control(CLAY_ID("control")) {}
    explicit Control(Clay_ElementId id) : _id(id) {}
    virtual ~Control() = default;

    Control(const Control&) = delete;
    Control& operator=(const Control&) = delete;
    Control(Control&&) = delete;
    Control& operator=(Control&&) = delete;

    Clay_LayoutConfig& Layout() { return _layout; }
    const Clay_LayoutConfig& Layout() const { return _layout; }
    Clay_FloatingElementConfig& Floating() { return _floating; }
    const Clay_FloatingElementConfig& Floating() const { return _floating; }

    void SetBackgroundColor(Clay_Color color) {
        _backgroundColor = color;
        _hasBackgroundColor = true;
    }
    void ClearBackgroundColor() {
        _backgroundColor = Clay_Color { 0 };
        _hasBackgroundColor = false;
    }
    bool HasBackgroundColor() const { return _hasBackgroundColor; }
    const Clay_Color& GetBackgroundColor() const { return _backgroundColor; }

    void SetBorder(Clay_BorderElementConfig border) {
        _border = border;
        _hasBorder = true;
    }
    void ClearBorder() {
        _border = Clay_BorderElementConfig {};
        _hasBorder = false;
    }
    bool HasBorder() const { return _hasBorder; }
    const Clay_BorderElementConfig& GetBorder() const { return _border; }

    void Declare(Theme* theme);

    // Returns true if the pointer (set via Clay_SetPointerState) is over this
    // element's bounding box from the previous frame. Safe to call anywhere
    // in Declare/DeclareContent/Prepare.
    bool IsPointerOver() const { return Clay_PointerOver(_id); }

protected:
    struct BoxStyle {
        Clay_Color backgroundColor = {0};
        Clay_Color overlayColor = {0};
        Clay_CornerRadius cornerRadius = {};
        Clay_AspectRatioElementConfig aspectRatio = {};
        Clay_BorderElementConfig border = {};
        Clay_ImageElementConfig image = {};
        Clay_FloatingElementConfig floating = {};
        Clay_CustomElementConfig custom = {};
        Clay_ClipElementConfig clip = {};
    };

    Clay_ElementId Id() const { return _id; }

    virtual void Prepare(Theme* theme, BoxStyle& style);

    virtual void DeclareContent(Theme* theme) = 0;

private:
    Clay_ElementId _id;
    Clay_LayoutConfig _layout = {};
    Clay_FloatingElementConfig _floating = {};
    Clay_Color _backgroundColor = {0};
    Clay_BorderElementConfig _border = {};
    bool _hasBackgroundColor = false;
    bool _hasBorder = false;
};
