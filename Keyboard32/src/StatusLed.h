#pragma once

#include <stdint.h>

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    constexpr bool operator==(const Color &other) const = default;
};

// Single WS2812 LED, driven by the Arduino core without any library
class StatusLed {
public:
    explicit StatusLed(uint8_t pin);
    // writes the color only when it changes, at the LED brightness
    void show(Color color);

private:
    uint8_t pin;
    Color shown;
    bool isShown = false;
};
