#ifndef CYPHER_SPLASH_H
#define CYPHER_SPLASH_H

#include <M5Cardputer.h>

static inline void cypherSplashShow() {
  auto& display = M5Cardputer.Display;
  const int width = display.width();
  const int height = display.height();
  const uint16_t bg = display.color565(2, 7, 16);
  const uint16_t grid = display.color565(0, 48, 76);
  const uint16_t cyan = display.color565(0, 224, 255);
  const uint16_t blue = display.color565(24, 112, 255);
  const uint16_t dim = display.color565(84, 154, 184);

  display.setRotation(1);
  display.setSwapBytes(false);
  display.setTextDatum(middle_center);
  display.setFont(&fonts::Font0);

  for (int frame = 0; frame < 18; ++frame) {
    display.fillScreen(bg);

    const int shift = (frame * 3) % 16;
    for (int x = -shift; x < width; x += 16) {
      display.drawFastVLine(x, 0, height, grid);
    }
    for (int y = shift - 16; y < height; y += 16) {
      display.drawFastHLine(0, y, width, grid);
    }

    const int sweepX = (frame * 17) % (width + 40) - 20;
    display.drawLine(sweepX, 0, sweepX - 44, height, blue);
    display.drawLine(sweepX + 1, 0, sweepX - 43, height, cyan);

    for (int i = 0; i < 5; ++i) {
      const int dotX = (frame * 9 + i * 47) % width;
      const int dotY = 18 + ((i * 23 + frame * 5) % (height - 36));
      display.fillCircle(dotX, dotY, 1 + (i % 2), (i % 2) ? blue : cyan);
    }

    display.drawRect(6, 6, width - 12, height - 12, blue);
    display.drawRect(9, 9, width - 18, height - 18, grid);

    display.setTextSize(1);
    display.setTextColor(cyan, bg);
    display.drawCenterString("Cardputer-Game-Station-Emulators", width / 2, 42);
    display.setTextColor(dim, bg);
    display.drawCenterString("by littlehakr", width / 2, 63);
    display.setTextColor(blue, bg);
    display.drawCenterString("Cores warming. SD ROM scan next.", width / 2, 94);
    delay(24);
  }

  display.setTextDatum(top_left);
}

#endif
