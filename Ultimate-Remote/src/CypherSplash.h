#pragma once

#include <M5Cardputer.h>

namespace cypher_splash {

inline uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return M5Cardputer.Display.color565(r, g, b);
}

inline void drawGrid(int offset, uint16_t gridColor) {
    auto& display = M5Cardputer.Display;
    const int width = display.width();
    const int height = display.height();

    for (int x = -24 + offset; x < width; x += 24) {
        display.drawLine(x, 0, x + 18, height, gridColor);
    }
    for (int y = offset / 2; y < height; y += 18) {
        display.drawFastHLine(0, y, width, gridColor);
    }
}

inline void show() {
    auto& display = M5Cardputer.Display;
    const int width = display.width();
    const int height = display.height();
    const uint16_t background = rgb(250, 248, 255);
    const uint16_t gridDim = rgb(217, 205, 250);
    const uint16_t gridBright = rgb(0, 180, 216);
    const uint16_t cyan = rgb(0, 180, 216);
    const uint16_t blue = rgb(118, 68, 224);
    const uint16_t soft = rgb(105, 94, 125);

    for (int frame = 0; frame < 24; ++frame) {
        display.fillScreen(background);
        drawGrid(frame % 24, gridDim);

        const int sweepX = (frame * (width + 36)) / 24 - 18;
        display.drawLine(sweepX, 0, sweepX - 34, height, gridBright);
        display.drawLine(sweepX + 1, 0, sweepX - 33, height, blue);

        const int pulse = frame % 8;
        display.drawRoundRect(6 + pulse, 6 + pulse / 2, width - 12 - pulse * 2,
                              height - 12 - pulse, 5, blue);
        display.drawRoundRect(10, 10, width - 20, height - 20, 4, gridBright);

        display.setTextColor(cyan, background);
        display.setTextSize(2);
        display.setCursor(28, 43);
        display.print("Ultimate-Remote");

        display.setTextColor(soft, background);
        display.setTextSize(1);
        display.setCursor(82, 72);
        display.print("by littlehakr");

        display.fillRect(36, height - 34, width - 72, 18, rgb(237, 230, 255));
        display.drawRect(36, height - 34, width - 72, 18, gridBright);
        display.setTextColor(blue, rgb(237, 230, 255));
        display.setCursor(57, height - 29);
        display.print("IR profiles armed");

        delay(24);
    }

    delay(160);
}

}  // namespace cypher_splash
