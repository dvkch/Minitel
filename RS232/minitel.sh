#!/bin/bash

stty -F /dev/ttyUSB0 4800 istrip cs7 parenb -parodd brkint ignpar icrnl ixon ixany opost onlcr cread hupcl isig icanon echo echoe echok
# agetty -c ttyUSB0 4800 minitel1b-80

