#include "PatternTileGenerator.h"

Image PatternTileGenerator::Generate(
    PatternStyle style,
    uint16_t tileSize,
    Clay_Color background,
    Clay_Color foreground
) {
    if (tileSize == 0) {
        tileSize = 1;
    }

    Color bg = ClayColorToRaylibColor(background);
    Color fg = ClayColorToRaylibColor(foreground);

    Image image = GenImageColor(tileSize, tileSize, bg);

    for (uint16_t y = 0; y < tileSize; ++y) {
        for (uint16_t x = 0; x < tileSize; ++x) {
            bool on = false;
            switch (style) {
                case PatternStyle::Diagonal:
                    on = ((x + y) % 4) == 0;
                    break;
                case PatternStyle::CrossHatch:
                    on = (((x + y) % 4) == 0) || (((x + tileSize - y) % 4) == 0);
                    break;
                case PatternStyle::Checker:
                    on = (((x / 2) + (y / 2)) % 2) == 0;
                    break;
                case PatternStyle::Dots:
                    on = (x % 4 == 1) && (y % 4 == 1);
                    break;
                case PatternStyle::HorizontalLines:
                    on = (y % 4) == 0;
                    break;
            }

            if (on) {
                ImageDrawPixel(&image, x, y, fg);
            }
        }
    }

    return image;
}

Image PatternTileGenerator::GenerateTiled(
    int width,
    int height,
    PatternStyle style,
    uint16_t tileSize,
    Clay_Color background,
    Clay_Color foreground
) {
    if (tileSize == 0) tileSize = 1;
    if (width <= 0)    width = 1;
    if (height <= 0)   height = 1;

    Image tile = Generate(style, tileSize, background, foreground);
    Image full = GenImageColor(width, height, ClayColorToRaylibColor(background));

    for (int y = 0; y < height; y += tileSize) {
        for (int x = 0; x < width; x += tileSize) {
            int drawW = (x + tileSize <= width)  ? tileSize : width  - x;
            int drawH = (y + tileSize <= height) ? tileSize : height - y;
            Rectangle src = { 0, 0, (float)drawW, (float)drawH };
            Rectangle dst = { (float)x, (float)y, (float)drawW, (float)drawH };
            ImageDraw(&full, tile, src, dst, WHITE);
        }
    }

    UnloadImage(tile);
    return full;
}
