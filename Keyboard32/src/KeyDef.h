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
    const char *label;  // shown in the logs, the key name when null

    constexpr bool isEmpty() const { return !mods && !key && !consumer; }
    constexpr bool isModifier() const { return mods && !key && !consumer; }
};

struct KeyDef {
    const char *name;   // label printed on the Minitel key
    KeyAction normal;
    KeyAction shifted;  // empty: the normal action is sent, with Shift still held
    KeyAction fn;       // empty: the normal action is sent, with Fnct acting as Alt
};

// which action of a KeyDef is used, None for keys without any
enum class KeyVariant : uint8_t { None, Normal, Shifted, Fn };

constexpr KeyAction NONE = {};

// consteval + throw turns an out of range value into a build error
consteval KeyAction K(int key, int mods, const char *label = nullptr) {
    if (key <= 0 || key > 0xDD || mods < 0 || mods > 0xFF) throw "invalid HID key";
    return { uint8_t(mods), uint8_t(key), 0, label };
}

consteval KeyAction K(int key, const char *label = nullptr) {
    return K(key, 0, label);
}

consteval KeyAction MOD(int mods, const char *label = nullptr) {
    if (mods <= 0 || mods > 0xFF) throw "invalid HID modifiers";
    return { uint8_t(mods), 0, 0, label };
}

consteval KeyAction CC(int usage, const char *label = nullptr) {
    if (usage <= 0 || usage > 0x0FFF) throw "invalid HID consumer usage";
    return { 0, 0, uint16_t(usage), label };
}
