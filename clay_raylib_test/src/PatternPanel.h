#pragma once

#include "project.h"
#include "ContainerControl.h"
#include "PatternTileGenerator.h"

class PatternPanel : public ContainerControl {
public:
    using PatternStyle = PatternTileGenerator::PatternStyle;

    PatternPanel() : PatternPanel(CLAY_ID("patternpanel")) {}
    explicit PatternPanel(Clay_ElementId id) : ContainerControl(id) {}
    ~PatternPanel() override;

    PatternPanel(const PatternPanel&) = delete;
    PatternPanel& operator=(const PatternPanel&) = delete;
    PatternPanel(PatternPanel&&) = delete;
    PatternPanel& operator=(PatternPanel&&) = delete;

    void SetPatternStyle(PatternStyle style);
    PatternStyle GetPatternStyle() const { return _patternStyle; }

    void SetTileSize(uint16_t tileSize);
    uint16_t GetTileSize() const { return _tileSize; }

    void SetPixelScale(uint16_t pixelScale);
    uint16_t GetPixelScale() const { return _pixelScale; }

private:
    void Prepare(Theme* theme, BoxStyle& style) override;

    PatternStyle _patternStyle = PatternStyle::Dots;
    uint16_t _tileSize = 8;
    uint16_t _pixelScale = 3;

    Image _tileImage = {0};
    Texture2D _tileTexture = {0};
    bool _tileTextureLoaded = false;
    bool _tileDirty = true;
    float _lastPanelWidth = 0.0f;
    float _lastPanelHeight = 0.0f;
};