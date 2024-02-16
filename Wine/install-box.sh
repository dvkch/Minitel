#!/bin/bash

############
### FROM
### https://github.com/ptitSeb/box64/blob/main/docs/COMPILE.md
############

set -e

sudo wget https://ryanfortner.github.io/box64-debs/box64.list -O /etc/apt/sources.list.d/box64.list
wget -qO- https://ryanfortner.github.io/box64-debs/KEY.gpg | sudo gpg --dearmor -o /etc/apt/trusted.gpg.d/box64-debs-archive-keyring.gpg

sudo wget https://itai-nelken.github.io/weekly-box86-debs/debian/box86.list -O /etc/apt/sources.list.d/box86.list
wget -qO- https://itai-nelken.github.io/weekly-box86-debs/debian/KEY.gpg | sudo gpg --dearmor -o /etc/apt/trusted.gpg.d/box86-debs-archive-keyring.gpg

sudo dpkg --add-architecture armhf
sudo apt update
sudo apt install box86:armhf libc6:armhf -y
sudo apt install box64-arm64 -y
