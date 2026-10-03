#include <string.h>
#include "KeyboardState.h"
#include "Keymap.h"

void KeyboardState::onKey(uint8_t row, uint8_t col, bool pressed) {
    if (!pressed) {
        for (uint8_t i = 0; i < heldCount; i++) {
            if (held[i].row == row && held[i].col == col) {
                memmove(&held[i], &held[i + 1], (heldCount - i - 1) * sizeof(HeldKey));
                heldCount--;
                break;
            }
        }
        return;
    }

    const KeyDef &def = KEYMAP[row][col];
    if (def.normal.isEmpty() || heldCount == MAX_HELD) {
        return;
    }

    // the variant is chosen on press and kept until release, even if Shift changes meanwhile
    bool isShifted = isShiftHeld() && !def.shifted.isEmpty();
    held[heldCount++] = { row, col, isShifted ? def.shifted : def.normal, isShifted };
}

bool KeyboardState::isShiftHeld() const {
    for (uint8_t i = 0; i < heldCount; i++) {
        if (held[i].action.isModifier() && (held[i].action.mods & SHIFTS)) {
            return true;
        }
    }
    return false;
}

void KeyboardState::flush(HidOutput &output) {
    uint8_t mods = 0;
    uint8_t keys[6] = {};
    uint8_t keyCount = 0;
    uint16_t consumer = 0;
    const HeldKey *latest = nullptr;

    for (uint8_t i = 0; i < heldCount; i++) {
        const KeyAction &action = held[i].action;
        if (action.isModifier()) {
            mods |= action.mods;
            continue;
        }
        if (action.key && keyCount < sizeof(keys)) {
            keys[keyCount++] = action.key;
        }
        if (action.consumer) {
            consumer = action.consumer;
        }
        latest = &held[i];
    }

    // the most recently pressed key decides the modifiers: a shifted variant is typed
    // without Shift, and a key can add its own modifiers, e.g. Shift for "."
    if (latest) {
        if (latest->isShiftedVariant) {
            mods &= ~SHIFTS;
        }
        mods |= latest->action.mods;
    }

    if ((mods != sentMods || memcmp(keys, sentKeys, sizeof(keys)) != 0) && output.sendKeyboard(mods, keys)) {
        sentMods = mods;
        memcpy(sentKeys, keys, sizeof(keys));
    }
    if (consumer != sentConsumer && output.sendConsumer(consumer)) {
        sentConsumer = consumer;
    }
}
