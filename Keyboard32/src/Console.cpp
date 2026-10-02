#include "Console.h"

Console::Console(const ConsoleCommand *commands, size_t count) : commands(commands), count(count) {}

void Console::poll() {
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\r' || c == '\n') {
            line.trim();
            execute(line);
            line = "";
        }
        else if (line.length() < 64) {
            line += c;
        }
    }
}

void Console::printHelp() const {
    Serial.println("Commands:");
    for (size_t i = 0; i < count; i++) {
        Serial.printf("  %-10s %s\n", commands[i].name, commands[i].help);
    }
}

void Console::execute(const String &name) {
    if (name.isEmpty()) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        if (name == commands[i].name) {
            commands[i].run();
            return;
        }
    }
    Serial.printf("Unknown command '%s', type 'help'\n", name.c_str());
}
