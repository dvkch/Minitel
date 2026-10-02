#pragma once

#include <stdint.h>

// Keyboard ribbon cable: X0 to X8 are the rows, driven, Y0 to Y8 the columns, read
constexpr uint8_t ROW_PINS[] = { 4, 5, 6, 7, 15, 16, 17, 18, 8 };
constexpr uint8_t COL_PINS[] = { 1, 2, 42, 41, 40, 39, 38, 47, 21 };
