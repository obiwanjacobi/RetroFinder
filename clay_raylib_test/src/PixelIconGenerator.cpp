#include "PixelIconGenerator.h"

namespace {
    constexpr int kIconSize = PixelIconGenerator::kLogicalIconSize;

    const char* kArrowUp[kIconSize] = {
        "000000010000000",
        "000000101000000",
        "000001000100000",
        "000010000010000",
        "000100000001000",
        "001000000000100",
        "011110000011110",
        "000010000010000",
        "000010000010000",
        "000010000010000",
        "000010000010000",
        "000010000010000",
        "000011111110000",
        "000000000000000",
        "000000000000000",
    };

    const char* kArrowDown[kIconSize] = {
        "000000000000000",
        "000000000000000",
        "000011111110000",
        "000010000010000",
        "000010000010000",
        "000010000010000",
        "000010000010000",
        "000010000010000",
        "011110000011110",
        "001000000000100",
        "000100000001000",
        "000010000010000",
        "000001000100000",
        "000000101000000",
        "000000010000000",
    };

    const char* kArrowLeft[kIconSize] = {
        "000000000000000",
        "000000100000000",
        "000001100000000",
        "000010100000000",
        "000100111111100",
        "001000000000100",
        "010000000000100",
        "100000000000100",
        "010000000000100",
        "001000000000100",
        "000100111111100",
        "000010100000000",
        "000001100000000",
        "000000100000000",
        "000000000000000",
    };

    const char* kArrowRight[kIconSize] = {
        "000000000000000",
        "000000001000000",
        "000000001100000",
        "000000001010000",
        "001111111001000",
        "001000000000100",
        "001000000000010",
        "001000000000001",
        "001000000000010",
        "001000000000100",
        "001111111001000",
        "000000001010000",
        "000000001100000",
        "000000001000000",
        "000000000000000",
    };

    const char* kResizeHandle[kIconSize] = {
        "000000000000000",
        "000000000000000",
        "001111110000000",
        "001000010000000",
        "001000010000000",
        "001000010111100",
        "001000010000100",
        "001111110000100",
        "000010000000100",
        "000010000000100",
        "000010000000100",
        "000010000000100",
        "000011111111100",
        "000000000000000",
        "000000000000000",
    };

    const char** kIconTable[] = {
        kArrowUp,
        kArrowDown,
        kArrowLeft,
        kArrowRight,
        kResizeHandle,
    };

    bool IsIconPixelOn(PixelIconGenerator::IconType icon, int x, int y) {
        const int iconIndex = (int)icon;
        if (iconIndex < 0 || iconIndex >= (int)(sizeof(kIconTable) / sizeof(kIconTable[0]))) {
            return false;
        }
        if (x >= kIconSize || y >= kIconSize)
        {
            return false;
        }

        const char* row = kIconTable[iconIndex][y];
        return row[x] == '1';
    }
}

Image PixelIconGenerator::Generate(
    IconType icon,
    uint16_t logicalSize,
    uint16_t pixelScale,
    Clay_Color foreground,
    Clay_Color background
) {
    (void)logicalSize;

    if (pixelScale == 0) {
        pixelScale = 1;
    }

    const int imageSize = kIconSize * (int)pixelScale;
    Image image = GenImageColor(
        imageSize,
        imageSize,
        ClayColorToRaylibColor(background)
    );

    const Color fg = ClayColorToRaylibColor(foreground);

    for (int y = 0; y < kIconSize; ++y) {
        for (int x = 0; x < kIconSize; ++x) {
            if (!IsIconPixelOn(icon, x, y)) {
                continue;
            }

            for (int py = 0; py < (int)pixelScale; ++py) {
                for (int px = 0; px < (int)pixelScale; ++px) {
                    ImageDrawPixel(
                        &image,
                        x * (int)pixelScale + px,
                        y * (int)pixelScale + py,
                        fg
                    );
                }
            }
        }
    }

    return image;
}
