#include "KeyLog.h"
#include "Keymap.h"

// ANSI styles, supported by common terminals and passed through by `pio device monitor`
static constexpr const char *BOLD = "\033[1m";
static constexpr const char *DIM = "\033[2m";
static constexpr const char *REVERSE = "\033[7m";
static constexpr const char *RESET = "\033[0m";

// pads by characters rather than bytes, labels contain UTF-8 symbols like ¨ or ←
static void printPadded(Print &out, const String &text, size_t width, const char *style = nullptr) {
    size_t length = 0;
    for (char c : text) {
        if ((c & 0xC0) != 0x80) {
            length++;
        }
    }
    if (style) {
        out.print(style);
    }
    out.print(text);
    if (style) {
        out.print(RESET);
    }
    for (; length < width; length++) {
        out.print(' ');
    }
}

static void printDirection(Print &out, bool pressed) {
    printPadded(out, pressed ? "DOWN" : "UP", 6, pressed ? nullptr : DIM);
}

// one possible usage of the key, highlighted when it is the one sent
static void printUsage(Print &out, const char *prefix, const KeyAction &action, const char *defaultLabel, bool isSent, size_t width) {
    String text = " ";
    text += prefix;
    text += action.isEmpty() ? "-" : action.label ? action.label : defaultLabel;
    text += " ";
    printPadded(out, text, width, isSent ? REVERSE : nullptr);
}

void printKeyCoords(Print &out, uint8_t row, uint8_t col, bool pressed) {
    printDirection(out, pressed);
    out.printf("%sx%uy%u%s\n", BOLD, row, col, RESET);
}

void printKeyName(Print &out, uint8_t row, uint8_t col, bool pressed, KeyVariant variant) {
    printDirection(out, pressed);

    const KeyDef &def = KEYMAP[row][col];
    if (!def.name) {
        out.printf("%sx%uy%u:%s not mapped\n", BOLD, row, col, RESET);
        return;
    }

    printPadded(out, String(def.name) + ":", 13, BOLD);
    printUsage(out, "", def.normal, def.name, variant == KeyVariant::Normal, 14);
    printUsage(out, "Shift: ", def.shifted, def.name, variant == KeyVariant::Shifted, 14);
    printUsage(out, "Fnct: ", def.fn, def.name, variant == KeyVariant::Fn, 0);
    out.println();
}
