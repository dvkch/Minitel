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
    USB.begin();
}
