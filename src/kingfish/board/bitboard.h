#ifndef BITBOARD_h_INCLUDED
#define BITBOARD_h_INCLUDED

#include "types.h"

// bit manipulation macros
#define get_bit(bitboard, index) (bitboard & (1ULL << index))
#define set_bit(bitboard, index) (bitboard |= (1ULL << index))
#define pop_bit(bitboard, index) \
    (get_bit(bitboard, index) ? bitboard ^= (1ULL << index) : 0)

#endif