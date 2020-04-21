#include "gbserial.h"
#include <avr/interrupt.h>

// Connections:
// GB Link Cable | AVR PIN | AVR INT
// --------------+---------+--------
// GBSO          | PD0     | INT0
// GBSI          | PD1     | INT1
// GBSR          | PD2     | INT2
// GBSC          | PD3     | INT3

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))

volatile uint8_t gb_bit = 0;
volatile uint8_t gb_sin = 0;
volatile uint8_t gb_sout = 0;

// falling edge interup
ISR(INT3_vect)
{
  gb_sin |= bit_get(PIND, BIT(1)) >> 1;
  bit_write((gb_sout & 0b10000000) >> 7, PORTD, BIT(0)); //output
  gb_bit++;

  if (gb_bit < 8) {
    gb_sin <<= 1;
    gb_sout <<= 1;
  }
}

int gbserial_init()
{
  DDRD = 0b00000001; // PD3/INT3 = INPUT, PD2 = N/A, PD1 = INPUT, PD0 = OUTPUT

  EICRA = 0b10000000; // ISC31 = 1, ISC30 = 1, falling edge of INT3 generates the interupt
  EIMSK = 0b00001000; // INT3

  return 0;
}

int gbserial_transceive(uint8_t input, uint8_t *output)
{
  gb_bit = 0;
  gb_sin = 0;
  gb_sout = input;

  sei();
  while (1)
  {
    if (gb_bit > 7)
    {
      break;
    }
  }
  cli();
  *output = gb_sin;
  return 0;
}

int gbserial_send(uint8_t input)
{
  uint8_t tmp;
  return gbserial_transceive(input, &tmp);
}

int gbserial_receive(uint8_t *output)
{
  return gbserial_transceive(0x00, output);
}
