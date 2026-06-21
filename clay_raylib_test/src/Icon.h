#pragma once

#include "Control.h"
#include "PixelIconGenerator.h"

class Icon : public Control {
public:
    using IconType = PixelIconGenerator::IconType;

    Icon() : Icon(CLAY_ID("icon")) {}
    explicit Icon(Clay_ElementId id) : Control(id) {}
    ~Icon() override;

    Icon(const Icon&) = delete;
    Icon& operator=(const Icon&) = delete;
    Icon(Icon&&) = delete;
    Icon& operator=(Icon&&) = delete;

    void SetIcon(IconType icon, uint16_t logicalSize = 15, uint16_t pixelScale = 1);
    void ClearIcon();
    bool HasIcon() const { return _hasIcon; }
    float GetPixelSize() const { return (float)(PixelIconGenerator::kLogicalIconSize * _iconPixelScale); }

    // Allows other controls (e.g. Button) to reuse Icon rendering behavior
    // while applying it to their own element style.
    void ApplyToStyle(Theme* theme, BoxStyle& style);

protected:
    void Prepare(Theme* theme, BoxStyle& style) override;
    void DeclareContent(Theme* theme) override;

private:
    bool ColorsEqual(const Clay_Color& a, const Clay_Color& b) const {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }
    void RebuildTexture(const Clay_Color& foregroundColor, const Clay_Color& backgroundColor);

    bool _hasIcon = false;
    IconType _iconType = IconType::ArrowUp;
    uint16_t _iconLogicalSize = 15;
    uint16_t _iconPixelScale = 1;
    bool _iconDirty = true;

    Image _iconImage = {0};
    Texture2D _iconTexture = {0};
    bool _iconTextureLoaded = false;

    Clay_Color _lastIconColor = {0};
    bool _hasLastIconColor = false;
    Clay_Color _lastIconBackground = {0};
    bool _hasLastIconBackground = false;
};
