#include <stdint.h>
#include "gbserial.h"
#include "pokemon.h"

#define POKEMON_TRADE_CANCELED 0x7F
#define POKEMON_TRADE_ACCEPT 0x72
#define POKEMON_TRADE_CENTER_SELECTION 0xD1

#define NAME_LENGTH 6
#define MAX_POKEMON_COUNT 6

int trainer_name[NAME_LENGTH] = { 0xf7, 0xf9, 0xf9, 0xfd, 0x50, 0x50 };

typedef enum {
  NOT_CONNECTED,
  CONNECTED,
  IN_TRADE_CENTER,
  TRANSMITING,
  TRADE
} t_state;

t_state state;

int intro_sequence()
{
  uint8_t input;

  // get input byte. it will always be 0x01 (player 1) since we're using the gameboy (player 1) clock
  gbserial_receive(&input);
  if (input == 0x01) {
    // we respond with 0x02 (player 2), since we're using the external clock
    gbserial_send(0x02);
  }

  // two 0x00 sent afterwards
  gbserial_send(0x00);
  gbserial_send(0x00);

  // NPC will talk to you
  // need to confirm next steps
  state = CONNECTED;

  // one zero byte sent here
  gbserial_transceive(0x00, &input);
  gbserial_transceive(0x00, &input);

  // 0x62 -- dunno
  while (input == 0x61) {
    gbserial_transceive(0x62, &input);
  }

  // 0x00 -- some padding?
  while (input == 0x00) {
    gbserial_transceive(0x00, &input);
  }

  if (input == POKEMON_TRADE_CENTER_SELECTION) {
    gbserial_send(POKEMON_TRADE_CENTER_SELECTION);
  }

  gbserial_send(0x00);

  state = IN_TRADE_CENTER;

  return 0;
}

int trading()
{
  uint8_t input;

  state = TRADE;

  // 0x00 - one blank byte
  gbserial_receive(&input);
  
  // respond with one 0x00, one byte offset
  gbserial_transceive(0x00, &input);

  // 0x75 - 14 bytes
  for (int i = 0; i < 14; i++) {
    gbserial_transceive(0x75, &input);
  }

  // 0x00 - one byte
  for (int i = 0; i < 10; i++) {
    gbserial_receive(&input);
  }

  // 0x76 - 14 bytes
  for (int i = 0; i < 14; i++) {
    gbserial_transceive(0x76, &input);
  }

  // 0x00 - 12 bytes
  for (int i = 0; i < 12; i++) {
    gbserial_receive(&input);
  }

  // 0xfd - 10 bytes
  for (int i = 0; i < 10; i++) {
    gbserial_transceive(0xfd, &input);
  }

  // 0x?? - 10 bytes
  for (int i = 0; i < 10; i++) {
    gbserial_transceive(input, &input);
  }

  // 0xfd - 9 bytes
  for (int i = 0; i < 9; i++) {
    gbserial_transceive(0xfd, &input);
  }

  state = TRANSMITING;

  // trainer name
  for (int i = 0; i < NAME_LENGTH; i++) {
    gbserial_send(trainer_name[i]);
  }

  int pokemon_count = 3; // 3 pokemon
  gbserial_transceive(pokemon_count, &input);
  for (int i = 0; i < pokemon_count; i++) {
    gbserial_transceive(0x97, &input); // all mews
  }
  gbserial_transceive(0xFF, &input); // end
  for (int i = 0; i < (6 - pokemon_count); i++) {
    gbserial_transceive(0x00, &input); // pad the rest
  }

  // 0x?? - 2 bytes
  for (int i = 0; i < 2; i++) {
    gbserial_transceive(input, &input);
  }

  // 6 pokemons - 6 * 48 = 288 bytes
  for (int i = 0; i < pokemon_count; i++) {
    for (int j = 0; j < 48; j++) {
      gbserial_send(pokemon[j]);
    }
  }
  for (int i = 0; i < (MAX_POKEMON_COUNT - pokemon_count); i++) { // pad the rest
    for (int j = 0; j < 48; j++) {
      gbserial_send(0x00);
    }
  }

  // 36 bytes of some pokemon data - for existing pokemon all 0x80 ending with 0x50
  for (int i = 0; i < pokemon_count; i++) {
    for (int j = 0; j < 5; j++) {
      gbserial_send(0x80);
    }
    gbserial_send(0x50);
  }
  for (int i = 0; i < (MAX_POKEMON_COUNT - pokemon_count); i++) {
    for (int j = 0; j < 6; j++) {
      gbserial_send(0x00);
    }
  }

  // 36 bytes of pokemon names - 5 bytes padded 0x50
  int pokemon_name[NAME_LENGTH] = {0xf7, 0xf9, 0xf9, 0xfd, 0x50, 0x50};
  for (int i = 0; i < pokemon_count; i++) {
    for (int j = 0; j < NAME_LENGTH; j++) {
      gbserial_send(pokemon_name[j]);
    }
  }
  for (int i = 0; i < (MAX_POKEMON_COUNT - pokemon_count); i++) {
    for (int j = 0; j < MAX_POKEMON_COUNT; j++) {
      gbserial_transceive(0x00, &input);
    }
  }

  // 20 bytes of whatever - ???
  for (int i = 0; i < 20; i++) {
    gbserial_transceive(input, &input);
  }

  // end 0xff
  if (input == 0xff) {
    gbserial_send(0xff);
  }

  state = TRADE;

  while (1) {
    gbserial_receive(&input);
    if (input == POKEMON_TRADE_CANCELED) {
      while (input == POKEMON_TRADE_CANCELED) {
        gbserial_transceive(POKEMON_TRADE_CANCELED, &input);
      }
      state = IN_TRADE_CENTER;
      break;
    } else if (input >= 0x70 && input <= 0x75) { // 0-5 chosen pokemon by player 1
      while (input >= 0x70 && input <= 0x75) {
        gbserial_transceive(0x70, &input); // give the first pokemon in return
      }
      while (input == 0x00) {
        gbserial_transceive(0x00, &input);
      }
      if (input == 0x72) { // player 1 has confirmed the trade
        while (input == 0x72) { // player 2 confirms the trade
          gbserial_transceive(0x72, &input);
        }
        while (input == 0x00) {
          gbserial_transceive(0x00, &input);
        }
        while (input >= 0x71) { // the trade has concluded
          gbserial_transceive(0x71, &input);
        }
        state = IN_TRADE_CENTER;
        break;
      } else if (input == 0x71) { // player 1 has canceled the trade
        while (input >= 0x71) {
          gbserial_transceive(0x71, &input); // player 2 confirms the cancelation
        }
      }
    }
  }

  return 0;
}

int main(void)
{
  gbserial_init();

  while (1) {
    switch (state) {
    case NOT_CONNECTED:
      intro_sequence();
      break;
    case IN_TRADE_CENTER:
      trading();
      break;
    default:
      break;
    }
  }

  return 0;
}

