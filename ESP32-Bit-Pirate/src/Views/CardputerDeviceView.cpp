#if defined(DEVICE_CARDPUTER)

#include "CardputerDeviceView.h"

void CardputerDeviceView::topBar(const std::string& title, bool /*submenu*/, bool searchBar) {
    constexpr int batteryReservedWidth = 58;
    const int textWidth = M5.Lcd.width() - batteryReservedWidth;

    M5.Lcd.fillRect(0, 0, M5.Lcd.width(), TOP_BAR_SIZE, BACKGROUND_COLOR);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TEXT_COLOR);

    std::string text = searchBar && title.empty() ? "Type to search" : title;
    if (text.length() > 13) {
        text = text.substr(0, 13);
    }

    int x = (textWidth - M5.Lcd.textWidth(text.c_str())) / 2;
    if (x < 0) x = 0;
    M5.Lcd.setCursor(x, 16);
    M5.Lcd.printf(text.c_str());

    drawBatteryIndicator(M5.Lcd.width() - batteryReservedWidth + 5, 7);
}

void CardputerDeviceView::drawBatteryIndicator(int x, int y) {
    int32_t level = M5.Power.getBatteryLevel();
    const bool unavailable = level < 0;
    if (level > 100) level = 100;
    if (level < 0) level = 0;

    const bool charging = M5.Power.isCharging() == m5::Power_Class::is_charging;
    uint16_t color = charging ? TFT_CYAN : TFT_GREEN;
    if (!charging && level < 15) {
        color = TFT_RED;
    } else if (!charging && level < 30) {
        color = TFT_ORANGE;
    }

    M5.Lcd.fillRect(x - 2, y - 2, 55, 16, BACKGROUND_COLOR);
    M5.Lcd.drawRoundRect(x, y, 22, 11, 2, color);
    M5.Lcd.fillRect(x + 22, y + 3, 2, 5, color);
    if (!unavailable) {
        const int fillWidth = (level * 18) / 100;
        if (fillWidth > 0) {
            M5.Lcd.fillRect(x + 2, y + 2, fillWidth, 7, color);
        }
    }

    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(color);
    M5.Lcd.setCursor(x + 28, y + 1);
    if (unavailable) {
        M5.Lcd.printf("--%%");
    } else {
        M5.Lcd.printf("%d%%", static_cast<int>(level));
    }
    M5.Lcd.setTextColor(TEXT_COLOR);
}

#endif // DEVICE_CARDPUTER
