#pragma once

#include "project.h"

class PixelIconGenerator {
public:
    static constexpr uint16_t kLogicalIconSize = 15;

    enum class IconType {
        ArrowUp,
        ArrowDown,
        ArrowLeft,
        ArrowRight,
        ResizeHandle,
    };

    static Image Generate(
        IconType icon,
        // Reserved for compatibility; icons are generated from fixed-size tables.
        uint16_t logicalSize,
        uint16_t pixelScale,
        Clay_Color foreground,
        Clay_Color background
    );
};
