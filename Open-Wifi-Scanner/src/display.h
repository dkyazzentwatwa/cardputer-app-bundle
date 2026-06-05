#ifndef DISPLAY_H
#define DISPLAY_H

#include <algorithm>
#include <M5Unified.h> 
#include "wifi.h"

#define BACKGROUND_COLOR 0xffdf
#define PRIMARY_COLOR 0x723c
#define ACCENT_COLOR 0x05bb
#define RECT_COLOR_DARK 0xef3f
#define RECT_COLOR_LIGHT 0xde7f
#define TEXT_COLOR 0x18c6
#define TEXT_COLOR_ALT 0x6aef
#define ON_PRIMARY_COLOR TFT_WHITE

void displayInit();
void displayWelcome();
void displayLoading();
void displaySetBrightness(uint8_t brightness);
void displayTopBar(bool state);
void displayVolume(uint8_t volume);
void drawBatteryIndicator(int x, int y);
void displayBottomBar();
void displayList(const std::vector<WiFiNetwork>& list);
void displayWifiCount(std::vector<WiFiNetwork> networks);
void drawWifiLogo(int centerX, int centerY, int radius, uint16_t iconColor);
void fillArc(int x, int y, int r1, int r2, int startAngle, int endAngle, uint16_t color);

#endif
