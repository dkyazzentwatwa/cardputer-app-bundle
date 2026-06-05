#ifndef CYPHER_SPLASH_H
#define CYPHER_SPLASH_H

#include <Arduino.h>
#include <M5Cardputer.h>

inline void showCypherSplash() {
    auto& display = M5Cardputer.Display;
    const int16_t width = display.width();
    const int16_t height = display.height();
    const uint16_t bg = display.color565(250, 248, 255);
    const uint16_t grid = display.color565(217, 205, 250);
    const uint16_t gridBright = display.color565(0, 180, 216);
    const uint16_t cyan = display.color565(0, 180, 216);
    const uint16_t blue = display.color565(118, 68, 224);
    const uint16_t muted = display.color565(105, 94, 125);

    display.setRotation(1);
    display.setTextDatum(textdatum_t::top_left);

    for (uint8_t frame = 0; frame < 20; ++frame) {
        const int16_t offset = frame % 12;
        display.fillScreen(bg);

        for (int16_t x = -offset; x < width; x += 12) {
            display.drawFastVLine(x, 0, height, grid);
        }
        for (int16_t y = offset; y < height; y += 12) {
            display.drawFastHLine(0, y, width, grid);
        }

        const int16_t sweepX = (frame * (width + 28)) / 19 - 14;
        display.drawFastVLine(sweepX, 8, height - 16, gridBright);
        display.drawFastVLine(sweepX + 1, 8, height - 16, blue);

        display.drawRect(8, 8, width - 16, height - 16, gridBright);
        display.drawRect(11, 11, width - 22, height - 22, blue);

        display.setTextSize(2);
        display.setTextColor(blue, bg);
        display.setCursor(17, 36);
        display.print("Open-Wifi-Scanner");

        display.setTextSize(1);
        display.setTextColor(muted, bg);
        display.setCursor(83, 64);
        display.print("by littlehakr");

        display.setTextColor(blue, bg);
        display.setCursor(53, 91);
        display.print("Scanning open WiFi");
        for (uint8_t dot = 0; dot <= frame % 3; ++dot) {
            display.print(".");
        }

        delay(35);
    }
}

#endif
