#include "Pattern.h"

Pattern::~Pattern() {
    if (_tileTextureLoaded) {
        UnloadTexture(_tileTexture);
        _tileTexture = Texture2D{0};
        _tileTextureLoaded = false;
    }
    if (IsImageValid(_tileImage)) {
        UnloadImage(_tileImage);
    }
}

void Pattern::SetPatternStyle(PatternStyle style) {
    if (_patternStyle != style) {
        _patternStyle = style;
        _tileDirty = true;
    }
}

void Pattern::SetTileSize(uint16_t tileSize) {
    if (tileSize == 0 || tileSize == _tileSize) {
        return;
    }
    _tileSize = tileSize;
    _tileDirty = true;
}

void Pattern::SetPixelScale(uint16_t pixelScale) {
    if (pixelScale == 0 || pixelScale == _pixelScale) {
        return;
    }
    _pixelScale = pixelScale;
    _tileDirty = true;
}

void Pattern::Prepare(Theme* theme, BoxStyle& style) {
    Control::Prepare(theme, style);
    ApplyToStyle(theme, style);
}

void Pattern::ApplyToStyle(Theme* theme, BoxStyle& style) {
    float width = _renderWidth;
    float height = _renderHeight;

    if (!_hasRenderSizeOverride) {
        Clay_ElementData patternData = Clay_GetElementData(Id());
        width = patternData.found ? patternData.boundingBox.width : 0.0f;
        height = patternData.found ? patternData.boundingBox.height : 0.0f;
    }

    ApplyPatternForSize(theme, style, width, height);
}

void Pattern::ApplyPatternForSize(Theme* theme, BoxStyle& style, float width, float height) {
    // Keep background transparent so only the generated pattern is visible.
    style.backgroundColor = Clay_Color { 0 };

    bool sizeKnown = width > 0.0f && height > 0.0f;
    bool sizeChanged = sizeKnown && (width != _lastWidth || height != _lastHeight);

    if (sizeKnown && (_tileDirty || sizeChanged)) {
        if (IsImageValid(_tileImage)) {
            UnloadImage(_tileImage);
        }
        if (_tileTextureLoaded) {
            UnloadTexture(_tileTexture);
            _tileTexture = Texture2D{0};
            _tileTextureLoaded = false;
        }

        int textureWidth = CLAY__MAX(1, (int)width / (int)_pixelScale);
        int textureHeight = CLAY__MAX(1, (int)height / (int)_pixelScale);
        uint16_t textureTileSize = (uint16_t)CLAY__MAX(1, (int)_tileSize / (int)_pixelScale);

        _tileImage = PatternTileGenerator::GenerateTiled(
            textureWidth,
            textureHeight,
            _patternStyle,
            textureTileSize,
            theme->GetBackgroundColor(),
            theme->GetForegroundColor()
        );
        _tileTexture = LoadTextureFromImage(_tileImage);
        _tileTextureLoaded = IsTextureValid(_tileTexture);
        if (_tileTextureLoaded) {
            SetTextureFilter(_tileTexture, TEXTURE_FILTER_POINT);
        }

        _lastWidth = width;
        _lastHeight = height;
        _tileDirty = false;
    }

    style.image.imageData = _tileTextureLoaded ? &_tileTexture : nullptr;
    style.clip = Clay_ClipElementConfig { .horizontal = true, .vertical = true };
}

void Pattern::DeclareContent(Theme* theme) {
    (void)theme;
}
