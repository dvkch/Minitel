#pragma once

#include "KeyDef.h"
#include "Matrix.h"

// HID usages are positional and named after the US layout: the host must use the French AZERTY layout.
// Ctrl + I, H, J and X, printed in red as Tab, BS, LF and Can, need no mapping: terminals turn them into these control codes
constexpr KeyDef KEYMAP[MATRIX_ROWS][MATRIX_COLS] = {
    { // X0
        // name         normal                                      shifted                                 fn
        { "Shift",      MOD(LSHIFT, "Shift"),                       NONE,                                   NONE },
        { "W",          K(HID_KEY_Z),                               NONE,                                   NONE },
        { "B",          K(HID_KEY_B),                               NONE,                                   NONE },
        { "N",          K(HID_KEY_N),                               NONE,                                   NONE },
        { "Shift",      MOD(LALT, "Alt"),                           NONE,                                   NONE },
        { "V",          K(HID_KEY_V),                               NONE,                                   NONE },
        { "C",          K(HID_KEY_C),                               NONE,                                   NONE },
        { "X",          K(HID_KEY_X),                               NONE,                                   NONE },
        {},
    },
    { // X1
        { "Q",          K(HID_KEY_A),                               NONE,                                   NONE },
        { "D",          K(HID_KEY_D),                               NONE,                                   NONE },
        { "G",          K(HID_KEY_G),                               NONE,                                   NONE },
        { "J",          K(HID_KEY_J),                               NONE,                                   NONE },
        { "L",          K(HID_KEY_L),                               NONE,                                   NONE },
        { "1",          K(HID_KEY_KEYPAD_1),                        K(HID_KEY_7, RALT, "`"),                K(HID_KEY_F1, "F1") },
        { "2",          K(HID_KEY_KEYPAD_2),                        K(HID_KEY_5, "("),                      K(HID_KEY_F2, "F2") },
        { "3",          K(HID_KEY_KEYPAD_3),                        K(HID_KEY_MINUS, ")"),                  K(HID_KEY_F3, "F3") },
        {},
    },
    { // X2
        { "Ctrl",       MOD(LCTRL, "Ctrl"),                         NONE,                                   NONE },
        { "S",          K(HID_KEY_S),                               NONE,                                   NONE },
        { "F",          K(HID_KEY_F),                               NONE,                                   NONE },
        { "H",          K(HID_KEY_H),                               NONE,                                   NONE },
        { "K",          K(HID_KEY_K),                               NONE,                                   NONE },
        { "M",          K(HID_KEY_SEMICOLON),                       NONE,                                   NONE },
        { "P",          K(HID_KEY_P),                               NONE,                                   NONE },
        { "O",          K(HID_KEY_O),                               NONE,                                   NONE },
        {},
    },
    { // X3
        { "A",          K(HID_KEY_Q),                               NONE,                                   NONE },
        { "Z",          K(HID_KEY_W),                               NONE,                                   NONE },
        { "E",          K(HID_KEY_E),                               NONE,                                   K(HID_KEY_E, RALT, "€") },
        { "R",          K(HID_KEY_R),                               NONE,                                   NONE },
        { "T",          K(HID_KEY_T),                               NONE,                                   NONE },
        { "Y",          K(HID_KEY_Y),                               NONE,                                   NONE },
        { "U",          K(HID_KEY_U),                               NONE,                                   NONE },
        { "I",          K(HID_KEY_I),                               NONE,                                   NONE },
        {},
    },
    { // X4
        { "Esc",        K(HID_KEY_ESCAPE, "Esc"),                   NONE,                                   NONE },
        { ",",          K(HID_KEY_M, ","),                          K(HID_KEY_EUROPE_2, "<"),               NONE },
        { ".",          K(HID_KEY_COMMA, LSHIFT, "."),              K(HID_KEY_EUROPE_2, LSHIFT, ">"),       NONE },
        { "'",          K(HID_KEY_4, "'"),                          K(HID_KEY_0, RALT, "@"),                NONE },
        { ";",          K(HID_KEY_COMMA, ";"),                      K(HID_KEY_EQUAL, LSHIFT, "+"),          NONE },
        { "-",          K(HID_KEY_6, "-"),                          K(HID_KEY_EQUAL, "="),                  K(HID_KEY_2, RALT, "~") },
        { ":",          K(HID_KEY_PERIOD, ":"),                     K(HID_KEY_BACKSLASH, "*"),              NONE },
        { "?",          K(HID_KEY_M, LSHIFT, "?"),                  K(HID_KEY_PERIOD, LSHIFT, "/"),         NONE },
        {},
    },
    { // X5
        { "Connexion",  MOD(LGUI, "Menu"),                          NONE,                                   SYS(SYSTEM_POWER, "Power") }, // also wakes a halted Pi
        { "Guide",      K(HID_KEY_END, "End"),                      K(HID_KEY_BRACKET_LEFT, LSHIFT, "¨"),   NONE },
        { "Correction", K(HID_KEY_BACKSPACE, "Backspace"),          K(HID_KEY_SLASH, LSHIFT, "§"),          NONE },
        { "Suite",      CC(HID_USAGE_CONSUMER_AC_FORWARD, "Forward"), K(HID_KEY_BACKSLASH, RALT, "`"),      CC(HID_USAGE_CONSUMER_VOLUME_INCREMENT, "Vol+") }, // dead grave
        { "Envoi",      K(HID_KEY_PAGE_DOWN, "Page Down"),          K(HID_KEY_EQUAL, RALT, "}"),            NONE },
        { "4",          K(HID_KEY_KEYPAD_4),                        K(HID_KEY_BRACKET_RIGHT, "$"),          K(HID_KEY_F4, "F4") },
        { "5",          K(HID_KEY_KEYPAD_5),                        K(HID_KEY_APOSTROPHE, LSHIFT, "%"),     K(HID_KEY_F5, "F5") },
        { "6",          K(HID_KEY_KEYPAD_6),                        K(HID_KEY_1, "&"),                      K(HID_KEY_F6, "F6") },
        {},
    },
    { // X6
        { "Fnct",       FN("Fn"),                                   NONE,                                   NONE },
        { "Sommaire",   K(HID_KEY_HOME, "Home"),                    K(HID_KEY_BRACKET_LEFT, "^"),           NONE },
        { "Annulation", K(HID_KEY_DELETE, "Delete"),                K(HID_KEY_8, RALT, "\\"),               NONE },
        { "Retour",     CC(HID_USAGE_CONSUMER_AC_BACK, "Back"),     K(HID_KEY_M, RALT, "´"),                CC(HID_USAGE_CONSUMER_VOLUME_DECREMENT, "Vol-") }, // dead acute
        { "Répétition", K(HID_KEY_PAGE_UP, "Page Up"),              K(HID_KEY_4, RALT, "{"),                NONE },
        { "7",          K(HID_KEY_KEYPAD_7),                        K(HID_KEY_SLASH, "!"),                  K(HID_KEY_F7, "F7") },
        { "8",          K(HID_KEY_KEYPAD_8),                        K(HID_KEY_3, "\""),                     K(HID_KEY_F8, "F8") },
        { "9",          K(HID_KEY_KEYPAD_9),                        K(HID_KEY_3, RALT, "#"),                K(HID_KEY_F9, "F9") },
        {},
    },
    { // X7
        { "↑",          K(HID_KEY_ARROW_UP, "Up"),                  NONE,                                   NONE },
        { "↓",          K(HID_KEY_ARROW_DOWN, "Down"),              NONE,                                   NONE },
        { "←",          K(HID_KEY_ARROW_LEFT, "Left"),              NONE,                                   NONE },
        { "→",          K(HID_KEY_ARROW_RIGHT, "Right"),            NONE,                                   NONE },
        { "Return",     K(HID_KEY_ENTER, "Enter"),                  NONE,                                   NONE },
        { "*",          K(HID_KEY_NUM_LOCK, "NumLock"),             K(HID_KEY_5, RALT, "["),                NONE },
        { "0",          K(HID_KEY_KEYPAD_0),                        K(HID_KEY_6, RALT, "|"),                NONE },
        { "#",          MOD(RCTRL, "Compose"),                      K(HID_KEY_MINUS, RALT, "]"),            NONE }, // Compose key with the host's compose:rctrl option
        { "Espace",     K(HID_KEY_SPACE, "Space"),                  K(HID_KEY_TAB, "Tab"),                  NONE },
    },
    { // X8, no key found on this row so far
    },
};
