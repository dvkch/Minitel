#pragma once

#include <USB.h>
#include <USBHIDKeyboard.h>

// Native USB port, seen by the host as a single composite device: a serial port and a keyboard
class UsbDevice {
public:
    USBCDC serial;
    USBHIDKeyboard keyboard;

    void begin();
};
