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

The empty variant keeps dead keys (see [Keys](#keys)).

The mapping is defined in `src/Keymap.h`. Each key has a normal action, and optionally a Shift and a Fnct one. An invalid key code in this file fails the build.

### Keys

Rules:

- Keys type what is printed on them.
- Shift types the upper symbol.
- Keypad digits need NumLock on.
- Fnct + a key without Fnct action acts as Alt.
- Compose types any other character.

Function keys, with the symbol typed with Shift:

```
Connexion    →  Super (menu)

Sommaire     Annulation    Retour     Répétition
Guide        Correction    Suite      Envoi

             ↓

Home ^       Delete \      Back ´     Page Up {
End ¨        Backspace §   Forward `  Page Down }
```

Other keys:

| Key           | Sends         | With Shift |
|---------------|---------------|------------|
| `*` (keypad)  | NumLock       | `[`        |
| `#` (keypad)  | Compose       | `]`        |
| Espace        | Space         | Tab        |

Fnct layer:

| Fnct +        | Sends         |
|---------------|---------------|
| 1 to 9        | F1 to F9      |
| E             | €             |
| Retour        | Volume down   |
| Suite         | Volume up     |

Dead keys: `^`, `¨`, `´` and `` ` `` (on Suite) wait for the next letter. `^` then `e` types `ê`. `´` then `e` types `é`. `` ` `` then `a` types `à`. Then Space types the accent alone. The `` ` `` on 1 is not a dead key.

Compose: press and release `#`, then type two characters:

| Type          | Gives |
|---------------|-------|
| `o` `e`       | œ     |
| `,` `c`       | ç     |
| `=` `e`       | €     |
| `L` `-`       | £     |
| `s` `s`       | ß     |

Labels printed in red, for Ctrl:

- Tab (I), BS (H), LF (J), Can (X): work in terminals, as standard control codes.
- £, Œ, œ, ç, ß, E.Page, Del: not supported, use Compose for the characters.

Not available: F10 to F12, Insert, Caps Lock, AltGr.

### Stickers

Labels to add to the keys. Blue: with Fnct. Yellow: with Shift. Keep the printed Shift symbols visible.

| Key           | Main label    | Small label       |
|---------------|---------------|-------------------|
| Connexion     | Menu          |                   |
| Sommaire      | Début ⇱       |                   |
| Guide         | Fin ⇲         |                   |
| Annulation    | Suppr         |                   |
| Répétition    | Pg ⇞          |                   |
| Envoi         | Pg ⇟          |                   |
| `*` (keypad)  | Verr Num      |                   |
| `#` (keypad)  | ⎄ (Compose)   |                   |
| Fnct          |               | Fn, blue          |
| Shift (both)  |               | mark, yellow      |
| Espace        |               | Tab, yellow       |
| Keypad 1 to 9 |               | F, blue           |
| E             |               | €, blue           |
| Retour        |               | Vol −, blue       |
| Suite         |               | Vol +, blue       |

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
- `log coords`: print each key press and release as its matrix position
- `log names`: print each key press and release with its possible usages, the one sent highlighted
- `log matrix`: print the state of the 81 matrix positions at each change, `#` for pressed keys, `?` for keys read as pressed but ignored as ghosts, `.` for released keys
- `log off`: stop printing key presses

`log coords` and `log names` print `-----` once all keys are released. Example of `log names`, for Shift + Sommaire, the highlighted usages being `Shift`, then `Shift: ^`:

```
DOWN  Shift:       [Shift]        Shift: -      Fnct: -
DOWN  Sommaire:     Home         [Shift: ^]     Fnct: -
UP    Sommaire:     Home         [Shift: ^]     Fnct: -
UP    Shift:       [Shift]        Shift: -      Fnct: -
-----
```

The logs use ANSI colors. Don't add the `default` filter to `monitor_filters`, it strips them and leaves stray characters.

A `-` usage means the normal one is sent instead, with Shift or Fnct still held.
