#ifndef BITBOARD_HPP_INCLUDED
#define BITBOARD_HPP_INCLUDED

#include "types.hpp"

// bit manipulation macros
#define get_bit(bitboard, index) (bitboard & (1ULL << index))
#define set_bit(bitboard, index) (bitboard |= (1ULL << index))
#define pop_bit(bitboard, index) \
    (get_bit(bitboard, index) ? bitboard ^= (1ULL << index) : 0)

#endif