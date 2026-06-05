#pragma once

#include <M5Cardputer.h>

namespace CypherSplash {

inline void show() {
    auto& display = M5Cardputer.Display;
    const int width = display.width();
    const int height = display.height();
    const uint16_t bg = display.color565(250, 248, 255);
    const uint16_t gridDim = display.color565(217, 205, 250);
    const uint16_t gridBright = display.color565(0, 180, 216);
    const uint16_t cyan = display.color565(0, 180, 216);
    const uint16_t blue = display.color565(118, 68, 224);
    const uint16_t muted = display.color565(105, 94, 125);

    for (int frame = 0; frame < 18; ++frame) {
        display.fillScreen(bg);

        const int offset = (frame * 2) % 16;
        for (int x = -16 + offset; x < width; x += 16) {
            display.drawFastVLine(x, 0, height, gridDim);
        }
        for (int y = -16 + offset; y < height; y += 16) {
            display.drawFastHLine(0, y, width, gridDim);
        }

        const int pulse = frame % 6;
        display.drawRoundRect(12 + pulse, 12 + pulse, width - 24 - (pulse * 2), height - 24 - (pulse * 2), 6, gridBright);
        display.drawRoundRect(16, 16, width - 32, height - 32, 4, blue);

        display.fillRoundRect(28, 78, 184, 18, 4, display.color565(237, 230, 255));
        display.drawRoundRect(28, 78, 184, 18, 4, cyan);
        display.fillRoundRect(34, 84, 118 + (frame % 5), 6, 3, blue);
        display.fillRoundRect(158, 72, 28, 30, 4, display.color565(217, 205, 250));
        display.fillRoundRect(185, 80, 12, 14, 2, cyan);

        display.setTextDatum(middle_center);
        display.setTextSize(2);
        display.setTextColor(blue, bg);
        display.drawString("Esp32-USB-Stick", width / 2, 42);

        display.setTextSize(1);
        display.setTextColor(muted, bg);
        display.drawString("by littlehakr", width / 2, 62);
        display.setTextColor(display.color565(31, 24, 51), bg);
        display.drawString("SD card mass storage bridge", width / 2, 116);

        delay(45);
    }
}

}  // namespace CypherSplash
