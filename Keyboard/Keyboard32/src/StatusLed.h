#pragma once

#include <Arduino.h>

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    constexpr bool operator==(const Color &other) const = default;
};

// Single WS2812 LED, driven by the Arduino core without any library
class StatusLed {
public:
    StatusLed(uint8_t pin, rgb_led_color_order_t order);
    // steady color, or slowly fading in and out when pulsing, to be called on every loop
    void show(Color color, bool isPulsing = false);

private:
    uint8_t pin;
    rgb_led_color_order_t order;
    Color written;
    bool isWritten = false;
};
