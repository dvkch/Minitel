#pragma once

#include <USB.h>
#include <USBHIDConsumerControl.h>
#include <USBHIDKeyboard.h>
#include "HidOutput.h"

// Native USB port, seen by the host as a single composite device: a serial port and a keyboard with media keys
class UsbDevice : public HidOutput {
public:
    USBCDC serial;
    USBHIDKeyboard keyboard;
    USBHIDConsumerControl consumer;

    void begin();
    bool sendKeyboard(uint8_t mods, const uint8_t keys[6]) override;
    bool sendConsumer(uint16_t usage) override;
};
