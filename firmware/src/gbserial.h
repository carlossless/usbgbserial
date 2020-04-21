#ifndef GBSERIAL_H
#define GBSERIAL_H

#include <stdint.h>

int gbserial_init();

int gbserial_transceive(uint8_t input, uint8_t *output);
int gbserial_send(uint8_t input);
int gbserial_receive(uint8_t *output);

#endif
