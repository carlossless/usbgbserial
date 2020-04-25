#pragma once

#include <stdint.h>

void send_bytes(uint8_t *bytes, int length);
void pad_with_zeroes(int length);
void repeat_values(uint8_t value, int length);
