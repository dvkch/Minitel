#include <Arduino.h>
#include "Console.h"

static void printHelp();
static void printVersion() { Serial.println("Keyboard32 " KEYBOARD32_VERSION); }

static const ConsoleCommand commands[] = {
    { "help",    "show this help",           printHelp },
    { "version", "show the firmware version", printVersion },
};
static Console console(commands, sizeof(commands) / sizeof(commands[0]));

static void printHelp() { console.printHelp(); }

void setup() {
    Serial.begin(115200);
}

void loop() {
    console.poll();
    delay(1);
}
