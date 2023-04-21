#include "bitboard.h"

#include <iostream>

namespace BBS {
void printBitboard(Bitboard bitboard) {
    std::cout << std::endl;

    for (BoardRank rank = 0; rank < 8; rank++) {
        std::cout << "  " << 8 - rank;
        for (BoardFile file = 0; file < 8; file++) {
            Square square = rank * 8 + file;
            std::cout << " " << (get_bit(bitboard, square) ? 1 : 0);
        }
        std::cout << std::endl;
    }

    std::cout << "    a b c d e f g h\n\n";
    std::cout << "     bitboard:  " << bitboard << "\n\n";
}

// mask pawn attacks
Bitboard maskPawnAttacks(Color side, Square square) {
    Bitboard attacks  = 0;
    Bitboard bitboard = 0;

    set_bit(bitboard, square);

    // white pawn attacks
    if (!side) {
        // make sure attack is on board
        if ((bitboard >> 7) & not_a_file)
            attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & not_h_file)
            attacks |= (bitboard >> 9);
    }

    // black pawn atacks
    else {
        // make sure attack is on board
        if ((bitboard << 7) & not_h_file)
            attacks |= (bitboard << 7);
        if ((bitboard << 9) & not_a_file)
            attacks |= (bitboard << 9);
    }

    // return attack map for pawn on a given square
    return attacks;
}

Bitboard maskKnightAttacks(Square square) {
    // attack bitboard
    Bitboard attacks = 0;

    // piece bitboard
    Bitboard bitboard = 0;

    // set piece on bitboard
    set_bit(bitboard, square);

    // generate knight
    if ((bitboard >> 17) & not_h_file)
        attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file)
        attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file)
        attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file)
        attacks |= (bitboard >> 6);
    if ((bitboard << 17) & not_a_file)
        attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file)
        attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file)
        attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file)
        attacks |= (bitboard << 6);

    // return attack map for knight on a given square
    return attacks;
}

Bitboard maskKingAttacks(Square square) {
    // attack bitboard
    Bitboard attacks = 0;

    // piece bitboard
    Bitboard bitboard = 0;

    // set piece on bitboard
    set_bit(bitboard, square);

    // generate king attacks
    if (bitboard >> 8)
        attacks |= (bitboard >> 8);
    if (bitboard << 8)
        attacks |= (bitboard << 8);
    if ((bitboard >> 1) & not_h_file)
        attacks |= (bitboard >> 1);
    if ((bitboard >> 9) & not_h_file)
        attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file)
        attacks |= (bitboard >> 7);
    if ((bitboard << 1) & not_a_file)
        attacks |= (bitboard << 1);
    if ((bitboard << 9) & not_a_file)
        attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file)
        attacks |= (bitboard << 7);

    // return attack map for king on a given square
    return attacks;
}

// mask bishop attacks
Bitboard maskBishopAttacks(Square square) {
    // attack bitboard
    Bitboard attacks = 0;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;

    // generate attacks
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
        attacks |= (1ULL << (r * 8 + f));

    // return attack map for bishop on a given square
    return attacks;
}

// mask rook attacks
Bitboard maskRookAttacks(Square square) {
    // attacks bitboard
    Bitboard attacks = 0ULL;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;

    // generate attacks
    for (r = tr + 1; r <= 6; r++)
        attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--)
        attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++)
        attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--)
        attacks |= (1ULL << (tr * 8 + f));

    // return attack map for bishop on a given square
    return attacks;
}

// bishop attacks
Bitboard bishopAttacks(Square square, Bitboard block) {
    // attack bitboard
    Bitboard attacks = 0;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;

    // generate attacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f)))
            break;
    }

    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f)))
            break;
    }

    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f)))
            break;
    }

    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f)))
            break;
    }

    // return attack map for bishop on a given square
    return attacks;
}

// rook attacks
Bitboard rookAttacks(Square square, Bitboard block) {
    // attacks bitboard
    Bitboard attacks = 0ULL;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;

    // generate attacks
    for (r = tr + 1; r <= 7; r++) {
        attacks |= (1ULL << (r * 8 + tf));
        if (block & (1ULL << (r * 8 + tf)))
            break;
    }

    for (r = tr - 1; r >= 0; r--) {
        attacks |= (1ULL << (r * 8 + tf));
        if (block & (1ULL << (r * 8 + tf)))
            break;
    }

    for (f = tf + 1; f <= 7; f++) {
        attacks |= (1ULL << (tr * 8 + f));
        if (block & (1ULL << (tr * 8 + f)))
            break;
    }

    for (f = tf - 1; f >= 0; f--) {
        attacks |= (1ULL << (tr * 8 + f));
        if (block & (1ULL << (tr * 8 + f)))
            break;
    }

    // return attack map for bishop on a given square
    return attacks;
}

// init pre-calculated attack tables for leaper pieces (pawns, knights, kings)
void initLeaperAttacks() {
    // loop over 64 board squares
    for (Square square = 0; square < 64; square++) {
        // init leaper attacks
        pawn_attacks[CL_WHITE][square] = maskPawnAttacks(CL_WHITE, square);
        pawn_attacks[CL_BLACK][square] = maskPawnAttacks(CL_BLACK, square);
        knight_attacks[square]         = maskKnightAttacks(square);
        king_attacks[square]           = maskKingAttacks(square);
    }
}
} // namespace BBS