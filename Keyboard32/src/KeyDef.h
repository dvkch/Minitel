#pragma once

#include <stdint.h>
#include "class/hid/hid.h"

constexpr uint8_t LCTRL  = KEYBOARD_MODIFIER_LEFTCTRL;
constexpr uint8_t LSHIFT = KEYBOARD_MODIFIER_LEFTSHIFT;
constexpr uint8_t LALT   = KEYBOARD_MODIFIER_LEFTALT;
constexpr uint8_t LGUI   = KEYBOARD_MODIFIER_LEFTGUI;
constexpr uint8_t RCTRL  = KEYBOARD_MODIFIER_RIGHTCTRL;
constexpr uint8_t RSHIFT = KEYBOARD_MODIFIER_RIGHTSHIFT;
constexpr uint8_t RALT   = KEYBOARD_MODIFIER_RIGHTALT;
constexpr uint8_t RGUI   = KEYBOARD_MODIFIER_RIGHTGUI;
constexpr uint8_t SHIFTS = LSHIFT | RSHIFT;

struct KeyAction {
    uint8_t mods;
    uint8_t key;        // HID keyboard page usage
    uint16_t consumer;  // HID consumer page usage

    constexpr bool isEmpty() const { return !mods && !key && !consumer; }
    constexpr bool isModifier() const { return mods && !key && !consumer; }
};

struct KeyDef {
    const char *name;
    KeyAction normal;
    KeyAction shifted;  // empty: the normal action is sent, with Shift still held
    KeyAction fn;       // empty: the normal action is sent, with Fnct acting as Alt
};

constexpr KeyAction NONE = {};

// consteval + throw turns an out of range value into a build error
consteval KeyAction K(int key, int mods = 0) {
    if (key <= 0 || key > 0xDD || mods < 0 || mods > 0xFF) throw "invalid HID key";
    return { uint8_t(mods), uint8_t(key), 0 };
}

consteval KeyAction MOD(int mods) {
    if (mods <= 0 || mods > 0xFF) throw "invalid HID modifiers";
    return { uint8_t(mods), 0, 0 };
}

consteval KeyAction CC(int usage) {
    if (usage <= 0 || usage > 0x0FFF) throw "invalid HID consumer usage";
    return { 0, 0, uint16_t(usage) };
}
