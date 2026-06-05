#include <M5Cardputer.h>
#include <Views/CardputerView.h>
#include <Inputs/CardputerInput.h>
#include <Dispatchers/AppDispatcher.h>
#include "CypherSplash.h"
#include <esp_wifi.h>

using namespace dispatchers;

CardputerView display;
CardputerInput input;
AppDispatcher* dispatcher;

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);

    cypher::showBitcoinCardWalletSplash();

    esp_wifi_stop();

    dispatcher = new AppDispatcher(display, input);
    dispatcher->setup();
}

void loop() {
    dispatcher->run();
}
