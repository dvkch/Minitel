#include <Arduino.h>
#include "Console.h"
#include "Matrix.h"
#include "UsbDevice.h"

enum class LogMode { Off, Coords, Matrix };

static UsbDevice usb;
static Matrix matrix;
static LogMode logMode = LogMode::Off;

static void printVersion(Print &out, const String &) {
    out.println("Keyboard32 " KEYBOARD32_VERSION);
}

static void setLogMode(Print &out, const String &args) {
    if (args == "coords") {
        logMode = LogMode::Coords;
    }
    else if (args == "matrix") {
        logMode = LogMode::Matrix;
    }
    else if (args == "off") {
        logMode = LogMode::Off;
    }
    else {
        out.printf("Unknown log mode '%s', use: log coords|matrix|off\n", args.c_str());
        return;
    }
    out.printf("Logging %s\n", args.c_str());
}

static const ConsoleCommand commands[] = {
    { "version", "show the firmware version",                           printVersion },
    { "log",     "print key presses: log coords, log matrix, log off",  setLogMode },
};
static Console console(usb.serial, commands, sizeof(commands) / sizeof(commands[0]));

static void onKey(uint8_t row, uint8_t col, bool pressed) {
    switch (logMode) {
        case LogMode::Off:
        case LogMode::Matrix:
            break;
        case LogMode::Coords:
            usb.serial.printf("x%uy%u %s\n", row, col, pressed ? "down" : "up");
            break;
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
    console.poll();
    delay(1);
}
