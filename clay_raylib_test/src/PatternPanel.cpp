#include "PatternPanel.h"

#include <cmath>

PatternPanel::~PatternPanel() {
    if (_tileTextureLoaded) {
        UnloadTexture(_tileTexture);
        _tileTexture = Texture2D{0};
        _tileTextureLoaded = false;
    }
    if (IsImageValid(_tileImage)) {
        UnloadImage(_tileImage);
    }
}

void PatternPanel::SetPatternStyle(PatternStyle style) {
    if (_patternStyle != style) {
        _patternStyle = style;
        _tileDirty = true;
    }
}

void PatternPanel::SetTileSize(uint16_t tileSize) {
    if (tileSize == 0 || tileSize == _tileSize) {
        return;
    }
    _tileSize = tileSize;
    _tileDirty = true;
}

void PatternPanel::SetPixelScale(uint16_t pixelScale) {
    if (pixelScale == 0 || pixelScale == _pixelScale) {
        return;
    }
    _pixelScale = pixelScale;
    _tileDirty = true;
}

void PatternPanel::Prepare(Theme* theme, BoxStyle& style) {
    ContainerControl::Prepare(theme, style);
    // reset background so our pattern is shown (not painted over)
    style.backgroundColor = Clay_Color { 0 };

    Clay_ElementData panelData = Clay_GetElementData(Id());
    float panelWidth  = panelData.found ? panelData.boundingBox.width  : 0.0f;
    float panelHeight = panelData.found ? panelData.boundingBox.height : 0.0f;

    bool sizeKnown   = panelWidth > 0.0f && panelHeight > 0.0f;
    bool sizeChanged = sizeKnown && (panelWidth != _lastPanelWidth || panelHeight != _lastPanelHeight);

    if (sizeKnown && (_tileDirty || sizeChanged)) {
        if (IsImageValid(_tileImage)) UnloadImage(_tileImage);
        if (_tileTextureLoaded) {
            UnloadTexture(_tileTexture);
            _tileTexture = Texture2D{0};
            _tileTextureLoaded = false;
        }

        int textureWidth = CLAY__MAX(1, (int)panelWidth / (int)_pixelScale);
        int textureHeight = CLAY__MAX(1, (int)panelHeight / (int)_pixelScale);
        uint16_t textureTileSize = (uint16_t)CLAY__MAX(1, (int)_tileSize / (int)_pixelScale);

        _tileImage = PatternTileGenerator::GenerateTiled(
            textureWidth, textureHeight,
            _patternStyle, textureTileSize,
            theme->GetBackgroundColor(), theme->GetForegroundColor()
        );
        _tileTexture = LoadTextureFromImage(_tileImage);
        _tileTextureLoaded = IsTextureValid(_tileTexture);
        if (_tileTextureLoaded) {
            SetTextureFilter(_tileTexture, TEXTURE_FILTER_POINT);
        }
        _lastPanelWidth  = panelWidth;
        _lastPanelHeight = panelHeight;
        _tileDirty = false;
    }

    style.image.imageData = _tileTextureLoaded ? &_tileTexture : nullptr;
    style.clip = Clay_ClipElementConfig { .horizontal = true, .vertical = true };
}
