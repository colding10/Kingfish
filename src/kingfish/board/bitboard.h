#ifndef KINGFISH_BITBOARD_H
#define KINGFISH_BITBOARD_H

#include <iostream>

#include "../types.h"
#include "bitboard.h"

// bit manipulation macros
#define get_bit(bitboard, index) (bitboard & (1ULL << index))
#define set_bit(bitboard, index) (bitboard |= (1ULL << index))
#define pop_bit(bitboard, index) \
    (get_bit(bitboard, index) ? bitboard ^= (1ULL << index) : 0)

namespace BBS {
void printBitboard(Bitboard bitboard);

constexpr Bitboard not_a_file  = 18374403900871474942ULL;
constexpr Bitboard not_h_file  = 9187201950435737471ULL;
constexpr Bitboard not_hg_file = 4557430888798830399ULL;
constexpr Bitboard not_ab_file = 18229723555195321596ULL;

static Bitboard pawn_attacks[2][64]; // pawn attacks array [side][square]
static Bitboard knight_attacks[64];
static Bitboard king_attacks[64];

Bitboard maskPawnAttacks(Color side, int square);
Bitboard maskKnightAttacks(Square square);
Bitboard maskKingAttacks(Square square);
Bitboard maskBishopAttacks(int square);
Bitboard maskRookAttacks(int square);

Bitboard bishopAttacks(int square, Bitboard block);
Bitboard rookAttacks(int square, Bitboard block);

void initLeaperAttacks();
} // namespace BBS

#endif