#pragma once

#include <Arduino.h>

struct ConsoleCommand {
    const char *name;
    const char *help;
    void (*run)(Print &out);
};

// Line based commands over a serial stream, `help` is always available
class Console {
public:
    Console(Stream &stream, const ConsoleCommand *commands, size_t count);
    void poll();

private:
    Stream &stream;
    const ConsoleCommand *commands;
    size_t count;
    String line;

    void execute(const String &name);
    void printHelp() const;
};
