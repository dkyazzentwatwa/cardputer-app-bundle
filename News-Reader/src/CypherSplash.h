#pragma once

#include <M5Cardputer.h>

static inline void showCypherSplash() {
    constexpr uint16_t bg = 0xffdf;
    constexpr uint16_t primary = 0x723c;
    constexpr uint16_t accent = 0x05bb;
    constexpr uint16_t text = 0x18c6;
    constexpr uint16_t muted = 0x6aef;

    M5.Lcd.fillScreen(bg);
    M5.Lcd.setTextDatum(middle_center);
    M5.Lcd.setTextColor(primary, bg);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawString("CYPHER", 120, 50);
    M5.Lcd.drawFastHLine(76, 66, 88, accent);
    M5.Lcd.setTextColor(muted, bg);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("News Reader", 120, 82);
    M5.Lcd.setTextColor(text, bg);
    delay(800);
    M5.Lcd.setTextDatum(top_left);
}
