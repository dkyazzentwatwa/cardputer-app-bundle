#pragma once

#include <Arduino.h>
#include <M5Cardputer.h>

namespace cypher_splash {

static constexpr uint16_t kBackground = 0xffdf;
static constexpr uint16_t kGridDark = 0xde7f;
static constexpr uint16_t kGridBright = 0x05bb;
static constexpr uint16_t kCyan = 0x05bb;
static constexpr uint16_t kBlue = 0x723c;
static constexpr uint16_t kPanel = 0xef3f;
static constexpr uint16_t kText = 0x18c6;

inline void drawGrid(M5GFX& display, int offset) {
    const int width = display.width();
    const int height = display.height();

    for (int x = -16 + offset; x < width; x += 16) {
        display.drawLine(x, 0, x, height, kGridDark);
    }

    for (int y = -16 + offset; y < height; y += 16) {
        display.drawLine(0, y, width, y, kGridDark);
    }

    display.drawLine(0, height - 24, width, height - 56, kBlue);
    display.drawLine(0, height - 8, width, height - 40, kGridBright);
}

inline void drawVaultMark(M5GFX& display, int centerX, int topY, uint16_t color) {
    const int bodyX = centerX - 22;
    const int bodyY = topY + 19;

    display.drawRoundRect(centerX - 14, topY, 28, 24, 10, color);
    display.fillRoundRect(bodyX, bodyY, 44, 28, 4, kPanel);
    display.drawRoundRect(bodyX, bodyY, 44, 28, 4, color);
    display.fillCircle(centerX, bodyY + 13, 4, color);
    display.drawFastVLine(centerX, bodyY + 16, 7, color);
    display.drawFastHLine(bodyX + 8, bodyY + 8, 28, kBlue);
}

inline void show(const char* statusLine = "Vault locked - SD vaults ready") {
    auto& display = M5Cardputer.Display;

    display.setRotation(1);
    display.setFont(&fonts::Font0);
    display.setTextDatum(middle_center);

    for (int frame = 0; frame < 18; ++frame) {
        const int pulse = frame % 8;
        const uint16_t accent = pulse < 4 ? kCyan : kBlue;

        display.fillScreen(kBackground);
        drawGrid(display, frame % 16);

        display.drawRoundRect(5, 5, display.width() - 10, display.height() - 10, 8, accent);
        display.drawRoundRect(8, 8, display.width() - 16, display.height() - 16, 6, kBlue);

        drawVaultMark(display, display.width() / 2, 18 + (pulse < 4 ? 0 : 1), accent);

        display.setTextColor(kText, kBackground);
        display.setTextSize(2);
        display.drawString("Password-Manager", display.width() / 2, 82);

        display.setTextSize(1);
        display.setTextColor(kBlue, kBackground);
        display.drawString("by littlehakr", display.width() / 2, 103);

        display.setTextColor(kText, kBackground);
        display.drawString(statusLine, display.width() / 2, 121);

        delay(35);
    }

    delay(120);
}

}  // namespace cypher_splash
