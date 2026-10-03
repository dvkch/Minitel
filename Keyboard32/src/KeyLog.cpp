#include "KeyLog.h"
#include "Keymap.h"

// pads by characters rather than bytes, labels contain UTF-8 symbols like ¨ or ←
static void printPadded(Print &out, const String &text, size_t width) {
    size_t length = 0;
    for (char c : text) {
        if ((c & 0xC0) != 0x80) {
            length++;
        }
    }
    out.print(text);
    for (; length < width; length++) {
        out.print(' ');
    }
}

// one possible usage of the key, in brackets when it is the one sent
static String usage(const char *prefix, const KeyAction &action, const char *defaultLabel, bool isSent) {
    String text = prefix;
    text += action.isEmpty() ? "-" : action.label ? action.label : defaultLabel;
    return String(isSent ? "[" : " ") + text + (isSent ? "]" : " ");
}

void printKeyCoords(Print &out, uint8_t row, uint8_t col, bool pressed) {
    printPadded(out, pressed ? "DOWN" : "UP", 6);
    out.printf("x%uy%u\n", row, col);
}

void printKeyName(Print &out, uint8_t row, uint8_t col, bool pressed, KeyVariant variant) {
    printPadded(out, pressed ? "DOWN" : "UP", 6);

    const KeyDef &def = KEYMAP[row][col];
    if (!def.name) {
        out.printf("x%uy%u: not mapped\n", row, col);
        return;
    }

    printPadded(out, String(def.name) + ":", 13);
    printPadded(out, usage("", def.normal, def.name, variant == KeyVariant::Normal), 14);
    printPadded(out, usage("Shift: ", def.shifted, def.name, variant == KeyVariant::Shifted), 14);
    out.println(usage("Fnct: ", def.fn, def.name, variant == KeyVariant::Fn));
}
