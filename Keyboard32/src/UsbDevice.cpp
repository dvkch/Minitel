#include "UsbDevice.h"

// pid.codes shared vendor ID for open source projects, product ID not registered yet
static constexpr uint16_t USB_VENDOR_ID  = 0x1209;
static constexpr uint16_t USB_PRODUCT_ID = 0x7501;

void UsbDevice::begin() {
    // the identity can only be changed before USB is started
    USB.VID(USB_VENDOR_ID);
    USB.PID(USB_PRODUCT_ID);
    USB.manufacturerName("Minitel");
    USB.productName("Minitel Keyboard");

    serial.begin();
    keyboard.begin();
    consumer.begin();
    USB.begin();
}

bool UsbDevice::sendKeyboard(uint8_t mods, const uint8_t keys[6]) {
    if (!USB) {
        return false;
    }
    KeyReport report = { mods, 0, {} };
    memcpy(report.keys, keys, sizeof(report.keys));
    keyboard.sendReport(&report);
    return true;
}

bool UsbDevice::sendConsumer(uint16_t usage) {
    if (!USB) {
        return false;
    }
    return usage ? consumer.press(usage) : consumer.release();
}
