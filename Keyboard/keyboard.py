#!/usr/bin/python3

import RPi.GPIO as GPIO
from time import sleep
from evdev import UInput, ecodes as e
import sys

ui = UInput(name = "Commodore Pi/4 Keyboard", vendor = 0x1209, product = 0x7501)
rows = [14, 15, 18, 23, 24, 25,  8,  7, 11]
cols = [ 9, 10, 22, 27, 17, 28, 29, 30, 31]

keycoords = [
    "x0y0", "x0y1", "x0y2", "x0y3", "x0y4", "x0y5", "x0y6", "x0y7", "x0y8",
    "x1y0", "x1y1", "x1y2", "x1y3", "x1y4", "x1y5", "x1y6", "x1y7", "x1y8",
    "x2y0", "x2y1", "x2y2", "x2y3", "x2y4", "x2y5", "x2y6", "x2y7", "x2y8",
    "x3y0", "x3y1", "x3y2", "x3y3", "x3y4", "x3y5", "x3y6", "x3y7", "x3y8",
    "x4y0", "x4y1", "x4y2", "x4y3", "x4y4", "x4y5", "x4y6", "x4y7", "x4y8",
    "x5y0", "x5y1", "x5y2", "x5y3", "x5y4", "x5y5", "x5y6", "x5y7", "x5y8",
    "x6y0", "x6y1", "x6y2", "x6y3", "x6y4", "x6y5", "x6y6", "x6y7", "x6y8",
    "x7y0", "x7y1", "x7y2", "x7y3", "x7y4", "x7y5", "x7y6", "x7y7", "x7y8",
    "x8y0", "x8y1", "x8y2", "x8y3", "x8y4", "x8y5", "x8y6", "x8y7", "x8y8",
]

keynames = [
    "Left Shift",   "W",        "B",            "N",        "Right Shift",  "V",        "C",        "X",        "<x0y8>",
    "Q",            "D",        "G",            "J",        "L",            "7",        "8",        "9",        "<x1y8>",
    "Control",      "S",        "F",            "H",        "K",            "M",        "P",        "O",        "<x2y8>",
    "A",            "Z",        "E",            "R",        "T",            "Y",        "U",        "I",        "<x3y8>",
    "Escape",       ",",        ".",            "\"",       ";",            "-",        ":",        "?",        "<x4y8>",
    "Connexion",    "Guide",    "Correction",   "Suite",    "Envoi",        "4",        "5",        "6",        "<x5y8>",
    "Fonction",     "Sommaire", "Annulation",   "Retour",   "Répétition",   "1",        "2",        "3",        "<x6y8>",
    "Up",           "Down",     "Left",         "Right",    "Return",       "*",        "0",        "#",        "Space",
    "<x8y0>",       "<x8y1>",   "<x8y2>",       "<x8y3>",   "<x8y4>",       "<x8y5>",   "<x8y6>",   "<x8y7>",   "<x8y8>",
]

keymap = [
    e.KEY_LEFTSHIFT, e.KEY_W,       e.KEY_B,        e.KEY_N,            e.KEY_RIGHTSHIFT, e.KEY_V,          e.KEY_C,        e.KEY_X,       e.KEY_UNKNOWN,
    e.KEY_Q,         e.KEY_D,       e.KEY_G,        e.KEY_J,            e.KEY_L,          e.KEY_KP7,        e.KEY_KP8,      e.KEY_KP9,     e.KEY_UNKNOWN,
    e.KEY_LEFTCTRL,  e.KEY_S,       e.KEY_F,        e.KEY_H,            e.KEY_K,          e.KEY_M,          e.KEY_P,        e.KEY_O,       e.KEY_UNKNOWN,
    e.KEY_A,         e.KEY_Z,       e.KEY_E,        e.KEY_R,            e.KEY_T,          e.KEY_Y,          e.KEY_U,        e.KEY_I,       e.KEY_UNKNOWN,
    e.KEY_ESC,       e.KEY_COMMA,   e.KEY_DOT,      e.KEY_APOSTROPHE,   e.KEY_SEMICOLON,  e.KEY_MINUS,      e.KEY_SLASH,    e.KEY_F1,      e.KEY_UNKNOWN, # slash should be ":", F1 should be "?"
    e.KEY_F1,        e.KEY_F7,      e.KEY_F8,       e.KEY_F9,           e.KEY_F10,        e.KEY_KP4,        e.KEY_KP5,      e.KEY_KP6,     e.KEY_UNKNOWN,
    e.KEY_F2,        e.KEY_F3,      e.KEY_F4,       e.KEY_F5,           e.KEY_F6,         e.KEY_KP1,        e.KEY_KP2,      e.KEY_KP3,     e.KEY_UNKNOWN,
    e.KEY_UP,        e.KEY_DOWN,    e.KEY_LEFT,     e.KEY_RIGHT,        e.KEY_ENTER,      e.KEY_KPASTERISK, e.KEY_KP0,      e.KEY_TAB,     e.KEY_SPACE,   # tab should be "#"
    e.KEY_UNKNOWN,   e.KEY_UNKNOWN, e.KEY_UNKNOWN,  e.KEY_UNKNOWN,      e.KEY_UNKNOWN,    e.KEY_UNKNOWN,    e.KEY_UNKNOWN,  e.KEY_UNKNOWN, e.KEY_UNKNOWN
]

# Connexion/Fin => F1
# Fnct          => F2
# Sommaire      => F3
# Annulation    => F4
# Retour        => F5
# Répétition    => F6
# Guide         => F7
# Correction    => F8
# Suite         => F9
# Envoi         => F10

def run(mode_coords, mode_names, mode_keyboard):
    GPIO.setwarnings(True)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(rows, GPIO.IN)
    GPIO.setup(cols, GPIO.IN, pull_up_down = GPIO.PUD_UP)
    pressed = set()

    while True:
        sleep(1/60)
        syn = False

        for i in range(len(rows)):
            GPIO.setup(rows[i], GPIO.OUT, initial = GPIO.LOW)

            for j in range(len(cols)):
                keycode = i * len(cols) + j
                newval = GPIO.input(cols[j]) == GPIO.LOW

                if  newval and not keycode in pressed:
                    pressed.add(keycode)
                    if mode_coords:
                        print(keycoords[keycode], "up")
                    if mode_names:
                        print(keynames[keycode], "up")
                    if mode_keyboard:
                        ui.write(e.EV_KEY, keymap[keycode], 1)
                    syn = True

                elif not newval and keycode in pressed:
                    pressed.discard(keycode)
                    if mode_coords:
                        print(keycoords[keycode], "down")
                    if mode_names:
                        print(keynames[keycode], "down")
                    if mode_keyboard:
                        ui.write(e.EV_KEY, keymap[keycode], 0)
                    syn = True

            GPIO.setup(rows[i], GPIO.IN)

        if syn:
            ui.syn()


def help():
        print("Unknown args: call this script using:")
        print("- keyboard.py coords    # print the pushed key coords")
        print("- keyboard.py names     # print the pushed key names")
        print("- keyboard.py keyboard  # run as a virtual input")

def main():
    if len(sys.argv) != 2:
        help()
        exit(1)

    if sys.argv[1] == "coords":
        print("Starting in key coords mode")
        run(mode_coords = True, mode_names = False, mode_keyboard = False)
    elif sys.argv[1] == "names":
        print("Starting in key names mode")
        run(mode_coords = False, mode_names = True, mode_keyboard = False)
    elif sys.argv[1] == "keyboard":
        print("Starting in UInput mode")
        run(mode_coords = False, mode_names = False, mode_keyboard = True)
    else:
        help()
        exit(1)

try:
    main()
except KeyboardInterrupt:
    GPIO.setwarnings(False)
    GPIO.cleanup()


