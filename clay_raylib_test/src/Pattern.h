#pragma once

#include "Control.h"
#include "PatternTileGenerator.h"

class Pattern : public Control {
public:
    using PatternStyle = PatternTileGenerator::PatternStyle;

    Pattern() : Pattern(CLAY_ID("pattern")) {}
    explicit Pattern(Clay_ElementId id) : Control(id) {}
    ~Pattern() override;

    Pattern(const Pattern&) = delete;
    Pattern& operator=(const Pattern&) = delete;
    Pattern(Pattern&&) = delete;
    Pattern& operator=(Pattern&&) = delete;

    void SetPatternStyle(PatternStyle style);
    PatternStyle GetPatternStyle() const { return _patternStyle; }

    void SetTileSize(uint16_t tileSize);
    uint16_t GetTileSize() const { return _tileSize; }

    void SetPixelScale(uint16_t pixelScale);
    uint16_t GetPixelScale() const { return _pixelScale; }

protected:
    void Prepare(Theme* theme, BoxStyle& style) override;
    void DeclareContent(Theme* theme) override;

private:
    PatternStyle _patternStyle = PatternStyle::Dots;
    uint16_t _tileSize = 8;
    uint16_t _pixelScale = 3;

    Image _tileImage = {0};
    Texture2D _tileTexture = {0};
    bool _tileTextureLoaded = false;
    bool _tileDirty = true;
    float _lastWidth = 0.0f;
    float _lastHeight = 0.0f;
};
