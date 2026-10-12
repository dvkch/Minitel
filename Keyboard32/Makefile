# PlatformIO and its toolchains are installed inside this folder, `make clean-deps` removes them all
VENV = .venv
PIO = $(VENV)/bin/pio
export PLATFORMIO_CORE_DIR = $(CURDIR)/.platformio

# optional serial port, e.g. `make flash PORT=/dev/ttyACM0`, auto-detected otherwise
PORT ?=
PORT_ARG = $(if $(PORT),--port $(PORT))
UPLOAD_PORT_ARG = $(if $(PORT),--upload-port $(PORT))

all: build

deps: $(PIO)

# python3-venv is a separate package on Debian based systems (Raspberry Pi OS, Ubuntu)
$(PIO):
	@python3 -c "import ensurepip" 2>/dev/null || sudo apt-get install python3-venv
	python3 -m venv $(VENV)
	$(VENV)/bin/pip install platformio

# the first build downloads the platform, toolchain and Arduino core (~6 GB), pruned right after
build: $(PIO)
	$(PIO) run
	@$(MAKE) --no-print-directory prune

flash: build
	$(PIO) run -t upload $(UPLOAD_PORT_ARG)

# the monitor needs the firmware built, to decode crashes
monitor: $(PIO)
	$(PIO) device monitor $(PORT_ARG)

# the same console without PlatformIO, through tio (crashes aren't decoded): useful once the dependencies are removed
# the firmware ends lines with a bare LF, which tio shows as is: INLCRNL adds the CR terminals need
CONSOLE_PORT = $(or $(PORT),$(firstword $(wildcard /dev/serial/by-id/*Minitel* /dev/cu.usbmodem*)))
console:
	@command -v tio >/dev/null || { echo "tio is needed: sudo apt install tio (macOS: brew install tio)"; exit 1; }
	@test -n "$(CONSOLE_PORT)" || { echo "keyboard not found, set PORT, e.g. make console PORT=/dev/ttyACM0"; exit 1; }
	tio --local-echo --map INLCRNL $(CONSOLE_PORT)

erase: build
	$(PIO) run -t erase $(UPLOAD_PORT_ARG)

# Trims dependencies to a minimum, from ~6 GB after first build to ~1.5GB. Removes download caches, the installer's copy
# of the tools, Arduino and compiler libraries for other chips, and the unused picolibc C library
prune:
	rm -rf .platformio/.cache .platformio/dist .platformio/tools
	find .platformio/packages/framework-arduinoespressif32-libs -mindepth 1 -maxdepth 1 \
		-type d -name 'esp32*' ! -name esp32s3 -exec rm -rf {} +
	rm -rf .platformio/packages/toolchain-xtensa-esp-elf/picolibc \
		.platformio/packages/toolchain-xtensa-esp-elf/xtensa-esp-elf/lib/esp32 \
		.platformio/packages/toolchain-xtensa-esp-elf/xtensa-esp-elf/lib/esp32s2

clean:
	rm -rf .pio

clean-deps: clean
	rm -rf $(VENV) .platformio

.PHONY: all deps build flash monitor console erase prune clean clean-deps