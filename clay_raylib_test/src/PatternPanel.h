#pragma once

#include "project.h"
#include "Control.h"
#include "PatternTileGenerator.h"

#include <vector>

class PatternPanel : public Control {
public:
    using PatternStyle = PatternTileGenerator::PatternStyle;

    PatternPanel() : PatternPanel(CLAY_ID("patternpanel")) {}
    PatternPanel(Clay_ElementId id);
    ~PatternPanel() override;

    PatternPanel(const PatternPanel&) = delete;
    PatternPanel& operator=(const PatternPanel&) = delete;
    PatternPanel(PatternPanel&&) = delete;
    PatternPanel& operator=(PatternPanel&&) = delete;

    void Declare(Theme* theme) override;

    Clay_LayoutConfig& Layout() { return _layout; }

    void Add(Control* control) { _controls.push_back(control); }

    void SetPatternStyle(PatternStyle style);
    PatternStyle GetPatternStyle() const { return _patternStyle; }

    void SetTileSize(uint16_t tileSize);
    uint16_t GetTileSize() const { return _tileSize; }

    void SetPixelScale(uint16_t pixelScale);
    uint16_t GetPixelScale() const { return _pixelScale; }

private:
    Clay_ElementId _id;
    Clay_LayoutConfig _layout;
    std::vector<Control*> _controls;

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