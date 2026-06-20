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

void PatternPanel::Declare(Theme* theme) {
    Clay_ElementData panelData = Clay_GetElementData(_id);
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

    Clay_ImageElementConfig imageConfig = {};
    imageConfig.imageData = _tileTextureLoaded ? &_tileTexture : nullptr;

    CLAY(_id, {
            .layout = _layout,
            .backgroundColor = theme->GetBackgroundColor(),
            .image = imageConfig,
            .clip = { .horizontal = true, .vertical = true },
            //.border = { .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(5) },
        }) {
        for (auto* control : _controls) {
            control->Declare(theme);
        }
    }
}
