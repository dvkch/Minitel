# Minitel/Keyboard32

Firmware for an ESP32-S3 that turns the Minitel keyboard into a USB keyboard. Tested on an ESP32-S3-DevKitC-1 N16R8.

### Connection

Use the ESP32's native USB port (labeled `USB`, not `COM`): it is used for flashing, for the serial console and as the keyboard.

The computer sees a single USB device, `1209:7501 Minitel Keyboard`, made of two parts: a serial port (`/dev/ttyACM0` on Linux) for the console, and a keyboard with media keys (used by `Suite` and `Retour`).

The `COM` port is optional: it shows the ESP32's boot and crash messages, printed before USB is started.

### GPIO

Connect the keyboard ribbon cable to these ESP32 GPIOs (labels as printed on the ribbon cable):

| Ribbon | X0 | X1 | X2 | X3 | X4 | X5 | X6 | X7 | X8 |
|--------|----|----|----|----|----|----|----|----|----|
| GPIO   |  4 |  5 |  6 |  7 | 15 | 16 | 17 | 18 |  8 |

| Ribbon | Y0 | Y1 | Y2 | Y3 | Y4 | Y5 | Y6 | Y7 | Y8 |
|--------|----|----|----|----|----|----|----|----|----|
| GPIO   |  1 |  2 | 42 | 41 | 40 | 39 | 38 | 47 | 21 |

Rows (X) are on the left header, contiguous from the 4th pin. Columns (Y) are on the right header, from the 4th to the 10th pin, then the 17th and 18th. The pins in between can't be used: GPIO 35 to 37 are connected to the PSRAM inside the module, GPIO 0 and 45 select the boot mode, GPIO 48 drives the onboard RGB LED. No key has been found on X8 so far, it is wired anyway.

Nota: the keyboard matrix has no diodes: holding 3 keys at the corners of a rectangle (e.g. W, B, D) makes the 4th one (G) look pressed. New key presses in such a configuration are ignored.

### Keyboard layout

The keyboard sends key positions, not characters: the computer must be set to the French AZERTY layout, with Right Ctrl as the Compose key (see [Keys](#keys)).

On Raspberry Pi OS desktop (Wayland, labwc), in `~/.config/labwc/environment`, then log out and back in:

```
XKB_DEFAULT_MODEL=pc105
XKB_DEFAULT_LAYOUT=fr
XKB_DEFAULT_VARIANT=
XKB_DEFAULT_OPTIONS=lv3:ralt_switch,compose:rctrl
```

For the console and X11, in `/etc/default/keyboard` (or using `sudo raspi-config` > Localization > Keyboard), then reboot:

```
XKBMODEL="pc105"
XKBLAYOUT="fr"
XKBVARIANT=""
XKBOPTIONS="lv3:ralt_switch,compose:rctrl"
```

The empty variant keeps dead keys: `^` and `¨` (Shift + Sommaire and Shift + Guide) wait for the next letter, e.g. `^` then `e` types `ê`, `^` then Space types `^` alone.

The mapping is the same as the [KeyboardC](../KeyboardC) driver, and can be changed in `src/Keymap.h`: each key has a normal action, and optionally another one used when Shift is held, to type the symbol printed on the Minitel key. An invalid key code in this file fails the build.

### Keys

Letters, digits and symbols type what is printed on the keys, Shift selecting the upper symbol. Digits are sent as numeric keypad keys: NumLock must be on to type them.

Keys without a PC equivalent printed on them:

| Minitel key   | Sends                                      | With Shift |
|---------------|--------------------------------------------|------------|
| Connexion     | F1                                         |            |
| Sommaire      | F2                                         | `^`        |
| Guide         | F3                                         | `¨`        |
| Annulation    | F4                                         | `\`        |
| Répétition    | F5                                         | `{`        |
| Correction    | Backspace                                  | `§`        |
| Envoi         | Super (Windows key)                        | `}`        |
| Suite         | Forward media key, e.g. next page in a browser     |    |
| Retour        | Back media key, e.g. previous page in a browser    |    |
| Fnct          | Alt                                        |            |
| `*` (keypad)  | NumLock                                    | `[`        |
| `#` (keypad)  | Right Ctrl, used as the Compose key        | `]`        |
| Espace        | Space                                      | Tab        |

Printed in red, to be used with Ctrl:

- Tab (I), BS (H), LF (J) and Can (X): nothing is mapped, terminals turn Ctrl + I, H, J and X into these control codes
- £, Œ, œ, ç, ß, E.Page and Del: not supported, Ctrl + the key is sent instead, use Compose for the characters

Not on the Minitel:

- other characters: press and release Compose (`#`), then type two characters, e.g. `,` `c` for `ç`, `o` `e` for `œ`, `=` `e` for `€`, `L` `-` for `£`, `s` `s` for `ß`
- F6 to F12, Home, End, Page Up, Page Down, Insert, Delete, AltGr and Caps Lock: not available

### Usage

Everything PlatformIO needs is installed inside this folder (`.venv` and `.platformio`, about 6 GB during first build, about 1.5 GB once pruned), nothing is installed system wide.

- `make deps`: install PlatformIO (on Debian based systems, also installs `python3-venv` using `sudo apt-get`)
- `make`: build the firmware, installing the dependencies on first run
- `make flash`: build and flash the firmware
- `make monitor`: open the serial console, exit with `Ctrl+C`
- `make erase`: wipe the whole ESP32 flash (if it cannot connect, use the buttons as for the first flash)
- `make clean`: remove build files
- `make prune`: remove PlatformIO's download caches and the files not needed for an ESP32-S3 (done automatically after each build)
- `make clean-deps`: remove build files and all dependencies, to free disk space

The serial port is detected automatically. If several are connected, pick one with `PORT`, e.g. `make flash PORT=/dev/ttyACM0`. On Linux the user must be in the `dialout` group to use it.

### First flash

A blank ESP32, or one running another firmware, must be put in bootloader mode before the first `make flash`:

- hold the `BOOT` button
- plug the USB cable in, or press and release `RST`
- release `BOOT`

Then reset the board using `RST` once flashed. Next flashes don't need any button: the firmware reboots itself into the bootloader when `make flash` asks for it. If that ever fails, use the buttons again.

### Debug

Open the console using `make monitor`, then type a command followed by `Enter`:

- `help`: list the available commands
- `version`: show the firmware version
- `log coords`: print each key press and release as its matrix position, e.g. `x3y1 down` for Z
- `log names`: print each key press and release as its name in the keymap, e.g. `Z down`
- `log matrix`: print the state of the 81 matrix positions at each change, `#` for pressed keys, `?` for keys read as pressed but ignored as ghosts, `.` for released keys
- `log off`: stop printing key presses
