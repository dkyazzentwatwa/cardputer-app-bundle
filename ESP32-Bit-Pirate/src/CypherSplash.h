#pragma once

#include <Arduino.h>
#include <M5Unified.h>

namespace CypherSplash {
namespace {
constexpr uint16_t kBackground = 0xffdf;
constexpr uint16_t kGridDark = 0xde7f;
constexpr uint16_t kGridBright = 0x05bb;
constexpr uint16_t kCyan = 0x05bb;
constexpr uint16_t kBlue = 0x723c;
constexpr uint16_t kText = 0x18c6;

inline int16_t centeredX(const char* text, uint8_t textSize, int16_t width) {
    return (width - (static_cast<int16_t>(strlen(text)) * 6 * textSize)) / 2;
}

inline void drawGrid(int16_t width, int16_t height, uint8_t phase) {
    M5.Lcd.fillScreen(kBackground);
    for (int16_t x = -(phase % 16); x < width; x += 16) {
        M5.Lcd.drawFastVLine(x, 0, height, kGridDark);
    }
    for (int16_t y = phase % 12; y < height; y += 12) {
        M5.Lcd.drawFastHLine(0, y, width, kGridDark);
    }
    for (int16_t y = height - 1; y > 0; y -= 24) {
        int16_t inset = (height - y) / 2;
        M5.Lcd.drawLine(inset, y, width - inset, y, kGridBright);
    }
}
}

inline void show() {
    M5.Lcd.setRotation(1);
    const int16_t width = M5.Lcd.width();
    const int16_t height = M5.Lcd.height();

    for (uint8_t frame = 0; frame < 5; ++frame) {
        drawGrid(width, height, frame * 4);
        M5.Lcd.drawRect(8 + frame, 8 + frame, width - 16 - (frame * 2), height - 16 - (frame * 2), frame % 2 ? kBlue : kCyan);
        M5.Lcd.fillRect(18, 30, width - 36, 2, kBlue);
        M5.Lcd.fillRect(18, height - 31, width - 36, 2, kCyan);

        M5.Lcd.setTextColor(kBlue, kBackground);
        M5.Lcd.setTextSize(2);
        M5.Lcd.drawString("ESP32-Bit-Pirate", centeredX("ESP32-Bit-Pirate", 2, width), 42);

        M5.Lcd.setTextColor(kText, kBackground);
        M5.Lcd.setTextSize(1);
        M5.Lcd.drawString("by littlehakr", centeredX("by littlehakr", 1, width), 70);

        M5.Lcd.setTextColor(kBlue, kBackground);
        M5.Lcd.drawString("I2C UART SPI RF tools online", centeredX("I2C UART SPI RF tools online", 1, width), 94);

        delay(70);
    }
    delay(180);
}
}
