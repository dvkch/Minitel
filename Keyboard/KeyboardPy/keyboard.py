#!/usr/bin/python3

import RPi.GPIO as GPIO
from time import sleep
from evdev import UInput, uinput, ecodes as e
import sys

rows = [14, 15, 18, 23, 24, 25,  8,  7,  1]
cols = [ 6,  5,  0, 11,  9, 10, 22, 27, 17]

# NB: KEY_A corresponds to KEY_A only on a US layout. so we switched the proper keys to have the proper keyboard here
# used this image : https://qph.cf2.quoracdn.net/main-qimg-5b419d2814ddde26d2d919a14354ca9a-lq
# To compare with a proper keyboard: `sudo apt install -y evtest && sudo evtest`
keyboard = [
    ##########
    # Row 0
    ##########
    { "id": "x0y0", "name": "Shift",        "code": e.KEY_LEFTSHIFT,  "is_shift": True },
    { "id": "x0y1", "name": "W",            "code": e.KEY_Z },
    { "id": "x0y2", "name": "B",            "code": e.KEY_B },
    { "id": "x0y3", "name": "N",            "code": e.KEY_N },
    { "id": "x0y4", "name": "Alt",          "code": e.KEY_LEFTALT }, # used as Alt
    { "id": "x0y5", "name": "V",            "code": e.KEY_V },
    { "id": "x0y6", "name": "C",            "code": e.KEY_C },
    { "id": "x0y7", "name": "X",            "code": e.KEY_X },
    { "id": "x0y8", "name": None,           "code": None },

    ##########
    # Row 1
    ##########
    { "id": "x1y0", "name": "Q",            "code": e.KEY_A },
    { "id": "x1y1", "name": "D",            "code": e.KEY_D },
    { "id": "x1y2", "name": "G",            "code": e.KEY_G },
    { "id": "x1y3", "name": "J",            "code": e.KEY_J },
    { "id": "x1y4", "name": "L",            "code": e.KEY_L },
    { "id": "x1y5", "name": "1",            "code": e.KEY_KP1,                      "shift_code": [e.KEY_RIGHTALT, e.KEY_7],          "fn_code": e.KEY_F1 },
    { "id": "x1y6", "name": "2",            "code": e.KEY_KP2,                      "shift_code": e.KEY_5,                            "fn_code": e.KEY_F2 },
    { "id": "x1y7", "name": "3",            "code": e.KEY_KP3,                      "shift_code": e.KEY_MINUS,                        "fn_code": e.KEY_F3 },
    { "id": "x1y8", "name": None,           "code": None },

    ##########
    # Row 2
    ##########
    { "id": "x2y0", "name": "Ctrl",         "code": e.KEY_LEFTCTRL },
    { "id": "x2y1", "name": "S",            "code": e.KEY_S },
    { "id": "x2y2", "name": "F",            "code": e.KEY_F },
    { "id": "x2y3", "name": "H",            "code": e.KEY_H },
    { "id": "x2y4", "name": "K",            "code": e.KEY_K },
    { "id": "x2y5", "name": "M",            "code": e.KEY_SEMICOLON },
    { "id": "x2y6", "name": "P",            "code": e.KEY_P },
    { "id": "x2y7", "name": "O",            "code": e.KEY_O },
    { "id": "x2y8", "name": None,           "code": None },

    ##########
    # Row 3
    ##########
    { "id": "x3y0", "name": "A",            "code": e.KEY_Q },
    { "id": "x3y1", "name": "Z",            "code": e.KEY_W },
    { "id": "x3y2", "name": "E",            "code": e.KEY_E,                                                                          "fn_code": [e.KEY_RIGHTALT, e.KEY_E] },
    { "id": "x3y3", "name": "R",            "code": e.KEY_R },
    { "id": "x3y4", "name": "T",            "code": e.KEY_T },
    { "id": "x3y5", "name": "Y",            "code": e.KEY_Y },
    { "id": "x3y6", "name": "U",            "code": e.KEY_U },
    { "id": "x3y7", "name": "I",            "code": e.KEY_I },
    { "id": "x3y8", "name": None,           "code": None },

    ##########
    # Row 4
    ##########
    { "id": "x4y0", "name": "Esc",          "code": e.KEY_ESC },
    { "id": "x4y1", "name": ",",            "code": e.KEY_M,                        "shift_code": e.KEY_102ND },
    { "id": "x4y2", "name": ".",            "code": [e.KEY_LEFTSHIFT, e.KEY_COMMA], "shift_code": [e.KEY_LEFTSHIFT, e.KEY_102ND] },
    { "id": "x4y3", "name": "'",            "code": e.KEY_4,                        "shift_code": [e.KEY_RIGHTALT, e.KEY_0]},
    { "id": "x4y4", "name": ";",            "code": e.KEY_COMMA,                    "shift_code": [e.KEY_LEFTSHIFT, e.KEY_EQUAL] },
    { "id": "x4y5", "name": "-",            "code": e.KEY_6,                        "shift_code": e.KEY_EQUAL,                        "fn_code": [e.KEY_RIGHTALT, e.KEY_2] },
    { "id": "x4y6", "name": ":",            "code": e.KEY_DOT,                      "shift_code": e.KEY_BACKSLASH },
    { "id": "x4y7", "name": "?",            "code": [e.KEY_LEFTSHIFT, e.KEY_M],     "shift_code": [e.KEY_LEFTSHIFT, e.KEY_DOT] },
    { "id": "x4y8", "name": None,           "code": None },

    ##########
    # Row 5
    ##########
    { "id": "x5y0", "name": "Connexion",    "code": e.KEY_LEFTMETA,                                                                   "fn_code": e.KEY_POWER }, # Power: shuts the Pi down, can't start it
    { "id": "x5y1", "name": "Guide",        "code": e.KEY_END,                      "shift_code": [e.KEY_LEFTSHIFT, e.KEY_LEFTBRACE] },
    { "id": "x5y2", "name": "Correction",   "code": e.KEY_BACKSPACE,                "shift_code": [e.KEY_LEFTSHIFT, e.KEY_SLASH] },
    { "id": "x5y3", "name": "Suite",        "code": e.KEY_FORWARD,                  "shift_code": [e.KEY_RIGHTALT, e.KEY_BACKSLASH],  "fn_code": e.KEY_VOLUMEUP }, # dead grave
    { "id": "x5y4", "name": "Envoi",        "code": e.KEY_PAGEDOWN,                 "shift_code": [e.KEY_RIGHTALT, e.KEY_EQUAL] },
    { "id": "x5y5", "name": "4",            "code": e.KEY_KP4,                      "shift_code": e.KEY_RIGHTBRACE,                   "fn_code": e.KEY_F4 },
    { "id": "x5y6", "name": "5",            "code": e.KEY_KP5,                      "shift_code": [e.KEY_LEFTSHIFT, e.KEY_APOSTROPHE],"fn_code": e.KEY_F5 },
    { "id": "x5y7", "name": "6",            "code": e.KEY_KP6,                      "shift_code": e.KEY_1,                            "fn_code": e.KEY_F6 },
    { "id": "x5y8", "name": None,           "code": None },

    ##########
    # Row 6
    ##########
    { "id": "x6y0", "name": "Fnct",         "code": None,             "is_fn": True },
    { "id": "x6y1", "name": "Sommaire",     "code": e.KEY_HOME,                     "shift_code": e.KEY_LEFTBRACE },
    { "id": "x6y2", "name": "Annulation",   "code": e.KEY_DELETE,                   "shift_code": [e.KEY_RIGHTALT, e.KEY_8] },
    { "id": "x6y3", "name": "Retour",       "code": e.KEY_BACK,                     "shift_code": [e.KEY_RIGHTALT, e.KEY_M],          "fn_code": e.KEY_VOLUMEDOWN }, # dead acute
    { "id": "x6y4", "name": "Répétition",   "code": e.KEY_PAGEUP,                   "shift_code": [e.KEY_RIGHTALT, e.KEY_4] },
    { "id": "x6y5", "name": "7",            "code": e.KEY_KP7,                      "shift_code": e.KEY_SLASH,                        "fn_code": e.KEY_F7 },
    { "id": "x6y6", "name": "8",            "code": e.KEY_KP8,                      "shift_code": e.KEY_3,                            "fn_code": e.KEY_F8 },
    { "id": "x6y7", "name": "9",            "code": e.KEY_KP9,                      "shift_code": [e.KEY_RIGHTALT, e.KEY_3],          "fn_code": e.KEY_F9 },
    { "id": "x6y8", "name": None,           "code": None },

    ##########
    # Row 7
    ##########
    { "id": "x7y0", "name": "↑",            "code": e.KEY_UP },
    { "id": "x7y1", "name": "↓",            "code": e.KEY_DOWN },
    { "id": "x7y2", "name": "←",            "code": e.KEY_LEFT },
    { "id": "x7y3", "name": "→",            "code": e.KEY_RIGHT },
    { "id": "x7y4", "name": "Return",       "code": e.KEY_ENTER },
    { "id": "x7y5", "name": "*",            "code": e.KEY_NUMLOCK,                  "shift_code": [e.KEY_RIGHTALT, e.KEY_5]  },
    { "id": "x7y6", "name": "0",            "code": e.KEY_KP0,                      "shift_code": [e.KEY_RIGHTALT, e.KEY_6] },
    { "id": "x7y7", "name": "#",            "code": e.KEY_RIGHTCTRL,                "shift_code": [e.KEY_RIGHTALT, e.KEY_MINUS] }, # Compose key with the host's compose:rctrl option
    { "id": "x7y8", "name": "Espace",       "code": e.KEY_SPACE,                    "shift_code": [e.KEY_TAB] },

    ##########
    # Row 8
    ##########
    { "id": "x8y0", "name": None,           "code": None },
    { "id": "x8y1", "name": None,           "code": None },
    { "id": "x8y2", "name": None,           "code": None },
    { "id": "x8y3", "name": None,           "code": None },
    { "id": "x8y4", "name": None,           "code": None },
    { "id": "x8y5", "name": None,           "code": None },
    { "id": "x8y6", "name": None,           "code": None },
    { "id": "x8y7", "name": None,           "code": None },
    { "id": "x8y8", "name": None,           "code": None },
]

def flatten(data):
    flat = []
    for item in data:
        if isinstance(item, list):
            flat.extend(flatten(item))
        elif item is None:
            continue
        else:
            flat.append(item)

    return flat

# Without diodes, 3 keys held at the corners of a rectangle make the 4th one look pressed. This happens
# when two rows share at least two columns: a new press in such a configuration is ignored.
def is_ambiguous(matrix, row, col):
    for i in range(len(matrix)):
        if i != row and matrix[i] & (1 << col) and matrix[i] & matrix[row] & ~(1 << col):
            return True
    return False

def press_key(ui, keycode, press, shifts, shifted_keys, fns, fn_keys):
    key_config = keyboard[keycode]

    # the variant is chosen on press and reused on release, even if Shift or Fnct changed meanwhile.
    # Fnct wins over Shift, and a key without Fnct variant is sent as if Fnct wasn't held
    if press and len(fns) > 0 and "fn_code" in key_config:
        fn_keys.add(keycode)
    elif press and len(shifts) > 0 and "shift_code" in key_config:
        shifted_keys.add(keycode)
    using_fn_codes = keycode in fn_keys
    using_shift_codes = keycode in shifted_keys
    if not press:
        fn_keys.discard(keycode)
        shifted_keys.discard(keycode)

    keys = flatten([key_config["fn_code" if using_fn_codes else "shift_code" if using_shift_codes else "code"]])
    if not press:
        keys.reverse()

    # shifted variants are typed without Shift, which is restored once the key is released
    if using_shift_codes and press:
        for shift in shifts:
            ui.write(e.EV_KEY, shift, 0)

    for key in keys:
        ui.write(e.EV_KEY, key, 1 if press else 0)

    if using_shift_codes and not press:
        for shift in shifts:
            ui.write(e.EV_KEY, shift, 1)

def create_keyboard():
    ui = None
    print("---- Initializing keyboard ----")

    while ui is None:
        try:
            ui = UInput(name = "Minitel Keyboard", vendor = 0x1209, product = 0x7501)
            print("Initialized:", ui)
        except uinput.UInputError as error:
            ui = None
            print("Error:", error)
            print("Retrying in 2s...")
            sleep(2)

    print("-------------------------------")
    return ui

def run(ui, mode_coords, mode_names, mode_keyboard):
    GPIO.setwarnings(True)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(rows, GPIO.IN, pull_up_down = GPIO.PUD_UP)
    GPIO.setup(cols, GPIO.IN, pull_up_down = GPIO.PUD_UP)
    pressed = set()
    shifts = set()
    shifted_keys = set()
    fns = set()
    fn_keys = set()

    while True:
        sleep(1/60)
        syn = False

        matrix = [0] * len(rows)
        for i in range(len(rows)):
            GPIO.setup(rows[i], GPIO.OUT, initial = GPIO.LOW)
            for j in range(len(cols)):
                if GPIO.input(cols[j]) == GPIO.LOW:
                    matrix[i] |= 1 << j

            # a released row floats low long enough to ghost on the next one, drive it high first
            GPIO.output(rows[i], GPIO.HIGH)
            GPIO.setup(rows[i], GPIO.IN, pull_up_down = GPIO.PUD_UP)

        for i in range(len(rows)):
            for j in range(len(cols)):
                keycode = i * len(cols) + j
                newval = bool(matrix[i] & (1 << j))

                if newval and not keycode in pressed and not is_ambiguous(matrix, i, j):
                    pressed.add(keycode)
                    is_shift = "is_shift" in keyboard[keycode] and keyboard[keycode]["is_shift"]
                    if is_shift:
                        shifts.add(keyboard[keycode]["code"])
                    is_fn = "is_fn" in keyboard[keycode] and keyboard[keycode]["is_fn"]
                    if is_fn:
                        fns.add(keycode)

                    if mode_coords:
                        print(keyboard[keycode]["id"], "down")
                    if mode_names:
                        print(keyboard[keycode]["name"], "down")
                    if mode_keyboard:
                        press_key(ui, keycode, press = True, shifts = shifts, shifted_keys = shifted_keys, fns = fns, fn_keys = fn_keys)
                        syn = True

                elif not newval and keycode in pressed:
                    pressed.discard(keycode)
                    is_shift = "is_shift" in keyboard[keycode] and keyboard[keycode]["is_shift"]
                    if is_shift:
                        shifts.discard(keyboard[keycode]["code"])
                    is_fn = "is_fn" in keyboard[keycode] and keyboard[keycode]["is_fn"]
                    if is_fn:
                        fns.discard(keycode)

                    if mode_coords:
                        print(keyboard[keycode]["id"], "up")
                    if mode_names:
                        print(keyboard[keycode]["name"], "up")
                    if mode_keyboard:
                        press_key(ui, keycode, press = False, shifts = shifts, shifted_keys = shifted_keys, fns = fns, fn_keys = fn_keys)
                        syn = True

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
        run(None, mode_coords = True, mode_names = False, mode_keyboard = False)
    elif sys.argv[1] == "names":
        print("Starting in key names mode")
        run(None, mode_coords = False, mode_names = True, mode_keyboard = False)
    elif sys.argv[1] == "keyboard":
        print("Starting in UInput mode")
        ui = create_keyboard()
        run(ui, mode_coords = False, mode_names = False, mode_keyboard = True)
    else:
        help()
        exit(1)

try:
    main()
except KeyboardInterrupt:
    GPIO.setwarnings(False)
    GPIO.cleanup()
