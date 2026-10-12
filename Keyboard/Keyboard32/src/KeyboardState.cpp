#include <string.h>
#include "KeyboardState.h"
#include "Keymap.h"

KeyVariant KeyboardState::onKey(uint8_t row, uint8_t col, bool pressed) {
    if (!pressed) {
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
    if (isFnHeld() && !def.fn.isEmpty()) {
        key.action = def.fn;
        key.variant = KeyVariant::Fn;
    }
    else if (isShiftHeld() && !def.shifted.isEmpty()) {
        key.action = def.shifted;
        key.variant = KeyVariant::Shifted;
    }
    held[heldCount++] = key;
    return key.variant;
}

bool KeyboardState::isFnHeld() const {
    for (uint8_t i = 0; i < heldCount; i++) {
        if (held[i].action.fn) {
            return true;
        }
    }
    return false;
}

bool KeyboardState::isShiftHeld() const {
    for (uint8_t i = 0; i < heldCount; i++) {
        if (held[i].action.isModifier() && (held[i].action.mods & LSHIFT)) {
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
        // Fnct sends nothing, and must not become the latest key
        if (action.fn) {
            continue;
        }
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
        if (action.system) {
            system = action.system;
        }
        latest = &held[i];
    }

    // the most recently pressed key decides the modifiers: a shifted variant is typed
    // without Shift, and a key can add its own modifiers, e.g. Shift for "."
    if (latest) {
        if (latest->variant == KeyVariant::Shifted) {
            mods &= ~LSHIFT;
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
    if (system != sentSystem && output.sendSystem(system)) {
        sentSystem = system;
    }
}
