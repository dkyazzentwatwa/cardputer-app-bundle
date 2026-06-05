#pragma once

#include <Arduino.h>
#include <M5Cardputer.h>

namespace cypher {

inline void drawCenteredText(const char* text, int y, uint8_t size, uint16_t color, uint16_t bg) {
    auto& display = M5Cardputer.Display;
    display.setTextSize(size);
    display.setTextColor(color, bg);
    int x = (display.width() - display.textWidth(text)) / 2;
    if (x < 0) {
        x = 0;
    }
    display.setCursor(x, y);
    display.print(text);
}

inline void showBitcoinCardWalletSplash() {
    auto& display = M5Cardputer.Display;
    const uint16_t bg = display.color565(250, 248, 255);
    const uint16_t grid = display.color565(217, 205, 250);
    const uint16_t cyan = display.color565(0, 180, 216);
    const uint16_t blue = display.color565(118, 68, 224);
    const uint16_t dim = display.color565(105, 94, 125);

    for (int frame = 0; frame < 18; ++frame) {
        display.fillScreen(bg);

        int offset = frame % 16;
        for (int x = -offset; x < display.width(); x += 16) {
            display.drawFastVLine(x, 0, display.height(), grid);
        }
        for (int y = offset; y < display.height(); y += 12) {
            display.drawFastHLine(0, y, display.width(), grid);
        }

        display.drawRect(14, 18, display.width() - 28, display.height() - 36, blue);
        display.drawRect(18, 22, display.width() - 36, display.height() - 44, cyan);
        display.fillRect(28, 34, display.width() - 56, 2, cyan);
        display.fillRect(28, display.height() - 37, display.width() - 56, 2, blue);

        int pulse = (frame % 9) * 6;
        display.fillCircle(32 + pulse, display.height() - 28, 3, cyan);
        display.drawCircle(display.width() - 35 - pulse, 29, 4, blue);

        drawCenteredText("Bitcoin-Card-Wallet", 54, 2, blue, bg);
        drawCenteredText("by littlehakr", 80, 1, dim, bg);
        drawCenteredText("Securing keys offline", 104, 1, blue, bg);

        delay(28);
    }
}

} // namespace cypher
