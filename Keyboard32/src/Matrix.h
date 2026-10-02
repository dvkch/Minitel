#pragma once

#include <Arduino.h>
#include "Pins.h"

constexpr uint8_t MATRIX_ROWS = sizeof(ROW_PINS);
constexpr uint8_t MATRIX_COLS = sizeof(COL_PINS);
static_assert(MATRIX_COLS <= 16, "a row is stored as a 16 bits mask");

// Keyboard matrix without diodes: rows are pulled low one at a time, and columns read through pull-ups
class Matrix {
public:
    using KeyHandler = void (*)(uint8_t row, uint8_t col, bool pressed);

    void begin();
    // reads the whole matrix and reports debounced changes, to be called about every millisecond.
    // Returns true if any debounced reading changed, including ghost keys that are not reported
    bool scan(KeyHandler handler);
    void print(Print &out) const;

private:
    uint16_t raw[MATRIX_ROWS] = {};
    uint16_t stable[MATRIX_ROWS] = {};
    uint16_t pressed[MATRIX_ROWS] = {};
    uint32_t changedAt[MATRIX_ROWS][MATRIX_COLS] = {};

    uint16_t readRow(uint8_t row) const;
    bool isAmbiguous(uint8_t row, uint8_t col) const;
};
