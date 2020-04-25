#include "utils.h"
#include "gbserial.h"

void send_bytes(uint8_t *bytes, int length)
{
  for (int i = 0; i < length; i++)
  {
    gbserial_send(bytes[i]);
  }
}

void pad_with_zeroes(int length)
{
  repeat_values(0x00, length);
}

void repeat_values(uint8_t value, int length)
{
  for (int i = 0; i < length; i++)
  {
    gbserial_send(value);
  }
}
