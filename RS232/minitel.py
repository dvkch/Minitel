#!/usr/bin/python
import serial
import glob

# pip install console-menu
from consolemenu import *
from consolemenu.items import *

def clearMinitel():
    with serial.Serial('/dev/ttyUSB0', 4800, parity=serial.PARITY_EVEN, bytesize=7, timeout=5) as ser:
        ser.write(b'\033c')

def showMenu():
    files = [f for f in glob.glob("./minitel_*.txt")]
    menu = SelectionMenu(files, "Which file?")
    menu.show()
    index = menu.selected_option
    if index < len(files):
        return files[index]
    return None

def printFile(path):
    lines = [line.rstrip('\n') for line in open(path)]
    clearMinitel()
    with serial.Serial('/dev/ttyUSB0', 4800, parity=serial.PARITY_EVEN, bytesize=7, timeout=5) as ser:
        for i in range(len(lines)):
            if i > 0:
                ser.write(b'\r\n')
            ser.write(lines[i].ljust(80, ' '))

clearMinitel()
path = showMenu()
if path is not None:
    printFile(path)
