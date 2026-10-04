# Minitel

Once upon a time, not so long ago, an ADHD-induced fantasy crossed my mind and I decided to go get an old [Minitel](https://en.wikipedia.org/wiki/Minitel) to modernize it.

After some weeks, the final work is as such:

- style:
  - the original box has been kept mostly intact
  - kept the original power button, cutting the main 12V input
  - added back an LED for status
- computing:
  - a Raspberry Pi 4 is the new brain
- display: 
  - the CRT screen has been cut to keep only the facade
  - an iPad2 screen with a driver board now handles the display
- keyboard: 
  - the original keyboard is connected to the RPi, originally via its GPIO pins, now via an ESP32-S3 acting as a USB keyboard
- additional IO:
  - an old SuperDrive now allows for reading and burning CDs/DVDs
  - a PS/2 adapter connects a beautiful IBM trackball mouse
  - a speaker and a mic are connected through a simple USB audio card

This is now my retro gaming station, and as any project should, this repo contains useful information and code to reproduce the same.

## Inside this repo

- [RS232](./RS232): Chatting to a Minitel with a reversible model via RS232
- [CRT](./CRT): Reusing the original CRT
- [KeyboardC](./KeyboardC): User-land drivers in C++ to test and use the original keyboard, wired directly to the RPi's GPIO pins
- [KeyboardPy](./KeyboardPy): Python equivalent of KeyboardC, technically its ancestor, easier to modify if you'd like to play with the code
- [Keyboard32](./Keyboard32): ESP32-S3 firmware acting as a USB keyboard for the original keyboard, the recommended option: no driver needed, works with any OS

## Useful Links

- Similar projects
  - <https://hackaday.com/tag/minitel/>

- MultiBoot on a RPi
  - <https://raspberrytips.fr/dual-boot-raspberry-pi/>
  - <https://sourceforge.net/projects/pinn/>
  - <https://github.com/procount/pinn>

- Gaming distributions:
  - <https://www.recalbox.com/>
  - <https://chimeraos.org/>

- Gaming software
  - [Doom](https://pimylifeup.com/raspberry-pi-doom/)
  - [Box64 x Wine](https://github.com/ptitSeb/box64)
    - Install [Pi-Apps](https://github.com/Botspot/pi-apps)
    - In Pi-Apps, install **Wine (x64)**
    - Run a game with `wine game.exe`, configure with `winecfg`


- SuperDrive
  - Currently using a 2009 MacBook Pro SuperDrive in a SATA-USB enclosure kit (`13fd:0840 Initio Corporation INIC-1618L SATA`), powered from the internal 5V via a USB Y cable. It works out of the box.
  - If you're using an external Apple USB SuperDrive (`05ac:1500`), you'll need to send a wake command before it accepts discs, see [this link](https://gist.github.com/AnnoyingTechnology/dbaae864822cf08372f0aafe64a63477?permalink_comment_id=3912068#gistcomment-3912068). You can do this automatically using the following udev rule:

    ```sh
    sudo apt install sg3-utils
    echo 'ACTION=="add", SUBSYSTEM=="block", KERNEL=="sr[0-9]*", ATTRS{idVendor}=="05ac", ATTRS{idProduct}=="1500", RUN+="/usr/bin/sg_raw --cmdset=1 /dev/%k EA 00 00 00 00 00 01"' | sudo tee /etc/udev/rules.d/90-superdrive.rules
    sudo reboot
    ```

- HDMI Overscan
  - <https://raspberrypi.stackexchange.com/a/145670>
  - Add this to `/boot/firmware/cmdline.txt`: `video=HDMI-A-1:1024x768@60,margin_left=20,margin_right=20,margin_top=32,margin_bottom=24`
