#!/bin/bash

############
### FROM
### https://github.com/ptitSeb/box64/blob/main/docs/X64WINE.md
############

# NOTE: Can only run on aarch64 (since box64 can only run on aarch64)
# box64 runs wine-amd64, box86 runs wine-i386.

### User-defined Wine version variables ################
# - Replace the variables below with your system's info.
# - Note that we need the amd64 version for Box64 even though we're installing it on our ARM processor.
# - Note that we need the i386 version for Box86 even though we're installing it on our ARM processor.
# - Wine download links from WineHQ: https://dl.winehq.org/wine-builds/

set -e

branch="devel" #example: devel, staging, or stable (wine-staging 4.5+ requires libfaudio0:i386)
#version="8.21" #example: "7.1"
version="7.15" # anything more recent runs into this issue: https://github.com/ptitSeb/box86/issues/600
id="debian" #example: debian, ubuntu
dist="bookworm" #example (for debian): bullseye, buster, jessie, wheezy, ${VERSION_CODENAME}, etc 
tag="-1" #example: -1 (some wine .deb files have -1 tag on the end and some don't)

######################################################

# Clean up any old wine instances
echo "Cleaning up"
#[ -x "$(command -v wineserver)" ] && wineserver -k # stop any old wine installations from running
rm -rf ~/.cache/wine # remove any old wine-mono/wine-gecko install files
rm -rf ~/.local/share/applications/wine # remove any old program shortcuts

# Backup any old wine installs
echo "Backing up old installs"
[ -d "~/wine-old" ] && rm -rf ~/wine-old
[ -d "~/wine" ] && mv ~/wine ~/wine-old
[ -d "~/.wine-old" ] && rm -rf ~/.wine-old
[ -d "~/.wine" ] && mv ~/.wine ~/.wine-old
sudo mv /usr/local/bin/wine /usr/local/bin/wine-old 2>/dev/null || true
sudo mv /usr/local/bin/wine64 /usr/local/bin/wine-old 2>/dev/null || true
sudo mv /usr/local/bin/wineboot /usr/local/bin/wineboot-old 2>/dev/null || true
sudo mv /usr/local/bin/winecfg /usr/local/bin/winecfg-old 2>/dev/null || true
sudo mv /usr/local/bin/wineserver /usr/local/bin/wineserver-old 2>/dev/null || true

# Wine download links from WineHQ: https://dl.winehq.org/wine-builds/
echo "Generating links"
LNKA="https://dl.winehq.org/wine-builds/${id}/dists/${dist}/main/binary-amd64/" #amd64-wine links
DEB_A1="wine-${branch}-amd64_${version}~${dist}${tag}_amd64.deb" #wine64 main bin
DEB_A2="wine-${branch}_${version}~${dist}${tag}_amd64.deb" #wine64 support files (required for wine64 / can work alongside wine_i386 main bin)
    #DEB_A3="winehq-${branch}_${version}~${dist}${tag}_amd64.deb" #shortcuts & docs
LNKB="https://dl.winehq.org/wine-builds/${id}/dists/${dist}/main/binary-i386/" #i386-wine links
DEB_B1="wine-${branch}-i386_${version}~${dist}${tag}_i386.deb" #wine_i386 main bin
DEB_B2="wine-${branch}_${version}~${dist}${tag}_i386.deb" #wine_i386 support files (required for wine_i386 if no wine64 / CONFLICTS WITH wine64 support files)
    #DEB_B3="winehq-${branch}_${version}~${dist}${tag}_i386.deb" #shortcuts & docs

# Install amd64-wine (64-bit) alongside i386-wine (32-bit)
echo -e "Downloading wine . . ."
wget -q --show-progress ${LNKA}${DEB_A1} -O ${DEB_A1}
wget -q --show-progress ${LNKA}${DEB_A2} -O ${DEB_A2}
wget -q --show-progress ${LNKB}${DEB_B1} -O ${DEB_B1}

echo -e "Extracting wine . . ."
[ -d "wine-installer" ] && rm -rf wine-installer
dpkg-deb -x ${DEB_A1} wine-installer
dpkg-deb -x ${DEB_A2} wine-installer
dpkg-deb -x ${DEB_B1} wine-installer

echo -e "Installing wine . . ."
mv wine-installer/opt/wine* ~/wine

# Download wine dependencies
# - these packages are needed for running box86/wine-i386 on a 64-bit RPiOS via multiarch
echo "Downloading armhf dependencies"
sudo dpkg --add-architecture armhf && sudo apt-get update # enable multi-arch
# required packages (i386)
sudo apt-get install -y libasound2:armhf libc6:armhf libglib2.0-0:armhf \
    libgphoto2-6:armhf libgphoto2-port12:armhf libgstreamer-plugins-base1.0-0:armhf \
    libgstreamer1.0-0:armhf libldap-2.5-0:armhf libopenal1:armhf libpcap0.8:armhf \
    libpulse0:armhf libsane1:armhf libudev1:armhf libusb-1.0-0:armhf libx11-6:armhf \
    libxext6:armhf ocl-icd-libopencl1:armhf libasound2-plugins:armhf libncurses6:armhf 

# recommended packages (i386)
sudo apt-get install -y libcap2-bin:armhf libcups2:armhf libdbus-1-3:armhf \
    libfontconfig1:armhf libfreetype6:armhf libglu1-mesa:armhf libgnutls30:armhf \
    libgssapi-krb5-2:armhf libkrb5-3:armhf libodbc1:armhf libosmesa6:armhf \
    libsdl2-2.0-0:armhf libv4l-0:armhf libxcomposite1:armhf libxcursor1:armhf \
    libxfixes3:armhf libxi6:armhf libxinerama1:armhf libxrandr2:armhf \
    libxrender1:armhf libxxf86vm1:armhf 
    # to run wine-i386 through box86:armhf on aarch64
    # This list found by downloading...
    #   wget https://dl.winehq.org/wine-builds/debian/dists/bullseye/main/binary-i386/wine-devel-i386_7.1~bullseye-1_i386.deb
    #   wget https://dl.winehq.org/wine-builds/debian/dists/bullseye/main/binary-i386/winehq-devel_7.1~bullseye-1_i386.deb
    #   wget https://dl.winehq.org/wine-builds/debian/dists/bullseye/main/binary-i386/wine-devel_7.1~bullseye-1_i386.deb
    # then `dpkg-deb -I package.deb`. Read output, add `:armhf` to packages in dep list, then try installing them on Pi aarch64.
    
# - these packages are needed for running box64/wine-amd64 on RPiOS (box64 only runs on 64-bit OS's)
echo "Downloading arm64 dependencies"
# required packages (x64)
sudo apt-get install -y libasound2:arm64 libc6:arm64 libglib2.0-0:arm64 \
    libgphoto2-6:arm64 libgphoto2-port12:arm64 libgstreamer-plugins-base1.0-0:arm64 \
    libgstreamer1.0-0:arm64 libldap-2.5-0:arm64 libopenal1:arm64 libpcap0.8:arm64 \
    libpulse0:arm64 libsane1:arm64 libudev1:arm64 libunwind8:arm64 libusb-1.0-0:arm64 \
    libx11-6:arm64 libxext6:arm64 ocl-icd-libopencl1:arm64 libasound2-plugins:arm64 \
    libncurses6:arm64 debconf:arm64 

# recommended packages (x64)
sudo apt-get install -y libcap2-bin:arm64 libcups2:arm64 libdbus-1-3:arm64 \
    libfontconfig1:arm64 libfreetype6:arm64 libglu1-mesa:arm64 libgnutls30:arm64 \
    libgssapi-krb5-2:arm64 libjpeg62-turbo:arm64 libkrb5-3:arm64 libodbc1:arm64 \
    libosmesa6:arm64 libsdl2-2.0-0:arm64 libv4l-0:arm64 libxcomposite1:arm64 \
    libxcursor1:arm64 libxfixes3:arm64 libxi6:arm64 libxinerama1:arm64 \
    libxrandr2:arm64 libxrender1:arm64 libxxf86vm1:arm64 

    # This list found by downloading...
    #   wget https://dl.winehq.org/wine-builds/debian/dists/bullseye/main/binary-amd64/wine-devel_7.1~bullseye-1_amd64.deb
    #   wget https://dl.winehq.org/wine-builds/debian/dists/bullseye/main/binary-amd64/wine-devel-amd64_7.1~bullseye-1_amd64.deb
    # then `dpkg-deb -I package.deb`. Read output, add `:arm64` to packages in dep list, then try installing them on Pi aarch64.    

# These packages are needed for running wine-staging on RPiOS (Credits: chills340)
echo "Downloading RPi dependencies"
sudo apt install libstb0 -y
cd ~/Downloads
wget -r -l1 -np -nd -A "libfaudio0_*~bpo10+1_i386.deb" http://ftp.us.debian.org/debian/pool/main/f/faudio/ # Download libfaudio i386 no matter its version number
dpkg-deb -xv libfaudio0_*~bpo10+1_i386.deb libfaudio
sudo cp -TRv libfaudio/usr/ /usr/
rm libfaudio0_*~bpo10+1_i386.deb # clean up
rm -rf libfaudio # clean up

# Install symlinks
echo "Making symlinks"
sudo ln -s ~/wine/bin/wine /usr/local/bin/wine
sudo ln -s ~/wine/bin/wine64 /usr/local/bin/wine64
sudo ln -s ~/wine/bin/wineboot /usr/local/bin/wineboot
sudo ln -s ~/wine/bin/winecfg /usr/local/bin/winecfg
sudo ln -s ~/wine/bin/wineserver /usr/local/bin/wineserver
sudo chmod +x /usr/local/bin/wine /usr/local/bin/wine64 /usr/local/bin/wineboot /usr/local/bin/winecfg /usr/local/bin/wineserver

# Install winetricks
sudo apt install cabextract

cd ~/wine
wget https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks
chmod +x winetricks
sudo ln -s ~/wine/winetricks /usr/local/bin/winetricks

echo "Done"
