#pragma once

#include "project.h"
#include "Button.h"
#include "Theme.h"
#include "Icon.h"

#include <functional>

class IconButton : public Button {
public:
    using ClickHandler = std::function<void()>;
    using IconType = Icon::IconType;

    IconButton() : IconButton(CLAY_ID("button")) {}
    explicit IconButton(Clay_ElementId id)
        : Button(id),
          _icon(CLAY_IDI("button_icon", id.id)) {}
    ~IconButton() override = default;

    IconButton(const IconButton&) = delete;
    IconButton& operator=(const IconButton&) = delete;
    IconButton(IconButton&&) = delete;
    IconButton& operator=(IconButton&&) = delete;

    void SetIcon(IconType icon, uint16_t logicalSize = 15, uint16_t pixelScale = 1);
    void ClearIcon();
    bool HasIcon() const { return _icon.HasIcon(); }

protected:
    void DeclareContent(Theme* theme) override;

private:
    Icon _icon;
};