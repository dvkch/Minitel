#include <Arduino.h>
#include "StatusLed.h"

// a full brightness WS2812 is blinding behind the Minitel's small window
static constexpr uint8_t BRIGHTNESS_PERCENT = 20;

static uint8_t scaled(uint8_t value) {
    return value * BRIGHTNESS_PERCENT / 100;
}

StatusLed::StatusLed(uint8_t pin) : pin(pin), shown({}) {}

void StatusLed::show(Color color) {
    if (isShown && color == shown) {
        return;
    }
    rgbLedWrite(pin, scaled(color.red), scaled(color.green), scaled(color.blue));
    shown = color;
    isShown = true;
}
