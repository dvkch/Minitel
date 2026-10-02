NAME = minitel-keyboard-py
OTHER = minitel-keyboard-c
PACKAGES = python3-rpi-lgpio python3-evdev

all:
	@echo "Nothing to build, use 'sudo make install' or 'sudo make uninstall'"

# no -y: apt asks before installing extras or replacing python3-rpi.gpio
deps:
	apt-get install $(PACKAGES)

install: deps
	-systemctl disable --now $(OTHER).service 2>/dev/null
	install -m 755 keyboard.py /usr/local/bin/$(NAME)
	install -m 644 $(NAME).service /etc/systemd/system/
	systemctl daemon-reload
	systemctl enable --now $(NAME).service

uninstall:
	-systemctl disable --now $(NAME).service
	rm -f /etc/systemd/system/$(NAME).service /usr/local/bin/$(NAME)
	systemctl daemon-reload
	@echo "Kept $(PACKAGES), run 'sudo make remove-deps' to remove them"

remove-deps:
	apt-get remove $(PACKAGES)

.PHONY: all deps install uninstall remove-deps
