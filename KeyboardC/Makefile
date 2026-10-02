all: keyboard

keyboard: keyboard.cpp
	g++ -Wall -Wextra -std=c++11 -O3 -o keyboard keyboard.cpp

install: keyboard
	cp keyboard /usr/local/bin/
	cp keyboard.service /etc/systemd/system/
	systemctl enable keyboard.service

uninstall:
	systemctl disable --now keyboard.service
	rm -f /etc/systemd/system/keyboard.service
	rm -f /usr/local/bin/keyboard

clean:
	rm -f keyboard
