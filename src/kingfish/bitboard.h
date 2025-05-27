#ifndef KINGFISH_BITBOARD_H
#define KINGFISH_BITBOARD_H

#include <iostream>

#include "types.h"

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

// Count bits in a bitboard
inline int count_bits(Bitboard bb) {
    int count = 0;
    while (bb) {
        count++;
        bb &= bb - 1;
    }
    return count;
}

// Get least significant bit index
inline int get_ls1b_index(Bitboard bb) {
    if (bb) {
        Bitboard lsb = bb & -bb;
        int index = 0;
        while (lsb) {
            lsb >>= 1;
            index++;
        }
        return index - 1;
    }
    return -1;
}

// Set occupancy bits
inline Bitboard set_occupancy(int index, int bits_in_mask, Bitboard attack_mask) {
    Bitboard occupancy = 0ULL;
    
    for (int count = 0; count < bits_in_mask; count++) {
        int square = get_ls1b_index(attack_mask);
        pop_bit(attack_mask, square);
        
        if (index & (1 << count))
            occupancy |= (1ULL << square);
    }
    
    return occupancy;
}

Bitboard maskPawnAttacks(Color side, Square square);
Bitboard maskKnightAttacks(Square square);
Bitboard maskKingAttacks(Square square);
Bitboard maskBishopAttacks(Square square);
Bitboard maskRookAttacks(Square square);

Bitboard pawnAttacks(Color side, Square square);
Bitboard knightAttacks(Square square);
Bitboard kingAttacks(Square square);

Bitboard bishopAttacks(Square square, Bitboard block);
Bitboard rookAttacks(Square square, Bitboard block);
Bitboard queenAttacks(Square square, Bitboard block);

void initLeaperAttacks();

} // namespace BBS

#endif
