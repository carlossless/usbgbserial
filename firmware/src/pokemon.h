#ifndef POKEMON_H
#define POKEMON_H

#include <stdint.h>

// Explanation: https://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_structure_in_Generation_II

uint8_t pokemon[48] = {
    0x97,             // Species - Mew
    0x00,             // Held Item - Nothing
    0x25,             // Move 1 - Mega Kick
    0x37,             // Move 2 - Thrash
    0x38,             // Move 3 - Double Edge
    0x26,             // Move 4 - Jump Kick
    0xde, 0xad,       // Trainer ID - 0xdead
    0x10, 0x2c, 0x14, // Experience - 1,059,860
    0x00, 0x00,       // HP EV
    0x00, 0x00,       // Attack EV
    0x00, 0x00,       // Defense EV
    0x00, 0x00,       // Speed EV
    0x00, 0x00,       // Special EV
    0x00, 0x00,       // IV
    0x14,             // Move 1 PP
    0x19,             // Move 2 PP
    0x05,             // Move 3 PP
    0x0f,             // Move 4 PP
    0x00,             // Friendship
    0x00,             // Pokerus
    0x00, 0x00,       // Caught Data
    0x64,             // Level - 100
    0x00,             // Status - Normal
    0x00,             // Unused
    0x01, 0x00,       // Current HP - 256
    0x01, 0x00,       // Max HP - 256
    0x01, 0x00,       // Attack - 256
    0x01, 0x00,       // Defense - 256
    0x01, 0x00,       // Speed - 256
    0x01, 0x00,       // Special Attack - 256
    0x01, 0x00        // Special Defense - 256
};

// TEMP: first 96 bytes of trade init
// 0,0.19987,00
// 1,0.21665,75
// 2,0.23332,75
// 3,0.25011,75
// 4,0.28356,75
// 5,0.30029,75
// 6,0.31707,75
// 7,0.33386,75
// 8,0.35053,75
// 9,0.36902,75
// 10,0.38398,75
// 11,0.40071,75
// 12,0.41749,75
// 13,0.43428,75
// 14,0.45101,75
// 15,0.46761,00
// 16,0.48434,00
// 17,0.50119,00
// 18,0.51785,00
// 19,0.53464,00
// 20,0.55143,00
// 21,0.56821,00
// 22,0.58488,00
// 23,0.60167,00
// 24,1.27542,00
// 25,1.28836,76
// 26,1.30509,76
// 27,1.32413,76
// 28,1.35521,76
// 29,1.37193,76
// 30,1.38872,76
// 31,1.40545,76
// 32,1.42217,76
// 33,1.43896,76
// 34,1.45575,76
// 35,1.47247,76
// 36,1.4892,76
// 37,1.50599,76
// 38,1.52259,76
// 39,1.53932,00
// 40,1.5561,00
// 41,1.57399,00
// 42,1.58956,00
// 43,1.60641,00
// 44,1.62307,00
// 45,1.6398,00
// 46,1.65659,00
// 47,1.67343,00
// 48,1.72465,00
// 49,1.74132,00
// 50,1.79058,00
// 51,1.79192,FD
// 52,1.79326,FD
// 53,1.79461,FD
// 54,1.79595,FD
// 55,1.79729,FD
// 56,1.79864,FD
// 57,1.79998,FD
// 58,1.80132,FD
// 59,1.80267,FD
// 60,1.80401,FD
// 61,1.80535,BB
// 62,1.80669,75
// 63,1.80804,2E
// 64,1.80938,E5
// 65,1.81072,9C
// 66,1.81207,52
// 67,1.81341,06
// 68,1.81475,BA
// 69,1.8161,6D
// 70,1.81744,1E
// 71,1.81872,FD
// 72,1.82006,FD
// 73,1.82141,FD
// 74,1.82275,FD
// 75,1.82409,FD
// 76,1.82544,FD
// 77,1.82678,FD
// 78,1.82812,FD
// 79,1.82946,FD
// 80,1.83081,80
// 81,1.83215,80
// 82,1.83349,80
// 83,1.83484,80
// 84,1.83618,80
// 85,1.83752,50
// 86,1.83887,03 // pokemon count
// 87,1.84021,9B // first pokemon - Cyndaquil
// 88,1.84155,A1 // second pokemon - Sentret
// 89,1.84289,10 // third pokemon - Pidgey
// 90,1.84424,FF
// 91,1.84558,00
// 92,1.84692,00
// 93,1.84827,00
// 94,1.84961,9F
// 95,1.85095,AC

#endif
