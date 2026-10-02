NAME = minitel-keyboard-c
OTHER = minitel-keyboard-py

all: $(NAME)

$(NAME): keyboard.cpp rpi_gpio.h
	g++ -Wall -Wextra -std=c++11 -O3 -o $(NAME) keyboard.cpp

install: $(NAME)
	-systemctl disable --now $(OTHER).service 2>/dev/null
	install -m 755 $(NAME) /usr/local/bin/
	install -m 644 $(NAME).service /etc/systemd/system/
	systemctl daemon-reload
	systemctl enable --now $(NAME).service

uninstall:
	-systemctl disable --now $(NAME).service
	rm -f /etc/systemd/system/$(NAME).service /usr/local/bin/$(NAME)
	systemctl daemon-reload

clean:
	rm -f $(NAME)

.PHONY: all install uninstall clean
