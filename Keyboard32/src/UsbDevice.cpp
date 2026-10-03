#include "UsbDevice.h"

// pid.codes shared vendor ID for open source projects, product ID not registered yet
static constexpr uint16_t USB_VENDOR_ID  = 0x1209;
static constexpr uint16_t USB_PRODUCT_ID = 0x7501;

// written by the USB event task, read from the main loop
static volatile bool suspended = false;
static volatile bool numLockOn = false;

static void onUsbEvent(void *, esp_event_base_t, int32_t id, void *) {
    switch (id) {
        case ARDUINO_USB_SUSPEND_EVENT:
            suspended = true;
            break;
        case ARDUINO_USB_RESUME_EVENT:
        case ARDUINO_USB_STARTED_EVENT:
            suspended = false;
            break;
        case ARDUINO_USB_STOPPED_EVENT:
            suspended = false;
            numLockOn = false;
            break;
        default:
            break;
    }
}

static void onKeyboardEvent(void *, esp_event_base_t, int32_t id, void *data) {
    if (id == ARDUINO_USB_HID_KEYBOARD_LED_EVENT) {
        numLockOn = static_cast<arduino_usb_hid_keyboard_event_data_t *>(data)->numlock;
    }
}

void UsbDevice::begin() {
    // the identity can only be changed before USB is started
    USB.VID(USB_VENDOR_ID);
    USB.PID(USB_PRODUCT_ID);
    USB.manufacturerName("Minitel");
    USB.productName("Minitel Keyboard");

    USB.onEvent(onUsbEvent);
    keyboard.onEvent(ARDUINO_USB_HID_KEYBOARD_LED_EVENT, onKeyboardEvent);

    serial.begin();
    keyboard.begin();
    consumer.begin();
    systemControl.begin();
    USB.begin();
}

bool UsbDevice::sendSystem(uint8_t value) {
    if (!USB) {
        return false;
    }
    if (suspended) {
        return true;
    }
    return value ? systemControl.press(value) : systemControl.release();
}

bool UsbDevice::isConnected() const {
    return USB;
}

bool UsbDevice::isSuspended() const {
    return suspended;
}

bool UsbDevice::isNumLockOn() const {
    return numLockOn;
}

// a host asleep can't receive reports: they are dropped rather than retried
bool UsbDevice::sendKeyboard(uint8_t mods, const uint8_t keys[6]) {
    if (!USB) {
        return false;
    }
    if (suspended) {
        return true;
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
    if (suspended) {
        return true;
    }
    return usage ? consumer.press(usage) : consumer.release();
}
