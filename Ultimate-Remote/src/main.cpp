#include <M5Cardputer.h>
#include "Views/CardputerView.h"
#include "Inputs/CardputerInput.h"
#include "Dispatchers/AppDispatcher.h"
#include "CypherSplash.h"

using namespace dispatchers;

CardputerView display;
CardputerInput input;

AppDispatcher dispatcher(display, input);

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    cypher_splash::show();

    CardputerView display = CardputerView();
    CardputerInput input = CardputerInput();

    AppDispatcher dispatcher = AppDispatcher(display, input);
    dispatcher.setup();
}

void loop() {
    dispatcher.run();
}
