#pragma once

#include "KeyDef.h"
#include "Matrix.h"

// position of the Fnct key, selecting the `fn` actions while held
constexpr uint8_t FN_ROW = 6;
constexpr uint8_t FN_COL = 0;

// HID usages are positional and named after the US layout: the host must use the French AZERTY layout.
// Ctrl + I, H, J and X, printed in red as Tab, BS, LF and Can, need no mapping: terminals turn them into these control codes
constexpr KeyDef KEYMAP[MATRIX_ROWS][MATRIX_COLS] = {
    { // X0
        // name             normal                              shifted                             fn
        { "Left Shift",     MOD(LSHIFT),                        NONE,                               NONE },
        { "W",              K(HID_KEY_Z),                       NONE,                               NONE },
        { "B",              K(HID_KEY_B),                       NONE,                               NONE },
        { "N",              K(HID_KEY_N),                       NONE,                               NONE },
        { "Right Shift",    MOD(RSHIFT),                        NONE,                               NONE },
        { "V",              K(HID_KEY_V),                       NONE,                               NONE },
        { "C",              K(HID_KEY_C),                       NONE,                               NONE },
        { "X",              K(HID_KEY_X),                       NONE,                               NONE },
        {},
    },
    { // X1
        { "Q",              K(HID_KEY_A),                       NONE,                               NONE },
        { "D",              K(HID_KEY_D),                       NONE,                               NONE },
        { "G",              K(HID_KEY_G),                       NONE,                               NONE },
        { "J",              K(HID_KEY_J),                       NONE,                               NONE },
        { "L",              K(HID_KEY_L),                       NONE,                               NONE },
        { "1/`",            K(HID_KEY_KEYPAD_1),                K(HID_KEY_7, RALT),                 K(HID_KEY_F1) },
        { "2/(",            K(HID_KEY_KEYPAD_2),                K(HID_KEY_5),                       K(HID_KEY_F2) },
        { "3/)",            K(HID_KEY_KEYPAD_3),                K(HID_KEY_MINUS),                   K(HID_KEY_F3) },
        {},
    },
    { // X2
        { "Control",        MOD(LCTRL),                         NONE,                               NONE },
        { "S",              K(HID_KEY_S),                       NONE,                               NONE },
        { "F",              K(HID_KEY_F),                       NONE,                               NONE },
        { "H",              K(HID_KEY_H),                       NONE,                               NONE },
        { "K",              K(HID_KEY_K),                       NONE,                               NONE },
        { "M",              K(HID_KEY_SEMICOLON),               NONE,                               NONE },
        { "P",              K(HID_KEY_P),                       NONE,                               NONE },
        { "O",              K(HID_KEY_O),                       NONE,                               NONE },
        {},
    },
    { // X3
        { "A",              K(HID_KEY_Q),                       NONE,                               NONE },
        { "Z",              K(HID_KEY_W),                       NONE,                               NONE },
        { "E",              K(HID_KEY_E),                       NONE,                               K(HID_KEY_E, RALT) }, // €
        { "R",              K(HID_KEY_R),                       NONE,                               NONE },
        { "T",              K(HID_KEY_T),                       NONE,                               NONE },
        { "Y",              K(HID_KEY_Y),                       NONE,                               NONE },
        { "U",              K(HID_KEY_U),                       NONE,                               NONE },
        { "I",              K(HID_KEY_I),                       NONE,                               NONE },
        {},
    },
    { // X4
        { "Escape",         K(HID_KEY_ESCAPE),                  NONE,                               NONE },
        { ",/<",            K(HID_KEY_M),                       K(HID_KEY_EUROPE_2),                NONE },
        { "./>",            K(HID_KEY_COMMA, LSHIFT),           K(HID_KEY_EUROPE_2, LSHIFT),        NONE },
        { "'/@",            K(HID_KEY_4),                       K(HID_KEY_0, RALT),                 NONE },
        { ";/+",            K(HID_KEY_COMMA),                   K(HID_KEY_EQUAL, LSHIFT),           NONE },
        { "-/=",            K(HID_KEY_6),                       K(HID_KEY_EQUAL),                   NONE },
        { ":/*",            K(HID_KEY_PERIOD),                  K(HID_KEY_BACKSLASH),               NONE },
        { "?//",            K(HID_KEY_M, LSHIFT),               K(HID_KEY_PERIOD, LSHIFT),          NONE },
        {},
    },
    { // X5
        { "Connexion",      MOD(LGUI),                          NONE,                               NONE },
        { "Guide/¨",        K(HID_KEY_END),                     K(HID_KEY_BRACKET_LEFT, LSHIFT),    NONE },
        { "Correction/§",   K(HID_KEY_BACKSPACE),               K(HID_KEY_SLASH, LSHIFT),           NONE },
        { "Suite/`",        CC(HID_USAGE_CONSUMER_AC_FORWARD),  K(HID_KEY_BACKSLASH, RALT),         CC(HID_USAGE_CONSUMER_VOLUME_INCREMENT) }, // dead grave
        { "Envoi/}",        K(HID_KEY_PAGE_DOWN),               K(HID_KEY_EQUAL, RALT),             NONE },
        { "4/$",            K(HID_KEY_KEYPAD_4),                K(HID_KEY_BRACKET_RIGHT),           K(HID_KEY_F4) },
        { "5/%",            K(HID_KEY_KEYPAD_5),                K(HID_KEY_APOSTROPHE, LSHIFT),      K(HID_KEY_F5) },
        { "6/&",            K(HID_KEY_KEYPAD_6),                K(HID_KEY_1),                       K(HID_KEY_F6) },
        {},
    },
    { // X6
        { "Fonction",       MOD(LALT),                          NONE,                               NONE },
        { "Sommaire/^",     K(HID_KEY_HOME),                    K(HID_KEY_BRACKET_LEFT),            NONE },
        { "Annulation/\\",  K(HID_KEY_DELETE),                  K(HID_KEY_8, RALT),                 NONE },
        { "Retour/´",       CC(HID_USAGE_CONSUMER_AC_BACK),     K(HID_KEY_M, RALT),                 CC(HID_USAGE_CONSUMER_VOLUME_DECREMENT) }, // dead acute
        { "Répétition/{",   K(HID_KEY_PAGE_UP),                 K(HID_KEY_4, RALT),                 NONE },
        { "7/!",            K(HID_KEY_KEYPAD_7),                K(HID_KEY_SLASH),                   K(HID_KEY_F7) },
        { "8/\"",           K(HID_KEY_KEYPAD_8),                K(HID_KEY_3),                       K(HID_KEY_F8) },
        { "9/#",            K(HID_KEY_KEYPAD_9),                K(HID_KEY_3, RALT),                 K(HID_KEY_F9) },
        {},
    },
    { // X7
        { "Up",             K(HID_KEY_ARROW_UP),                NONE,                               NONE },
        { "Down",           K(HID_KEY_ARROW_DOWN),              NONE,                               NONE },
        { "Left",           K(HID_KEY_ARROW_LEFT),              NONE,                               NONE },
        { "Right",          K(HID_KEY_ARROW_RIGHT),             NONE,                               NONE },
        { "Return",         K(HID_KEY_ENTER),                   NONE,                               NONE },
        { "NumLock/[",      K(HID_KEY_NUM_LOCK),                K(HID_KEY_5, RALT),                 NONE },
        { "0/|",            K(HID_KEY_KEYPAD_0),                K(HID_KEY_6, RALT),                 NONE },
        { "Compose/]",      MOD(RCTRL),                         K(HID_KEY_MINUS, RALT),             NONE }, // Compose key with the host's compose:rctrl option
        { "Space/Tab",      K(HID_KEY_SPACE),                   K(HID_KEY_TAB),                     NONE },
    },
    { // X8, no key found on this row so far
    },
};
