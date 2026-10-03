#pragma once

#include <Arduino.h>
#include "KeyDef.h"

// Key events as printed by the console logs
void printKeyCoords(Print &out, uint8_t row, uint8_t col, bool pressed);
void printKeyName(Print &out, uint8_t row, uint8_t col, bool pressed, KeyVariant variant);
