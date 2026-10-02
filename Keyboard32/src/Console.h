#pragma once

#include <Arduino.h>

struct ConsoleCommand {
    const char *name;
    const char *help;
    void (*run)(Print &out, const String &args);
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

    void execute(const String &line);
    void printHelp() const;
};
