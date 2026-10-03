#include <Arduino.h>
#include "Console.h"
#include "KeyLog.h"
#include "Keymap.h"
#include "KeyboardState.h"
#include "Matrix.h"
#include "Pins.h"
#include "StatusLed.h"
#include "UsbDevice.h"

enum class LogMode { Off, Coords, Names, Matrix };

static UsbDevice usb;
static Matrix matrix;
static KeyboardState keyboardState;
static StatusLed led(LED_PIN, LED_COLOR_ORDER_RGB);

static constexpr Color RED          = { 255, 0, 0 };
static constexpr Color YELLOW       = { 255, 140, 0 };
static constexpr Color GREEN        = { 0, 255, 0 };
static constexpr Color BLUE         = { 0, 0, 255 };

// how long the Pi's GPIO 3 is held low to wake it, it only needs a short pulse
static constexpr uint32_t WAKE_PULSE_MS = 200;
static uint32_t wakeStartedAt = 0;
static bool isWaking = false;
static bool isPowerHeld = false;

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

// The line is never driven high: it floats at rest, held high by the Pi's pull-up, and is only pulled low
static void startWake() {
    digitalWrite(PI_WAKE_PIN, LOW);
    pinMode(PI_WAKE_PIN, OUTPUT);
    wakeStartedAt = millis();
    isWaking = true;
}

static void updateWake() {
    if (isWaking && millis() - wakeStartedAt >= WAKE_PULSE_MS) {
        pinMode(PI_WAKE_PIN, INPUT);
        isWaking = false;
    }
}

// the power key is sent over USB while the Pi runs, a halted Pi doesn't listen to USB and is woken through GPIO 3
static void onPowerKey(bool pressed) {
    isPowerHeld = pressed;
    if (pressed && (!usb.isConnected() || usb.isSuspended())) {
        startWake();
    }
}

static void onKey(uint8_t row, uint8_t col, bool pressed) {
    KeyVariant variant = keyboardState.onKey(row, col, pressed);
    if (KEYMAP[row][col].action(variant).system == SYSTEM_POWER) {
        onPowerKey(pressed);
    }
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
    if (isPowerHeld) {
        led.show(BLUE);
    }
    else if (!usb.isConnected()) {
        led.show(RED);
    }
    else if (usb.isSuspended()) {
        led.show(YELLOW, true);
    }
    else if (usb.isNumLockOn()) {
        led.show(GREEN);
    }
    else {
        led.show(YELLOW);
    }
}

void setup() {
    pinMode(PI_WAKE_PIN, INPUT);
    usb.begin();
    matrix.begin();
}

void loop() {
    if (matrix.scan(onKey) && logMode == LogMode::Matrix) {
        matrix.print(usb.serial);
        usb.serial.println();
    }
    keyboardState.flush(usb);
    updateWake();
    updateLed();
    console.poll();
    delay(1);
}
