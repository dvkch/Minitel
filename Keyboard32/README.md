# Minitel/Keyboard32

Firmware for an ESP32-S3 that will turn the Minitel keyboard into a USB keyboard. Tested on an ESP32-S3-DevKitC-1 N16R8.

### Connection

Use the ESP32's native USB port (labeled `USB`, not `COM`): it is used for flashing and for the serial console.

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
