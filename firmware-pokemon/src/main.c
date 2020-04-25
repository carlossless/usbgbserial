#include <stdint.h>
#include "gbserial.h"
#include "utils.h"
#include "pokemon_payload.h"

#define POKEMON_PLAYER_ONE 0x01
#define POKEMON_PLAYER_TWO 0x02

#define POKEMON_TRADE_CENTER_SELECTION 0xD1

#define POKEMON_TRANSFER_PREAMBLE 0xfd

#define POKEMON_TRADE_CANCELED 0x7F
#define POKEMON_TRADE_ACCEPT 0x72

#define NAME_LENGTH 6
#define POKEMON_NAME_LENGTH 11
#define MAX_POKEMON_COUNT 6
#define POKEMON_COUNT 3

#define TRAINER_ID \
  {                \
    0xde, 0xad     \
  }

const uint8_t trainer_id[2] = TRAINER_ID;

// https://github.com/pret/pokecrystal/blob/master/charmap.asm#L409-L418
uint8_t trainer_name[NAME_LENGTH] = {0xf7, 0xf9, 0xf9, 0xfd, 0x50, 0x50}; // 1337 in the japaneese charmap
uint8_t pokemon_name[NAME_LENGTH] = {0xf7, 0xf9, 0xf9, 0xfd, 0x50, 0x50};
uint8_t original_trainer_name[NAME_LENGTH] = {0xf7, 0xf9, 0xf9, 0xfd, 0x50, 0x50};

const pokemon_payload mew = {
    .species = 0x97,                   // Mew
    .held_item = 0x01,                 // Master Ball
    .moves = {0x25, 0x37, 0x38, 0x26}, // Mega Kick, Thrash, Double Edge, Jump Kick
    .trainer_id = TRAINER_ID,
    .experience = 1059860,
    .hp_ev = 0,
    .attack_ev = 0,
    .defense_ev = 0,
    .speed_ev = 0,
    .special_ev = 0,
    .iv = 0,
    .move_pps = {20, 25, 5, 15}, // 20/20, 25/25, 5/5, 15/15
    .friendship = 80,
    .pokerus = 0,
    .caught_data = 0,
    .level = 100,
    .status = 0, // Normal
    .unused = 0,
    .hp = 256,
    .max_hp = 256,
    .attack = 256,
    .defense = 256,
    .speed = 256,
    .special_attack = 256,
    .special_defense = 256};

typedef enum
{
  NOT_CONNECTED,
  CONNECTED,
  IN_TRADE_CENTER,
  TRADE_SELECTION,
  TRADE_CONFIRMATION,
  TRADE_FINALIZATION
} t_state;

t_state state = NOT_CONNECTED;

void connect()
{
  uint8_t input;

  // get input byte. it will always be 0x01 (player 1) since we're using the gameboy (player 1) clock
  gbserial_receive(&input);
  if (input == POKEMON_PLAYER_ONE)
  {
    // we respond with 0x02 (player 2), since we're using the external clock from player 1
    gbserial_send(POKEMON_PLAYER_TWO);
  }

  // two 0x00 sent afterwards
  pad_with_zeroes(2);

  state = CONNECTED;
}

void enter_trade_center()
{
  uint8_t input;

  // one 0x00 padding.
  gbserial_receive(&input);

  // 0x62 -- unknown
  repeat_values(0x62, 13);

  pad_with_zeroes(10);

  gbserial_receive(&input);
  if (input == POKEMON_TRADE_CENTER_SELECTION)
  {
    gbserial_send(POKEMON_TRADE_CENTER_SELECTION);
  }

  pad_with_zeroes(2);

  state = IN_TRADE_CENTER;
}

void trade_initiated()
{
  uint8_t input;

  // 0x00 - one blank byte
  gbserial_receive(&input);

  // 0x75 - 13 bytes
  repeat_values(0x75, 13);
  pad_with_zeroes(9);

  // 0x00 - one blank byte
  pad_with_zeroes(1);

  // 0x76 - 13 bytes
  repeat_values(0x76, 13);

  // 0x00 - 12 bytes
  for (int i = 0; i < 12; i++)
  {
    gbserial_receive(&input);
  }

  // 0xfd - 9 bytes
  repeat_values(POKEMON_TRANSFER_PREAMBLE, 9);

  gbserial_receive(&input); // first byte of random data

  // mirror 10 bytes of random data
  for (int i = 0; i < 10; i++)
  {
    gbserial_transceive(input, &input);
  }

  // 0xfd - 9 bytes
  repeat_values(POKEMON_TRANSFER_PREAMBLE, 9);

  // trainer name - 6 bytes
  send_bytes(trainer_name, NAME_LENGTH);

  // number of pokemon in party - 1 byte
  gbserial_send(POKEMON_COUNT);

  // party pokemon species - 6 bytes + 1 byte
  for (int i = 0; i < POKEMON_COUNT; i++)
  {
    gbserial_send(mew.species); // all mews
  }
  gbserial_send(0xff);                                // end
  pad_with_zeroes(MAX_POKEMON_COUNT - POKEMON_COUNT); // pad the rest

  // trainer id - 2 bytes
  send_bytes((uint8_t *)&trainer_id, 2);

  // pokemons structs - 6 * 48 = 288 bytes
  for (int i = 0; i < POKEMON_COUNT; i++)
  {
    send_bytes((uint8_t *)&mew, POKEMON_PAYLOAD_SIZE);
  }
  pad_with_zeroes((MAX_POKEMON_COUNT - POKEMON_COUNT) * POKEMON_PAYLOAD_SIZE); // pad the rest

  // original trainer names - 36 bytes
  for (int i = 0; i < POKEMON_COUNT; i++)
  {
    send_bytes(original_trainer_name, NAME_LENGTH);
  }
  pad_with_zeroes((MAX_POKEMON_COUNT - POKEMON_COUNT) * NAME_LENGTH);

  // pokemon names - 36 bytes
  for (int i = 0; i < POKEMON_COUNT; i++)
  {
    send_bytes(pokemon_name, NAME_LENGTH); // check
  }
  pad_with_zeroes((MAX_POKEMON_COUNT - POKEMON_COUNT) * NAME_LENGTH); // check

  // unkown region of payload
  pad_with_zeroes(7);

  repeat_values(0xfd, 6);

  pad_with_zeroes(7);

  // end transfer 0xff
  repeat_values(0xff, 2);

  // unkown region of payload
  pad_with_zeroes(446);

  gbserial_send(0xff);

  pad_with_zeroes(30);

  // payload ended

  state = TRADE_SELECTION;
}

void trade_selection()
{
  uint8_t input;

  // 0x00
  gbserial_receive(&input);

  // first real byte - selected pokemon or trade canceled
  gbserial_receive(&input);
  if (input >= 0x70 && input <= 0x75) // any available pokemons selected
  {
    repeat_values(0x70, 13); // first pokemon selected
    state = TRADE_CONFIRMATION;
  }
  else if (input == POKEMON_TRADE_CANCELED)
  {
    repeat_values(POKEMON_TRADE_CANCELED, 13); // confirm cancelation
    state = IN_TRADE_CENTER;
  }
  else
  {
    // unexpected state
  }

  pad_with_zeroes(9);
}

void trade_confirmation()
{
  uint8_t input;

  // 0x00
  gbserial_receive(&input);

  // first real byte - confirmed or canceled
  gbserial_receive(&input);
  if (input == POKEMON_TRADE_ACCEPT) // trade has been accepted
  {
    repeat_values(POKEMON_TRADE_ACCEPT, 13); // accept trade
    state = TRADE_FINALIZATION;
  }
  else if (input == 0x71) // trade has been declined
  {
    repeat_values(0x71, 13); // also decline trade (value probably doesn't matter here)
    state = TRADE_SELECTION;
  }
  else
  {
    // unexpected state
  }

  pad_with_zeroes(9);
}

void trade_finalization()
{
  uint8_t input;

  // 0x00
  gbserial_receive(&input);

  gbserial_receive(&input);
  if (input == 0x71) // trade finalized (animation finished)
  {
    repeat_values(0x71, 13); // also finalize (value probably doesn't matter here)
    state = IN_TRADE_CENTER;
  }
  else
  {
    // unexpected state
  }

  pad_with_zeroes(9);
}

int main(void)
{
  gbserial_init();

  while (1)
  {
    switch (state)
    {
    case NOT_CONNECTED:
      connect();
      break;
    case CONNECTED:
      enter_trade_center();
      break;
    case IN_TRADE_CENTER:
      trade_initiated();
      break;
    case TRADE_SELECTION:
      trade_selection();
      break;
    case TRADE_CONFIRMATION:
      trade_confirmation();
      break;
    case TRADE_FINALIZATION:
      trade_finalization();
      break;
    default:
      break;
    }
  }

  return 0;
}
