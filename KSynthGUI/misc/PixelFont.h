#ifndef PIXELFONT_H
#define PIXELFONT_H

#include <cstdint>

static const uint8_t LETTER_0[8] = {
	0b001100,
	0b010010,
	0b100001,
	0b100001,
	0b100001,
	0b100001,
	0b010010,
	0b001100
};

static const uint8_t LETTER_1[8] = {
	0b000100,
	0b001100,
	0b010100,
	0b000100,
	0b000100,
	0b000100,
	0b000100,
	0b011111
};

static const uint8_t LETTER_2[8] = {
	0b001110,
	0b010001,
	0b100001,
	0b000010,
	0b000100,
	0b001000,
	0b010000,
	0b111111
};

/*
0b000000,
0b000000,
0b000000,
0b000000,
0b000000,
0b000000,
0b000000,
0b000000
*/

static const uint8_t LETTER_3[8] = {
	0b011110,
	0b100001,
	0b000001,
	0b000110,
	0b000001,
	0b000001,
	0b100001,
	0b011110
};

static const uint8_t LETTER_4[8] = {
	0b000100,
	0b001000,
	0b010000,
	0b100000,
	0b100100,
	0b111111,
	0b000100,
	0b000100
};

static const uint8_t LETTER_5[8] = {
	0b111111,
	0b100000,
	0b100000,
	0b111100,
	0b000001,
	0b000001,
	0b000001,
	0b011110
};

static const uint8_t LETTER_6[8] = {
	0b000100,
	0b001000,
	0b010000,
	0b111110,
	0b100001,
	0b100001,
	0b100001,
	0b011110
};

static const uint8_t LETTER_7[8] = {
	0b111111,
	0b000001,
	0b000010,
	0b000010,
	0b000100,
	0b000100,
	0b001000,
	0b001000
};

static const uint8_t LETTER_8[8] = {
	0b011110,
	0b100001,
	0b100001,
	0b011110,
	0b100001,
	0b100001,
	0b100001,
	0b011110
};

static const uint8_t LETTER_9[8] = {
	0b011110,
	0b100001,
	0b100001,
	0b100001,
	0b011111,
	0b000001,
	0b000001,
	0b011110
};

static const uint8_t LETTER_NEG[8] = {
	0b000000,
	0b000000,
	0b000000,
	0b000000,
	0b011110,
	0b000000,
	0b000000,
	0b000000
};

#endif // PIXELFONT_H
