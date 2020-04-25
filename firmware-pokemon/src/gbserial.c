#include "gbserial.h"
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

// Connections:
// GB Link Cable | AVR PIN | AVR INT
// --------------+---------+--------
// GBSO          | PD0     | INT0
// GBSI          | PD1     | INT1
// GBSR          | PD2     | INT2
// GBSC          | PD3     | INT3

#define bit_get(p, m) ((p) & (m))
#define bit_set(p, m) ((p) |= (m))
#define bit_clear(p, m) ((p) &= ~(m))
#define bit_flip(p, m) ((p) ^= (m))
#define bit_write(c, p, m) (c ? bit_set(p, m) : bit_clear(p, m))

#define gbserial_read_bit(r, p, b) b |= bit_get(r, _BV(p)) >> p
#define gbserial_write_bit(r, p, b) bit_write((b & 0b10000000) >> 7, r, _BV(p))

volatile uint8_t gb_bit = 0; // current_bit
volatile uint8_t gb_sin = 0;
volatile uint8_t gb_sout = 0;

// falling edge interupt
ISR(INT3_vect)
{
  gbserial_read_bit(PIND, 1, gb_sin);
  gbserial_write_bit(PORTD, 0, gb_sout);

  gb_bit++;
  if (gb_bit < 8)
  {
    gb_sin <<= 1;
    gb_sout <<= 1;
  }
}

void gbserial_init()
{
  DDRD = 0b00000001;  // PD3/INT3 = INPUT, PD2 = N/A, PD1 = INPUT, PD0 = OUTPUT.
  PORTD = 0b00001111; // set internal pullups for all inputs.

  EICRA = 0b10000000; // ISC31 = 1, ISC30 = 0, falling edge of INT3 generates the interupt.
  EIMSK = 0b00001000; // enable INT3 interupt.
}

void gbserial_transceive(uint8_t input, uint8_t *output)
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
}

void gbserial_transceive_bytes(uint8_t *input, uint8_t *output, int length)
{
  for (int i = 0; i < length; i++)
  {
    gbserial_transceive(input[i], &output[i]);
  }
}

void gbserial_send(uint8_t input)
{
  uint8_t tmp;
  gbserial_transceive(input, &tmp);
}

void gbserial_receive(uint8_t *output)
{
  gbserial_transceive(0x00, output);
}
