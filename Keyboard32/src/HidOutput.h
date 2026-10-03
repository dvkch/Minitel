#pragma once

#include <stdint.h>

// Destination of the HID reports, USB for now
class HidOutput {
public:
    virtual ~HidOutput() = default;
    // both return false when the report could not be sent, e.g. no host connected
    virtual bool sendKeyboard(uint8_t mods, const uint8_t keys[6]) = 0;
    virtual bool sendConsumer(uint16_t usage) = 0;
};
