#pragma once

#include "project.h"

class PatternTileGenerator {
public:
    enum class PatternStyle {
        Diagonal,
        CrossHatch,
        Checker,
        Dots,
        HorizontalLines,
    };

    static Image Generate(
        PatternStyle style,
        uint16_t tileSize,
        Clay_Color background,
        Clay_Color foreground
    );

    // Generates a full-size image of the given pixel dimensions, tiling the
    // pattern to fill it. Use this to avoid creating one Clay element per tile.
    static Image GenerateTiled(
        int width,
        int height,
        PatternStyle style,
        uint16_t tileSize,
        Clay_Color background,
        Clay_Color foreground
    );
};
