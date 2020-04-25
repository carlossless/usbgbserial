#pragma once

#include <stdint.h>

void gbserial_init();

void gbserial_transceive(uint8_t input, uint8_t *output);
void gbserial_send(uint8_t input);
void gbserial_receive(uint8_t *output);
