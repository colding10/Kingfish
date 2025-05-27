#ifndef KINGFISH_MAGIC_H
#define KINGFISH_MAGIC_H

#include "bitboard.h"
#include "types.h"

namespace Magic {
    // Magic numbers for bishops
    extern const Bitboard bishop_magics[64];
    
    // Magic numbers for rooks
    extern const Bitboard rook_magics[64];
    
    // Precomputed attack tables
    extern Bitboard bishop_attacks[64][512];
    extern Bitboard rook_attacks[64][4096];
    
    // Bitboard masks for sliding pieces
    extern Bitboard bishop_masks[64];
    extern Bitboard rook_masks[64];
    
    // Number of relevant bits for each square
    extern const int bishop_relevant_bits[64];
    extern const int rook_relevant_bits[64];
    
    // Initialize magic bitboards
    void init_magic_bitboards();
    
    // Get bishop attacks using magic bitboards
    Bitboard get_bishop_attacks(Square square, Bitboard occupancy);
    
    // Get rook attacks using magic bitboards
    Bitboard get_rook_attacks(Square square, Bitboard occupancy);
    
    // Get queen attacks using magic bitboards
    inline Bitboard get_queen_attacks(Square square, Bitboard occupancy) {
        return get_bishop_attacks(square, occupancy) | get_rook_attacks(square, occupancy);
    }
    
    // Generate magic number
    Bitboard find_magic_number(Square square, int relevant_bits, bool is_bishop);
    
    // Initialize attack masks
    void init_attack_masks();
    
    // Initialize attack tables
    void init_attack_tables();
}

#endif 