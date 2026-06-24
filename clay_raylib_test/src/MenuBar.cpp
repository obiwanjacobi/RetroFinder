#include "MenuBar.h"

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

MenuPanel& MenuBar::AddMenu(Clay_String title)
{
    const int index = static_cast<int>(_entries.size());

    MenuEntry entry;
    entry.trigger = std::make_unique<MenuItem>(CLAY_IDI("menu_bar_trigger", (Id().id * 100) + index + 1));
    entry.panel = std::make_unique<MenuPanel>(CLAY_IDI("menu_bar_panel", (Id().id * 100) + index + 1));
    entry.panel->SetOnItemActivated([this]() {
        CloseMenus();
    });

    entry.trigger->SetText(title);
    entry.trigger->SetOnClick([this, index]() {
        _openMenuIndex = (_openMenuIndex == index) ? -1 : index;
    });

    _entries.push_back(std::move(entry));
    return *_entries.back().panel;
}

void MenuBar::Prepare(Theme* theme, BoxStyle& style)
{
    Control::Prepare(theme, style);
    style.backgroundColor = theme->GetBackgroundColor();
    style.border = { .color = theme->GetForegroundColor(), .width = { 0, 0, 0, 3, 0 } };
}

void MenuBar::DeclareContent(Theme* theme)
{
    constexpr float kMenuBarHeight = 60.0f;
    constexpr float kMenuTriggerHeight = 52.0f;
    constexpr float kTriggerLeftPadding = 12.0f;
    constexpr float kTriggerRightPadding = 12.0f;
    constexpr float kTriggerExtraRightPadding = 8.0f;
    constexpr float kMinTriggerWidth = 80.0f;

    Layout().layoutDirection = CLAY_TOP_TO_BOTTOM;

    CLAY_AUTO_ID({
        .layout = {
            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(kMenuBarHeight) },
            .padding = { .left = 8, .right = 8, .top = 4, .bottom = 4 },
            .childGap = 4,
            .childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER },
            .layoutDirection = CLAY_LEFT_TO_RIGHT
        }
    }) {
        int hoveredTopMenuIndex = -1;

        for (size_t i = 0; i < _entries.size(); ++i) {
            auto& trigger = _entries[i].trigger;
            const float textWidth = MeasureClayStringWidth(theme, trigger->GetText());
            const float triggerWidth = textWidth + kTriggerLeftPadding + kTriggerRightPadding + kTriggerExtraRightPadding;
            trigger->Layout().sizing = {
                .width = CLAY_SIZING_FIXED(triggerWidth < kMinTriggerWidth ? kMinTriggerWidth : triggerWidth),
                .height = CLAY_SIZING_FIXED(kMenuTriggerHeight)
            };
            trigger->SetEnabled(true);
            trigger->Declare(theme);

            if (trigger->IsPointerOver()) {
                hoveredTopMenuIndex = static_cast<int>(i);
            }
        }

        // Desktop-style behavior: once a top-level menu is open, hovering
        // another trigger switches the open panel without requiring a click.
        if (_openMenuIndex >= 0 && hoveredTopMenuIndex >= 0 && hoveredTopMenuIndex != _openMenuIndex) {
            _openMenuIndex = hoveredTopMenuIndex;
        }
    }

    if (_openMenuIndex >= 0 && _openMenuIndex < static_cast<int>(_entries.size())) {
        auto& openEntry = _entries[_openMenuIndex];
        openEntry.panel->Floating() = Clay_FloatingElementConfig {
            .offset = { 0, 4 },
            .expand = { 0, 0 },
            .parentId = openEntry.trigger->Id().id,
            .zIndex = 20,
            .attachPoints = {
                .element = CLAY_ATTACH_POINT_LEFT_TOP,
                .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM,
            },
            .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE,
            .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID,
            .clipTo = CLAY_CLIP_TO_NONE,
        };
        openEntry.panel->Declare(theme);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
            && !IsPointerOver()
            && !openEntry.panel->IsPointerOver()) {
            _openMenuIndex = -1;
        }
    }
}
