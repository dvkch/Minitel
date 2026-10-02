all: keyboard

keyboard: keyboard.cpp
	g++ -Wall -Wextra -std=c++11 -O3 -o keyboard-c keyboard.cpp

install: keyboard
	cp keyboard-c /usr/local/bin/
	cp keyboard-c.service /etc/systemd/system/
	systemctl enable keyboard-c.service

uninstall:
	systemctl disable --now keyboard-c.service
	rm -f /etc/systemd/system/keyboard-c.service
	rm -f /usr/local/bin/keyboard-c

clean:
	rm -f keyboard-c
