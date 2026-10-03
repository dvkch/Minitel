#include <string.h>
#include "KeyboardState.h"
#include "Keymap.h"

// held modifiers ignored while a key is the latest pressed: Shift for a shifted variant, Alt (Fnct) for a Fnct one
static uint8_t removedMods(KeyVariant variant) {
    switch (variant) {
        case KeyVariant::Shifted:
            return SHIFTS;
        case KeyVariant::Fn:
            return LALT;
        case KeyVariant::None:
        case KeyVariant::Normal:
            break;
    }
    return 0;
}

KeyVariant KeyboardState::onKey(uint8_t row, uint8_t col, bool pressed) {
    if (!pressed) {
        if (row == FN_ROW && col == FN_COL) {
            fnUsedAsAlt = false;
        }
        for (uint8_t i = 0; i < heldCount; i++) {
            if (held[i].row == row && held[i].col == col) {
                KeyVariant variant = held[i].variant;
                memmove(&held[i], &held[i + 1], (heldCount - i - 1) * sizeof(HeldKey));
                heldCount--;
                return variant;
            }
        }
        return KeyVariant::None;
    }

    const KeyDef &def = KEYMAP[row][col];
    if (def.normal.isEmpty() || heldCount == MAX_HELD) {
        return KeyVariant::None;
    }

    // the variant is chosen on press and kept until release, even if Shift or Fnct change meanwhile
    HeldKey key = { row, col, def.normal, KeyVariant::Normal };
    bool isFnHeld = isHeld(FN_ROW, FN_COL);
    if (isFnHeld && !def.fn.isEmpty()) {
        key.action = def.fn;
        key.variant = KeyVariant::Fn;
    }
    else {
        // Fnct only sends Alt once combined with a key without Fnct action, then until released. Sending it
        // on press would make apps see a lone Alt press before F1, and open their menu bar
        if (isFnHeld) {
            fnUsedAsAlt = true;
        }
        if (isShiftHeld() && !def.shifted.isEmpty()) {
            key.action = def.shifted;
            key.variant = KeyVariant::Shifted;
        }
    }
    held[heldCount++] = key;
    return key.variant;
}

bool KeyboardState::isHeld(uint8_t row, uint8_t col) const {
    for (uint8_t i = 0; i < heldCount; i++) {
        if (held[i].row == row && held[i].col == col) {
            return true;
        }
    }
    return false;
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
    uint8_t system = 0;
    const HeldKey *latest = nullptr;

    for (uint8_t i = 0; i < heldCount; i++) {
        const KeyAction &action = held[i].action;
        if (action.isModifier()) {
            bool isFn = held[i].row == FN_ROW && held[i].col == FN_COL;
            if (!isFn || fnUsedAsAlt) {
                mods |= action.mods;
            }
            continue;
        }
        if (action.key && keyCount < sizeof(keys)) {
            keys[keyCount++] = action.key;
        }
        if (action.consumer) {
            consumer = action.consumer;
        }
        if (action.system) {
            system = action.system;
        }
        latest = &held[i];
    }

    // the most recently pressed key decides the modifiers: a shifted variant is typed
    // without Shift, and a key can add its own modifiers, e.g. Shift for "."
    if (latest) {
        mods &= ~removedMods(latest->variant);
        mods |= latest->action.mods;
    }

    if ((mods != sentMods || memcmp(keys, sentKeys, sizeof(keys)) != 0) && output.sendKeyboard(mods, keys)) {
        sentMods = mods;
        memcpy(sentKeys, keys, sizeof(keys));
    }
    if (consumer != sentConsumer && output.sendConsumer(consumer)) {
        sentConsumer = consumer;
    }
    if (system != sentSystem && output.sendSystem(system)) {
        sentSystem = system;
    }
}
