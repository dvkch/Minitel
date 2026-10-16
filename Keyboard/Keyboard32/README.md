# Minitel/Keyboard/Keyboard32

Firmware for an ESP32-S3 that turns the Minitel keyboard into a USB keyboard. Tested on an ESP32-S3-DevKitC-1 N16R8.

### Connection

Use the ESP32's native USB port (labeled `USB`, not `COM`): it is used for flashing, for the serial console and as the keyboard.

The computer sees a single USB device, `1209:7501 Minitel Keyboard`, made of two parts: a serial port (`/dev/ttyACM0` on Linux) for the console, and a keyboard with media keys.

The `COM` port is optional: it shows the ESP32's boot and crash messages, printed before USB is started.

### GPIO

Ribbon names are written on the cable. Yes, really.

| Ribbon | X0 | X1 | X2 | X3 | X4 | X5 | X6 | X7 | X8 |
|--------|----|----|----|----|----|----|----|----|----|
| GPIO   |  4 |  5 |  6 |  7 | 15 | 16 | 17 | 18 |  8 |

| Ribbon | Y0 | Y1 | Y2 | Y3 | Y4 | Y5 | Y6 | Y7 | Y8 |
|--------|----|----|----|----|----|----|----|----|----|
| GPIO   |  1 |  2 | 42 | 41 | 40 | 39 | 38 | 47 | 21 |

### Status LED

A WS2812 LED shows the keyboard state:

| Color          | Meaning |
|----------------|----------|
| Red            | Waiting for a computer: none has connected since the keyboard was powered on |
| Pulsing red    | The computer was there, but isn't listening anymore: shut down or rebooting |
| Pulsing yellow | No key pressed for a while (15 minutes by default, see the `idle` command) |
| Green          | Ready to type, NumLock on |
| Yellow         | Ready to type, NumLock off |
| Blue           | Fnct + Connexion held: the power key goes to the computer, or wakes it up if it is shut down |

Wiring: data to GPIO 14 through a ~330 Ω resistor, power to 5V and GND. These are the last three pins of the left header. The `IN-OUT` jumper must be closed (solder bridge), otherwise 5V gets almost no power from USB.

### Raspberry Pi power

While all drivers can send a request for shutdown to the OS when pressing `Fnct + Connexion`, only this driver can wake the Pi once it has halted.

Wiring: ESP32 GPIO 10 (left header, 16th pin) to the Pi's GPIO 3 (header pin 5). No resistor. Ground is shared through the USB cable.

Ensure the Pi EEPROM settings are correct or update them using `sudo -E rpi-eeprom-config --edit` then reboot :

- `WAKE_ON_GPIO=1`
- `POWER_OFF_ON_HALT=0`

### Usage

Everything PlatformIO needs is installed inside this folder (`.venv` and `.platformio`, about 6 GB during first build, about 1.5 GB once pruned), nothing is installed system wide.

- `make deps`: install PlatformIO (on Debian based systems, also installs `python3-venv` using `sudo apt-get`)
- `make`: build the firmware, installing the dependencies on first run
- `make flash`: build and flash the firmware
- `make monitor`: open the serial console, exit with `Ctrl+C`
- `make console`: open the serial console without PlatformIO, using [tio](https://github.com/tio/tio) (`sudo apt install tio`), exit with `Ctrl+T` then `Q`. Crashes aren't decoded
- `make erase`: wipe the whole ESP32 flash
- `make clean`: remove build files
- `make prune`: remove PlatformIO's download caches and the files not needed for an ESP32-S3 (done automatically after each build)
- `make clean-deps`: remove build files and all dependencies, to free disk space

The serial port is detected automatically. If several are connected, pick one with `PORT`, e.g. `make flash PORT=/dev/ttyACM0`. On Linux the user must be in the `dialout` or `plugdev` group to use it.

### Debug

Open the console using `make monitor` or `make console`, then type a command followed by `Enter`:

- `help`: list the available commands
- `version`: show the firmware version
- `log coords`: print each key press and release as its matrix position
- `log names`: print each key press and release with its possible usages, the one sent highlighted
- `log matrix`: print the state of the 81 matrix positions at each change:
  - `#` for pressed keys
  - `?` for keys read as pressed but ignored as ghosts
  - `.` for released keys
- `log off`: stop printing key presses
- `idle`: handle the delay without key presses before the LED pulses (saved across reboots, 15 minutes by default):
  - `idle` shows the current value
  - `idle 600` sets it in seconds
  - `idle 0` disables it
