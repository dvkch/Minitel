# Minitel/Keyboard/KeyboardPy

User-land driver in Python, reading the keyboard wired to the Pi's GPIO pins. 

Tested on a Raspberry Pi 4B, should work on any Pi with a 40-pin header, the Pi 5 included (untested).

### GPIO

Connect the keyboard ribbon cable to these BCM GPIO IDs. Ribbon names are written on the cable. Yes, really.

| Ribbon | X0 | X1 | X2 | X3 | X4 | X5 | X6 | X7 | X8 |
|--------|----|----|----|----|----|----|----|----|----|
| GPIO   | 14 | 15 | 18 | 23 | 24 | 25 |  8 |  7 |  1 |

| Ribbon | Y0 | Y1 | Y2 | Y3 | Y4 | Y5 | Y6 | Y7 | Y8 |
|--------|----|----|----|----|----|----|----|----|----|
| GPIO   |  6 |  5 |  0 | 11 |  9 | 10 | 22 | 27 | 17 |

### Usage

- `sudo make install`
  - installs the driver and its `minitel-keyboard-py` service
  - installs `python3-rpi-lgpio` and `python3-evdev` using apt if missing
  - replaces the C++ driver if installed
- `sudo make uninstall` to uninstall
- `sudo make remove-deps` to remove dependencies
- `sudo systemctl start/stop/status minitel-keyboard-py` to use the service

To run the driver as a non-root user, allow using uinput (cf [Steam help](https://wiki.gentoo.org/wiki/Steam_Controller#Permissions)):

```
sudo groupadd minitel
sudo usermod -a -G minitel $USER
echo 'KERNEL=="uinput", SUBSYSTEM=="misc", MODE="0660", GROUP="minitel", OPTIONS+="static_node=uinput"' | sudo tee /etc/udev/rules.d/99-minitel.rules
```

Then log out and back in.

### Debugging

- stop the service `sudo systemctl stop minitel-keyboard-py`
- run manually as `sudo ./keyboard.py coords` to print the matrix position of each key
- run manually as `sudo ./keyboard.py names` to print the key labels
- run manually as `sudo ./keyboard.py keyboard` to run the driver normally after some changes

### Limitations

- `Fnct + Connexion` can only shut the Pi down, it can't start it once halted (see [Keyboard32](../Keyboard32))
- The keyboard is scanned 60 times per second: press Shift, Fnct, Ctrl and Alt slightly before the other key, pressed during the same scan they may be ignored
- No status LED
