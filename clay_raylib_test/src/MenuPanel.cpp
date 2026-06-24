#include "MenuPanel.h"

#include <algorithm>
#include <string>

namespace {
    float MeasureClayStringWidth(const Theme* theme, const Clay_String& text)
    {
        if (text.length <= 0 || text.chars == nullptr) {
            return 0.0f;
        }

        std::string value(text.chars, static_cast<size_t>(text.length));
        return MeasureTextEx(theme->GetFont(), value.c_str(), (float)theme->GetFontSize(), (float)theme->GetFontSpacing()).x;
    }
}

MenuItem& MenuPanel::AddItem(Clay_String text, MenuItem::ClickHandler onClick, Clay_String shortcutText)
{
    const int itemIndex = static_cast<int>(_items.size());
    auto item = std::make_unique<MenuItem>(CLAY_IDI("menu_panel_item", (Id().id * 100) + itemIndex + 1));
    item->SetText(text);
    item->SetShortcutText(shortcutText);

    MenuItem::ClickHandler action = std::move(onClick);
    item->SetOnClick([this, action = std::move(action)]() mutable {
        if (action) {
            action();
        }

        // TODO: Support a key combination (for example Shift) to keep the
        // menu panel open after activating an item.
        if (_onItemActivated) {
            _onItemActivated();
        }
    });

    _items.push_back(std::move(item));
    return *_items.back();
}

void MenuPanel::Prepare(Theme* theme, BoxStyle& style)
{
    FloatingPanel::Prepare(theme, style);

    style.backgroundColor = theme->GetBackgroundColor();
    style.border = { .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(3) };
}

void MenuPanel::DeclareContent(Theme* theme)
{
    constexpr uint16_t kPanelPaddingLeft = 10;
    constexpr uint16_t kPanelPaddingRight = 10;
    constexpr uint16_t kPanelPaddingTop = 10;
    constexpr uint16_t kPanelPaddingBottom = 10;
    constexpr float kItemLeftPadding = 12.0f;
    constexpr float kItemRightPadding = 22.0f;
    constexpr float kItemTextGap = 56.0f;
    constexpr float kMinInnerWidth = 200.0f;

    float widestItemWidth = 0.0f;
    for (const auto& item : _items) {
        const float textWidth = MeasureClayStringWidth(theme, item->GetText());
        const Clay_String& shortcut = item->GetShortcutText();
        const bool hasShortcut = shortcut.length > 0;
        const float shortcutWidth = hasShortcut ? MeasureClayStringWidth(theme, shortcut) : 0.0f;

        const float itemWidth = kItemLeftPadding + textWidth + (hasShortcut ? (kItemTextGap + shortcutWidth) : 0.0f) + kItemRightPadding;
        widestItemWidth = std::max(widestItemWidth, itemWidth);
    }

    const float innerWidth = std::max(kMinInnerWidth, widestItemWidth);

    Layout().layoutDirection = CLAY_TOP_TO_BOTTOM;
    Layout().padding = {
        .left = kPanelPaddingLeft,
        .right = kPanelPaddingRight,
        .top = kPanelPaddingTop,
        .bottom = kPanelPaddingBottom
    };
    Layout().sizing = {
        .width = CLAY_SIZING_FIXED(innerWidth + kPanelPaddingLeft + kPanelPaddingRight),
        .height = CLAY_SIZING_FIT(0)
    };

    for (auto& item : _items) {
        item->Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(52) };
        item->Declare(theme);
    }
}
