#include <Arduino.h>
#include <USB.h>
#include <USBMSC.h>
#include <SPI.h>
#include <SD.h>
#include <M5Unified.h>
#include <M5Cardputer.h>
#include "CypherSplash.h"
#include "CypherLauncherReturn.h"

// MSC Object
USBMSC msc;

// SPI Instance
SPIClass sdSPI;

// Cardputer SD Card pins
constexpr int sdCardCSPin   = 12;
constexpr int sdCardMISOPin = 39;
constexpr int sdCardMOSIPin = 14;
constexpr int sdCardCLKPin  = 40;

constexpr uint16_t kCypherBg = 0xffdf;
constexpr uint16_t kCypherSurface = 0xef3f;
constexpr uint16_t kCypherSurfaceStrong = 0xde7f;
constexpr uint16_t kCypherPrimary = 0x723c;
constexpr uint16_t kCypherAccent = 0x05bb;
constexpr uint16_t kCypherText = 0x18c6;
constexpr uint16_t kCypherMuted = 0x6aef;
constexpr uint16_t kCypherOnPrimary = TFT_WHITE;

void drawUSBStickIcon() {
    // Port USB
    M5.Lcd.fillRoundRect(155, 55, 40, 40, 5, kCypherSurfaceStrong);
    // Body
    M5.Lcd.fillRoundRect(45, 50, 112, 50, 5, kCypherPrimary);
    // Small square on port
    M5.Lcd.fillRoundRect(164, 63, 20, 10, 5, kCypherMuted);
    M5.Lcd.fillRoundRect(164, 78, 20, 10, 5, kCypherMuted);
}

void drawLedIndicator(bool plugged) {
    M5.Lcd.fillRoundRect(54, 60, 5, 30, 5, plugged ? GREEN : RED);
}

void drawBatteryIndicator(int x, int y) {
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

    M5.Lcd.fillRect(x - 2, y - 2, 55, 16, kCypherBg);
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
        M5.Lcd.print("--%");
    } else {
        M5.Lcd.printf("%d%%", static_cast<int>(level));
    }
    M5.Lcd.setTextColor(kCypherText, kCypherBg);
}

void displayWelcome() {
    // Title
    M5.Lcd.fillScreen(kCypherBg);
    M5.Lcd.setTextSize(1.8);
    M5.Lcd.setCursor(20, 17);
    M5.Lcd.setTextColor(kCypherText, kCypherBg);
    M5.Lcd.print("SD Card -> USB Stick");
    drawBatteryIndicator(184, 7);
    // Icon
    drawUSBStickIcon();
    // Press Mention
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(62, 71);
    M5.Lcd.setTextColor(kCypherOnPrimary, kCypherPrimary);
    M5.Lcd.print("PRESS ANY KEY");
    // Version
    M5.Lcd.setCursor(69, 120);
    M5.Lcd.setTextColor(kCypherMuted, kCypherBg);
    M5.Lcd.print("Version 0.1 - Geo");
    // Wait Press
    while(true){
        M5Cardputer.update();
        if (M5Cardputer.Keyboard.isChange()) {
            break;
        }
        delay(5);
    }
    M5.Lcd.fillScreen(kCypherBg);
}

void displayMessage(std::string message) {
    M5.Lcd.fillRect(63, 28, 120, 17, kCypherBg);
    M5.Lcd.setCursor(66, 30);
    M5.Lcd.setTextColor(kCypherText, kCypherBg);
    M5.Lcd.print(message.c_str());
}

int32_t usbWriteCallback(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
    // Verify freespace
    uint64_t freeSpace = SD.totalBytes() - SD.usedBytes();
    if (bufsize > freeSpace) {
        return -1; // no space available
    }

    // Verify sector size
    const uint32_t secSize = SD.sectorSize();
    if (secSize == 0) return -1;  // disk error

    // Write blocs
    for (uint32_t x = 0; x < bufsize / secSize; ++x) {
        uint8_t blkBuffer[secSize];
        memcpy(blkBuffer, buffer + secSize * x, secSize);
        if (!SD.writeRAW(blkBuffer, lba + x)) {
            return -1;  // write error
        }
    }
    return bufsize;
}

int32_t usbReadCallback(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    // Verify sector size
    const uint32_t secSize = SD.sectorSize();
    if (secSize == 0) return -1;  // disk error

    // Read blocs
    for (uint32_t x = 0; x < bufsize / secSize; ++x) {
        if (!SD.readRAW(reinterpret_cast<uint8_t*>(buffer) + (x * secSize), lba + x)) {
            return -1;  // read error
        }
    }
    return bufsize;
}

bool usbStartStopCallback(uint8_t power_condition, bool start, bool load_eject) {
    return true;
}

void setupUsbCallback() {
  // USB MSC
  uint32_t secSize = SD.sectorSize();
  uint32_t numSectors = SD.numSectors();
  msc.vendorID("ESP32");
  msc.productID("USB_MSC");
  msc.productRevision("1.0");
  msc.onRead(usbReadCallback);
  msc.onWrite(usbWriteCallback);
  msc.onStartStop(usbStartStopCallback);
  msc.mediaPresent(true);
  msc.begin(numSectors, secSize);
}

void setupUsbEvent() {
  USB.onEvent([](void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == ARDUINO_USB_EVENTS) {
      auto* data = reinterpret_cast<arduino_usb_event_data_t*>(event_data);
      switch (event_id) {
        case ARDUINO_USB_STARTED_EVENT:
          drawLedIndicator(true);
          break;
        case ARDUINO_USB_STOPPED_EVENT:
          drawLedIndicator(false);
          break;
        case ARDUINO_USB_SUSPEND_EVENT:
          displayMessage("USB suspend");
          break;
        case ARDUINO_USB_RESUME_EVENT:
          displayMessage("USB resume");
          break;
        default:
          break;
      }
    }
  });
}

void setup() {
  auto cfg = M5.config();
  M5Cardputer.begin(cfg);
  M5Cardputer.Display.setRotation(1);
  CypherSplash::show();

  // Welcome screen and wait input
  displayWelcome();

  // SPI
  sdSPI.begin(sdCardCLKPin, sdCardMISOPin, sdCardMOSIPin, sdCardCSPin);

  // SD
  while(!SD.begin(sdCardCSPin, sdSPI)) {
    displayMessage("No SD Card found !");
    delay(1000);
  }

  // USB
  setupUsbCallback();
  setupUsbEvent();
  displayMessage("USB Stick is ready");
  drawBatteryIndicator(184, 7);
  drawUSBStickIcon();
  drawLedIndicator(false);
  USB.begin();
}

void loop() {
  M5Cardputer.update();
  Keyboard_Class::KeysState keys = M5Cardputer.Keyboard.keysState();
  if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed() && keys.fn && keys.del) {
    CypherLauncherReturn::returnToLauncher();
  }
  delay(1);
}
