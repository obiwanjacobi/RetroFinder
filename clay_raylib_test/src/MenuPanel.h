#pragma once

#include "project.h"
#include "FloatingPanel.h"
#include "MenuItem.h"

#include <functional>
#include <memory>
#include <vector>

class MenuPanel : public FloatingPanel {
public:
    MenuPanel() : MenuPanel(CLAY_ID("menu_panel")) {}
    explicit MenuPanel(Clay_ElementId id) : FloatingPanel(id) {}
    ~MenuPanel() override = default;

    MenuPanel(const MenuPanel&) = delete;
    MenuPanel& operator=(const MenuPanel&) = delete;
    MenuPanel(MenuPanel&&) = delete;
    MenuPanel& operator=(MenuPanel&&) = delete;

    MenuItem& AddItem(Clay_String text, MenuItem::ClickHandler onClick = nullptr, Clay_String shortcutText = CLAY_STRING(""));
    void SetOnItemActivated(std::function<void()> callback) { _onItemActivated = std::move(callback); }

protected:
    void Prepare(Theme* theme, BoxStyle& style) override;
    void DeclareContent(Theme* theme) override;

private:
    std::vector<std::unique_ptr<MenuItem>> _items;
    std::function<void()> _onItemActivated;
};
