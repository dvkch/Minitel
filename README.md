# Minitel

## Multiboot

- <https://raspberrytips.fr/dual-boot-raspberry-pi/>
- <https://sourceforge.net/projects/pinn/>
- <https://github.com/procount/pinn>


## Generic

#### Useful links

- <https://hackaday.com/tag/minitel/>

## Over RS232

cf [RS232](./RS232)

#### Useful links

- <https://pila.fr/wordpress/?p=361>
    - [PDF](RS232/docs/Un%20minitel%20comme%20terminal%20linux%20USB.pdf)

## Reusing the CRT display

cf [CRT](./CRT)

#### Useful links

- [CFP Radio Minitel 1](http://cfp-radio.com/realisations/rea48/minitel-01.html)
    - [PDF](CRT/CFPRadio1.pdf)
    - [Mail](CRT/CFPRadio-mail.pdf)
- [CFP Radio Minitel 2](http://cfp-radio.com/realisations/rea48/minitel-02.html)
    - [PDF](CRT/CFPRadio2.pdf)
- [Jla](http://jla.1313-blog.overblog.com/recyclage-d-un-minitel-9-en-moniteur-b/w)
    - [Wayback](https://web.archive.org/web/20131228235557/http://jla.1313-blog.overblog.com/recyclage-d-un-minitel-9-en-moniteur-b/w)
    - [PDF](CRT/jla.pdf)
    - [Mail](CRT/jla-mail.pdf)
    - [Board](CRT/jla-ob_8fa9ba8a03352d48b94c1b4ec49454e7_minitel.Tci)
    - [Board software](CRT/jla-tci4-4.exe)
- [MiniPi Prequel](https://virtualabs.fr/Super-Minitel-Entertainment-System)
    - [Wayback](https://web.archive.org/web/20161227100240/http://www.virtualabs.fr:80/Super-Minitel-Entertainment-System)
    - [PDF](CRT/virtualabs-prequel.pdf)
- [MiniPi v1](https://virtualabs.fr/openit/doku.php?id=projets:minipi-v1)
    - [PDF](CRT/virtualabs-minipi-v1.pdf)
    - [Board](CRT/virtualabs-minipi-v1-oshpark.brd)
    - [Git](CRT/virtualabs-minipi-v1-github.zip)
- [Labomedia](https://wiki.labomedia.org/index.php/Renaissance_d%27un_Minitel_avec_une_Raspberry_Pi)
    - [PDF](CRT/Labomedia.pdf)

## Connecting the Keyboard to GPIO

Tested on a Raspberry Pi 4B, should work on any Pi with a 40-pin header (except the Pi 5 for the C version).

Connect the keyboard ribbon cable to these BCM GPIO IDs:

- For X0 to X8 (see ribbon cable, it's written on it, yes, really)
    - 14, 15, 18, 23, 24, 25,  8,  7, 1
- For Y0 to Y8 
    - 6, 5, 0, 11, 9, 10, 22, 27, 17

The keyboard matrix has no diodes: holding 3 keys at the corners of a rectangle (e.g. W, B, D) makes the 4th one (G) look pressed. Both drivers ignore new key presses in such a configuration.

Update keyboard config at `/etc/default/keyboard` (or use `sudo raspi-config` > Localization > Keyboard):

```
# KEYBOARD CONFIGURATION FILE

# Consult the keyboard(5) manual page.

XKBMODEL="pc105"
XKBLAYOUT="fr"
XKBVARIANT=""
XKBOPTIONS="lv3:ralt_switch,compose:rctrl"

BACKSPACE="guess"
```

Two equivalent drivers are available, installing one disables the other:

- [KeyboardC](./KeyboardC): `cd KeyboardC && sudo make install`
- [KeyboardPy](./KeyboardPy): `cd KeyboardPy && sudo make install` (also installs the required apt packages)

Both can be removed with `sudo make uninstall`. To debug, stop the service and run the driver manually with `coords` (prints the matrix position of each key) or `names` (prints the key label), e.g. `sudo ./minitel-keyboard-c coords` or `sudo ./keyboard.py coords`.

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


## SuperDrive

Apple SuperDrive requires specific commands to enable it, see [this link](https://gist.github.com/AnnoyingTechnology/dbaae864822cf08372f0aafe64a63477?permalink_comment_id=3912068#gistcomment-3912068).

Adapt using the proper USB vendor/product IDs, in our case: `ID 13fd:0840 Initio Corporation INIC-1618L SATA`

## Gaming

- <https://chimeraos.org/>
- <https://www.recalbox.com/>
- [Doom](https://pimylifeup.com/raspberry-pi-doom/)
- [Box64 x Wine](https://github.com/ptitSeb/box64/blob/main/docs/X64WINE.md)

## HDMI Overscan

> <https://raspberrypi.stackexchange.com/a/145670>

Add this to `/boot/firmware/cmdline.txt`

`video=HDMI-A-1:1024x768@60,margin_left=20,margin_right=20,margin_top=32,margin_bottom=24`
