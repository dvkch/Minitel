#include <math.h>
#include "StatusLed.h"

// a full brightness WS2812 is blinding behind the Minitel's small window
static constexpr uint8_t BRIGHTNESS_PERCENT = 20;
// one breath in and out, like the sleep light of 2009 MacBooks
static constexpr uint32_t PULSE_PERIOD_MS = 5000;

StatusLed::StatusLed(uint8_t pin, rgb_led_color_order_t order) : pin(pin), order(order), written({}) {}

void StatusLed::show(Color color, bool isPulsing) {
    float level = BRIGHTNESS_PERCENT / 100.0f;
    if (isPulsing) {
        float phase = (millis() % PULSE_PERIOD_MS) / float(PULSE_PERIOD_MS);
        level *= 0.5f - 0.5f * cosf(2 * PI * phase);
    }

    Color scaled = { uint8_t(color.red * level), uint8_t(color.green * level), uint8_t(color.blue * level) };
    if (isWritten && scaled == written) {
        return;
    }
    rgbLedWriteOrdered(pin, order, scaled.red, scaled.green, scaled.blue);
    written = scaled;
    isWritten = true;
}
