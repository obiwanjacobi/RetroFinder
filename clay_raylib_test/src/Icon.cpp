#include "Icon.h"

Icon::~Icon() {
    if (_iconTextureLoaded) {
        UnloadTexture(_iconTexture);
        _iconTexture = Texture2D{0};
        _iconTextureLoaded = false;
    }
    if (IsImageValid(_iconImage)) {
        UnloadImage(_iconImage);
    }
}

void Icon::SetIcon(IconType icon, uint16_t logicalSize, uint16_t pixelScale) {
    if (logicalSize == 0) {
        logicalSize = 1;
    }
    if (pixelScale == 0) {
        pixelScale = 1;
    }

    const bool changed =
        !_hasIcon ||
        _iconType != icon ||
        _iconLogicalSize != logicalSize ||
        _iconPixelScale != pixelScale;

    _hasIcon = true;
    _iconType = icon;
    _iconLogicalSize = logicalSize;
    _iconPixelScale = pixelScale;

    if (changed) {
        _iconDirty = true;
    }
}

void Icon::ClearIcon() {
    _hasIcon = false;
}

void Icon::RebuildTexture(const Clay_Color& foregroundColor, const Clay_Color& backgroundColor) {
    if (IsImageValid(_iconImage)) {
        UnloadImage(_iconImage);
    }
    if (_iconTextureLoaded) {
        UnloadTexture(_iconTexture);
        _iconTexture = Texture2D{0};
        _iconTextureLoaded = false;
    }

    _iconImage = PixelIconGenerator::Generate(
        _iconType,
        _iconLogicalSize,
        _iconPixelScale,
        foregroundColor,
        backgroundColor
    );
    _iconTexture = LoadTextureFromImage(_iconImage);
    _iconTextureLoaded = IsTextureValid(_iconTexture);
    if (_iconTextureLoaded) {
        SetTextureFilter(_iconTexture, TEXTURE_FILTER_POINT);
    }

    _lastIconColor = foregroundColor;
    _hasLastIconColor = true;
    _lastIconBackground = backgroundColor;
    _hasLastIconBackground = true;
    _iconDirty = false;
}

void Icon::ApplyToStyle(Theme* theme, BoxStyle& style) {
    if (!_hasIcon) {
        return;
    }

    const Clay_Color fg = theme->GetForegroundColor();
    const Clay_Color bg = style.backgroundColor;
    const bool colorChanged = !_hasLastIconColor || !ColorsEqual(fg, _lastIconColor);
    const bool backgroundChanged = !_hasLastIconBackground || !ColorsEqual(bg, _lastIconBackground);
    if (_iconDirty || colorChanged || backgroundChanged || !_iconTextureLoaded) {
        RebuildTexture(fg, bg);
    }

    // Keep icon visible even if rectangle/image command ordering changes.
    style.backgroundColor = Clay_Color { 0 };
    style.image.imageData = _iconTextureLoaded ? &_iconTexture : nullptr;
}

void Icon::Prepare(Theme* theme, BoxStyle& style) {
    Control::Prepare(theme, style);

    if (!HasBackgroundColor()) {
        style.backgroundColor = theme->GetBackgroundColor();
    }

    ApplyToStyle(theme, style);
}

void Icon::DeclareContent(Theme* theme) {
    (void)theme;
}
