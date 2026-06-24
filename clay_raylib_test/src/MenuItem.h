#pragma once

#include "project.h"
#include "Control.h"
#include "Theme.h"

#include <functional>

class MenuItem : public Control {
public:
    using ClickHandler = std::function<void()>;

    MenuItem() : MenuItem(CLAY_ID("menu_item")) {}
    explicit MenuItem(Clay_ElementId id) : Control(id) {}
    ~MenuItem() override = default;

    MenuItem(const MenuItem&) = delete;
    MenuItem& operator=(const MenuItem&) = delete;
    MenuItem(MenuItem&&) = delete;
    MenuItem& operator=(MenuItem&&) = delete;

    void SetText(Clay_String text) { _text = text; }
    const Clay_String& GetText() const { return _text; }

    void SetShortcutText(Clay_String text) { _shortcutText = text; }
    const Clay_String& GetShortcutText() const { return _shortcutText; }

    void SetEnabled(bool enabled) { _enabled = enabled; }
    bool IsEnabled() const { return _enabled; }

    void SetOnClick(ClickHandler handler) { _onClick = std::move(handler); }
    void ClearOnClick() { _onClick = nullptr; }
    bool HasOnClick() const { return _onClick != nullptr; }

protected:
    void Prepare(Theme* theme, BoxStyle& style) override;
    void DeclareContent(Theme* theme) override;

private:
    static void OnHoverCallback(Clay_ElementId, Clay_PointerData pointerData, void* userData) {
        if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
            MenuItem* item = static_cast<MenuItem*>(userData);
            if (item->_enabled && item->_onClick) {
                item->_onClick();
            }
        }
    }

    Clay_String _text = CLAY_STRING("");
    Clay_String _shortcutText = CLAY_STRING("");
    ClickHandler _onClick;
    bool _enabled = true;
};
