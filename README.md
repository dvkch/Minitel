# Minitel

## Generic

Useful links:

- <https://hackaday.com/tag/minitel/>

## Over RS232

cf [RS232](./RS232)

Useful links:

- <https://pila.fr/wordpress/?p=361>
    - [PDF](RS232/docs/Un%20minitel%20comme%20terminal%20linux%20USB.pdf)

## Reusing the CRT display

cf [CRT](./CRT)

Useful links:

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

Connect all available GPIO pins, starting by ping  (GPIO BCM 14), up until pin 7 (BCM 4), then use pins 3, 4 and 5 of the P5 pinout.

This should be the BCM GPIO IDs:

- For X0 to X8 (see ribbon cable, it's written on it, yes, really)
    - 14, 15, 18, 23, 24, 25,  8,  7, 11
- For Y0 to Y8 
    -  9, 10, 22, 27, 17,  4, 28, 19, 30

Useful links:

- [Labomedia](https://wiki.labomedia.org/index.php/Renaissance_d%27un_Minitel_avec_une_Raspberry_Pi)
- [GPIO Matrix Keyboard](https://github.com/nutki/rpi-gpio-matrix-keyboard/blob/master/gpio_keyboard.py)
- [P5 pinout (BCM)](https://elinux.org/RPi_Low-level_peripherals#P5_header)
- [P5 pinout (WiringPi)](https://pi4j.com/1.2/pins/model-a-rev2.html)
