PACKAGES = python3-rpi-lgpio python3-evdev

all:
	@echo "Nothing to build, use 'sudo make install' or 'sudo make uninstall'"

deps:
	apt-get install $(PACKAGES)

install: deps
	install -m 755 keyboard.py /usr/local/bin/keyboard-py
	install -m 644 keyboard-py.service /etc/systemd/system/
	systemctl daemon-reload
	systemctl enable --now keyboard-py.service

uninstall:
	-systemctl disable --now keyboard-py.service
	rm -f /etc/systemd/system/keyboard-py.service /usr/local/bin/keyboard-py
	systemctl daemon-reload
	@echo "Kept $(PACKAGES), run 'sudo make remove-deps' to remove them"

remove-deps:
	apt-get remove $(PACKAGES)

.PHONY: all deps install uninstall remove-deps
