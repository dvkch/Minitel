#include <Arduino.h>
#include "Console.h"
#include "KeyLog.h"
#include "KeyboardState.h"
#include "Matrix.h"
#include "Pins.h"
#include "StatusLed.h"
#include "UsbDevice.h"

enum class LogMode { Off, Coords, Names, Matrix };

static UsbDevice usb;
static Matrix matrix;
static KeyboardState keyboardState;
static StatusLed led(LED_PIN);

static constexpr Color RED          = { 255, 0, 0 };
static constexpr Color YELLOW       = { 255, 140, 0 };
static constexpr Color DIM_YELLOW   = { 64, 35, 0 };
static constexpr Color GREEN        = { 0, 255, 0 };

static LogMode logMode = LogMode::Off;
static uint8_t pressedCount = 0;

static void printVersion(Print &out, const String &) {
    out.println("Keyboard32 " KEYBOARD32_VERSION);
}

static void setLogMode(Print &out, const String &args) {
    if (args == "coords") {
        logMode = LogMode::Coords;
    }
    else if (args == "names") {
        logMode = LogMode::Names;
    }
    else if (args == "matrix") {
        logMode = LogMode::Matrix;
    }
    else if (args == "off") {
        logMode = LogMode::Off;
    }
    else {
        out.printf("Unknown log mode '%s', use: log coords|names|matrix|off\n", args.c_str());
        return;
    }
    out.printf("Logging %s\n", args.c_str());
}

static const ConsoleCommand commands[] = {
    { "version", "show the firmware version",                                       printVersion },
    { "log",     "print key presses: log coords, log names, log matrix, log off",   setLogMode },
};
static Console console(usb.serial, commands, sizeof(commands) / sizeof(commands[0]));

static void onKey(uint8_t row, uint8_t col, bool pressed) {
    KeyVariant variant = keyboardState.onKey(row, col, pressed);
    if (pressed) {
        pressedCount++;
    }
    else if (pressedCount > 0) {
        pressedCount--;
    }

    switch (logMode) {
        case LogMode::Off:
        case LogMode::Matrix:
            return;
        case LogMode::Coords:
            printKeyCoords(usb.serial, row, col, pressed);
            break;
        case LogMode::Names:
            printKeyName(usb.serial, row, col, pressed, variant);
            break;
    }
    // separates sequences of keys pressed together
    if (pressedCount == 0) {
        usb.serial.println("-----");
    }
}

static void updateLed() {
    if (!usb.isConnected()) {
        led.show(RED);
    }
    else if (usb.isSuspended()) {
        led.show(DIM_YELLOW);
    }
    else if (usb.isNumLockOn()) {
        led.show(GREEN);
    }
    else {
        led.show(YELLOW);
    }
}

void setup() {
    usb.begin();
    matrix.begin();
}

void loop() {
    if (matrix.scan(onKey) && logMode == LogMode::Matrix) {
        matrix.print(usb.serial);
        usb.serial.println();
    }
    keyboardState.flush(usb);
    updateLed();
    console.poll();
    delay(1);
}
