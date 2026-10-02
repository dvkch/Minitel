#include "Console.h"

Console::Console(Stream &stream, const ConsoleCommand *commands, size_t count) : stream(stream), commands(commands), count(count) {}

void Console::poll() {
    while (stream.available()) {
        char c = stream.read();
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
    stream.println("Commands:");
    stream.printf("  %-10s %s\n", "help", "show this help");
    for (size_t i = 0; i < count; i++) {
        stream.printf("  %-10s %s\n", commands[i].name, commands[i].help);
    }
}

void Console::execute(const String &name) {
    if (name.isEmpty()) {
        return;
    }
    if (name == "help") {
        printHelp();
        return;
    }
    for (size_t i = 0; i < count; i++) {
        if (name == commands[i].name) {
            commands[i].run(stream);
            return;
        }
    }
    stream.printf("Unknown command '%s', type 'help'\n", name.c_str());
}
