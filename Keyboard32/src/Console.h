#pragma once

#include <Arduino.h>

struct ConsoleCommand {
    const char *name;
    const char *help;
    void (*run)();
};

// Line based commands over the USB serial port
class Console {
public:
    Console(const ConsoleCommand *commands, size_t count);
    void poll();
    void printHelp() const;

private:
    const ConsoleCommand *commands;
    size_t count;
    String line;

    void execute(const String &name);
};
