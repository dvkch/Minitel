#pragma once

#include "HidOutput.h"
#include "KeyDef.h"

// Keys held on the Minitel keyboard, turned into HID reports following the keymap
class KeyboardState {
public:
    // returns the variant chosen on press, and on release the one that was used
    KeyVariant onKey(uint8_t row, uint8_t col, bool pressed);
    // sends the reports that changed since the last successful send, to be called on every loop
    void flush(HidOutput &output);

private:
    struct HeldKey {
        uint8_t row;
        uint8_t col;
        KeyAction action;
        KeyVariant variant;
    };
    static constexpr uint8_t MAX_HELD = 16;

    HeldKey held[MAX_HELD];
    uint8_t heldCount = 0;
    bool fnUsedAsAlt = false;
    uint8_t sentMods = 0;
    uint8_t sentKeys[6] = {};
    uint16_t sentConsumer = 0;
    uint8_t sentSystem = 0;

    bool isHeld(uint8_t row, uint8_t col) const;
    bool isShiftHeld() const;
};
