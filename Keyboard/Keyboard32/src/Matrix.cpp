#include "Matrix.h"

// a key must read the same for this long before its change is accepted
static constexpr uint32_t DEBOUNCE_MS = 5;
// time for the columns to follow the selected row through the pull-ups
static constexpr uint32_t SETTLE_US = 10;

void Matrix::begin() {
    for (uint8_t pin : ROW_PINS) {
        pinMode(pin, INPUT_PULLUP);
    }
    for (uint8_t pin : COL_PINS) {
        pinMode(pin, INPUT_PULLUP);
    }
}

uint16_t Matrix::readRow(uint8_t row) const {
    uint8_t pin = ROW_PINS[row];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delayMicroseconds(SETTLE_US);

    uint16_t bits = 0;
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        if (digitalRead(COL_PINS[col]) == LOW) {
            bits |= 1 << col;
        }
    }

    // a released row floats low long enough to ghost on the next one, drive it high first
    digitalWrite(pin, HIGH);
    pinMode(pin, INPUT_PULLUP);
    return bits;
}

bool Matrix::scan(KeyHandler handler) {
    uint32_t now = millis();
    bool stableChanged = false;
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        uint16_t bits = readRow(row);
        uint16_t changed = bits ^ raw[row];
        raw[row] = bits;

        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint16_t mask = 1 << col;
            if (changed & mask) {
                changedAt[row][col] = now;
            }
            else if (now - changedAt[row][col] >= DEBOUNCE_MS && (stable[row] & mask) != (bits & mask)) {
                stable[row] ^= mask;
                stableChanged = true;
            }
        }
    }

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint16_t mask = 1 << col;
            bool isDown = stable[row] & mask;
            bool wasDown = pressed[row] & mask;

            if (isDown && !wasDown && !isAmbiguous(row, col)) {
                pressed[row] |= mask;
                handler(row, col, true);
            }
            else if (!isDown && wasDown) {
                pressed[row] &= ~mask;
                handler(row, col, false);
            }
        }
    }
    return stableChanged;
}

// Without diodes, 3 keys held at the corners of a rectangle make the 4th one look pressed. This happens
// when two rows share at least two columns: a new press in such a configuration is ignored.
bool Matrix::isAmbiguous(uint8_t row, uint8_t col) const {
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        if (i != row && (stable[i] & (1 << col)) && (stable[i] & stable[row] & ~(1 << col))) {
            return true;
        }
    }
    return false;
}

void Matrix::print(Print &out) const {
    out.print("   ");
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        out.printf(" Y%u", col);
    }
    out.println();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        out.printf("X%u ", row);
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint16_t mask = 1 << col;
            char state = (pressed[row] & mask) ? '#' : (stable[row] & mask) ? '?' : '.';
            out.printf("  %c", state);
        }
        out.println();
    }
}
