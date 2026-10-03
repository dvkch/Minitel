# Minitel/KeyboardC

Tested on a Raspberry Pi 4B, should work on any Pi with a 40-pin header (except the Pi 5 for the C version).

### GPIO

Connect the keyboard ribbon cable to these BCM GPIO IDs:

- For X0 to X8 (see ribbon cable, it's written on it, yes, really)
    - 14, 15, 18, 23, 24, 25,  8,  7, 1
- For Y0 to Y8 
    - 6, 5, 0, 11, 9, 10, 22, 27, 17

### Keyboard layout

The keyboard layout is French only, here are the best settings to use with this driver or its Python equivalent.

On Raspberry Pi OS desktop (Wayland, labwc), update `~/.config/labwc/environment`, then log out and back in:

```
XKB_DEFAULT_MODEL=pc105
XKB_DEFAULT_LAYOUT=fr
XKB_DEFAULT_VARIANT=
XKB_DEFAULT_OPTIONS=lv3:ralt_switch,compose:rctrl
```

For the console and X11, update `/etc/default/keyboard` (or use `sudo raspi-config` > Localization > Keyboard):

```
# KEYBOARD CONFIGURATION FILE

# Consult the keyboard(5) manual page.

XKBMODEL="pc105"
XKBLAYOUT="fr"
XKBVARIANT=""
XKBOPTIONS="lv3:ralt_switch,compose:rctrl"

BACKSPACE="guess"
```

The empty variant keeps dead keys (`^` then `e` types `ê`), and the Compose key (Right Ctrl, `#` on the Minitel keypad) types other characters: press and release it, then type two characters, e.g. `o` `e` for `œ`.

Nota: The keyboard matrix has no diodes: holding 3 keys at the corners of a rectangle (e.g. W, B, D) makes the 4th one (G) look pressed. Both drivers ignore new key presses in such a configuration.

### Usage

- Install using `sudo make install`
- Uninstall using `sudo make uninstall`
- Debug: 
  - stop the service (`minitel-keyboard-c` or `minitel-keyboard-py`) 
  - run manually as `sudo ./minitel-keyboard-c coords` to print the matrix position of each key
  - run manually as `sudo ./minitel-keyboard-c names` to print the key labels

To run a driver as a non-root user, allow using uinput (cf [Steam help](https://wiki.gentoo.org/wiki/Steam_Controller#Permissions)):

```
sudo groupadd minitel
sudo usermod -a -G minitel pi
echo 'KERNEL=="uinput", SUBSYSTEM=="misc", MODE="0660", GROUP="minitel", OPTIONS+="static_node=uinput"' | sudo tee /etc/udev/rules.d/99-minitel.rules
```

#### Useful links

- [Labomedia](https://wiki.labomedia.org/index.php/Renaissance_d%27un_Minitel_avec_une_Raspberry_Pi)
- [GPIO Matrix Keyboard](https://github.com/nutki/rpi-gpio-matrix-keyboard/blob/master/gpio_keyboard.py)
- [Pinout](https://pinout.xyz)
- [UInput events](https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h)
