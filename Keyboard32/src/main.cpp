#include <Arduino.h>
#include "Console.h"
#include "UsbDevice.h"

static void printVersion(Print &out) { out.println("Keyboard32 " KEYBOARD32_VERSION); }

static UsbDevice usb;

static const ConsoleCommand commands[] = {
    { "version", "show the firmware version", printVersion },
};
static Console console(usb.serial, commands, sizeof(commands) / sizeof(commands[0]));

void setup() {
    usb.begin();
}

void loop() {
    console.poll();
    delay(1);
}
