#pragma once

#include <stdint.h>

// Destination of the HID reports, USB for now
class HidOutput {
public:
    virtual ~HidOutput() = default;
    // return false when the report should be sent again later, e.g. no host connected
    virtual bool sendKeyboard(uint8_t mods, const uint8_t keys[6]) = 0;
    virtual bool sendConsumer(uint16_t usage) = 0;
    virtual bool sendSystem(uint8_t value) = 0;
};
