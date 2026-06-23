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
        uint16_t pixelScale,
        Clay_Color foreground,
        Clay_Color background
    );
};
