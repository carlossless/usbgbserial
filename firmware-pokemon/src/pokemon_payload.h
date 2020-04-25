#pragma once

#include <stdint.h>
#include <assert.h>

#define STR1(x) #x
#define STR(x) STR1(x)

// payload format is described here: https://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_structure_in_Generation_II
// struct packing and scalar storage order is important because that's the order that serial data gets transmitted.

#define POKEMON_PAYLOAD_SIZE 48

typedef struct __attribute__((packed, scalar_storage_order("big-endian")))
{
    uint8_t species;
    uint8_t held_item;
    uint8_t moves[4];
    uint8_t trainer_id[2];
    uint32_t experience : 24;
    uint16_t hp_ev;
    uint16_t attack_ev;
    uint16_t defense_ev;
    uint16_t speed_ev;
    uint16_t special_ev;
    uint16_t iv;
    uint8_t move_pps[4];
    uint8_t friendship;
    uint8_t pokerus;
    uint16_t caught_data;
    uint8_t level;
    uint8_t status;
    uint8_t unused;
    uint16_t hp;
    uint16_t max_hp;
    uint16_t attack;
    uint16_t defense;
    uint16_t speed;
    uint16_t special_attack;
    uint16_t special_defense;
} pokemon_payload;

_Static_assert(sizeof(pokemon_payload) == sizeof(uint8_t) * POKEMON_PAYLOAD_SIZE, "the pokemon_payload struct must be serialisable into " STR(POKEMON_PAYLOAD_SIZE) " uint8_ts");
