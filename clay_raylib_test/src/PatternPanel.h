#pragma once

#include "project.h"
#include "ContainerControl.h"
#include "Pattern.h"

class PatternPanel : public ContainerControl {
public:
    using PatternStyle = Pattern::PatternStyle;

    PatternPanel() : PatternPanel(CLAY_ID("patternpanel")) {}
    explicit PatternPanel(Clay_ElementId id);
    ~PatternPanel() override = default;

    PatternPanel(const PatternPanel&) = delete;
    PatternPanel& operator=(const PatternPanel&) = delete;
    PatternPanel(PatternPanel&&) = delete;
    PatternPanel& operator=(PatternPanel&&) = delete;

    void SetPatternStyle(PatternStyle style) { _pattern.SetPatternStyle(style); }
    PatternStyle GetPatternStyle() const { return _pattern.GetPatternStyle(); }

    void SetTileSize(uint16_t tileSize) { _pattern.SetTileSize(tileSize); }
    uint16_t GetTileSize() const { return _pattern.GetTileSize(); }

    void SetPixelScale(uint16_t pixelScale) { _pattern.SetPixelScale(pixelScale); }
    uint16_t GetPixelScale() const { return _pattern.GetPixelScale(); }

protected:
    void Prepare(Theme* theme, BoxStyle& style) override;
    //void DeclareContent(Theme* theme) override { (void)theme; }

private:
    Pattern _pattern;
};