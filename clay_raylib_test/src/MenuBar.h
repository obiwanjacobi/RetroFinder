#pragma once

#include "project.h"
#include "Control.h"
#include "MenuPanel.h"

#include <memory>
#include <vector>

class MenuBar : public Control {
public:
    MenuBar() : MenuBar(CLAY_ID("menu_bar")) {}
    explicit MenuBar(Clay_ElementId id) : Control(id) {}
    ~MenuBar() override = default;

    MenuBar(const MenuBar&) = delete;
    MenuBar& operator=(const MenuBar&) = delete;
    MenuBar(MenuBar&&) = delete;
    MenuBar& operator=(MenuBar&&) = delete;

    MenuPanel& AddMenu(Clay_String title);

    void CloseMenus() { _openMenuIndex = -1; }
    int GetOpenMenuIndex() const { return _openMenuIndex; }

protected:
    void Prepare(Theme* theme, BoxStyle& style) override;
    void DeclareContent(Theme* theme) override;

private:
    struct MenuEntry {
        std::unique_ptr<MenuItem> trigger;
        std::unique_ptr<MenuPanel> panel;
    };

    std::vector<MenuEntry> _entries;
    int _openMenuIndex = -1;
};
