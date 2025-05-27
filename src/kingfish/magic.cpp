#include "magic.h"
#include <random>
#include <chrono>

namespace Magic {
    // Magic numbers for bishops
    const Bitboard bishop_magics[64] = {
        0x40040844404084ULL, 0x2004208a004208ULL, 0x10190041080202ULL, 0x108060845042010ULL,
        0x581104180800210ULL, 0x2112080446200010ULL, 0x1080820820060210ULL, 0x3c0808410220200ULL,
        0x4050404440404ULL, 0x21001420088ULL, 0x24d0080801082102ULL, 0x1020a0a020400ULL,
        0x40308200402ULL, 0x4011002100800ULL, 0x401484104104005ULL, 0x801010402020200ULL,
        0x400210c3880100ULL, 0x404022024108200ULL, 0x810018200204102ULL, 0x4002801a02003ULL,
        0x85040820080400ULL, 0x810102c808880400ULL, 0xe900410884800ULL, 0x8002020480840102ULL,
        0x220200865090201ULL, 0x2010100a02021202ULL, 0x152048408022401ULL, 0x20080002081110ULL,
        0x4001001021004000ULL, 0x800040400a011002ULL, 0xe4004081011002ULL, 0x1c004001012080ULL,
        0x8004200962a00220ULL, 0x8422100208500202ULL, 0x2000402200300c08ULL, 0x8646020080080080ULL,
        0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL, 0x42008c0340209202ULL,
        0x209188240001000ULL, 0x400408a884001800ULL, 0x110400a6080400ULL, 0x1840060a44020800ULL,
        0x90080104000041ULL, 0x201011000808101ULL, 0x1a2208080504f080ULL, 0x8012020600211212ULL,
        0x500861011240000ULL, 0x180806108200800ULL, 0x4000020e01040044ULL, 0x300000261044000aULL,
        0x802241102020002ULL, 0x20906061210001ULL, 0x5a84841004010310ULL, 0x4010801011c04ULL,
        0xa010109502200ULL, 0x4a02012000ULL, 0x500201010098b028ULL, 0x8040002811040900ULL,
        0x28000010020204ULL, 0x6000020202d0240ULL, 0x8918844842082200ULL, 0x4010011029020020ULL
    };

    // Magic numbers for rooks
    const Bitboard rook_magics[64] = {
        0x8a80104000800020ULL, 0x140002000100040ULL, 0x2801880a0017001ULL, 0x100001001000420ULL,
        0x40002080000080ULL, 0x2260080010001ULL, 0x800100020008080ULL, 0x200020008080040ULL,
        0x8000400080020ULL, 0x400020008080040ULL, 0x100010008080020ULL, 0x800100008080020ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL,
        0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL, 0x80008000800080ULL
    };

    // Number of relevant bits for each square
    const int bishop_relevant_bits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
    };

    const int rook_relevant_bits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
    };

    // Precomputed attack tables
    Bitboard bishop_attacks[64][512];
    Bitboard rook_attacks[64][4096];

    // Bitboard masks for sliding pieces
    Bitboard bishop_masks[64];
    Bitboard rook_masks[64];

    // Generate random number
    Bitboard random_uint64() {
        static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        return rng();
    }

    // Generate magic number
    Bitboard find_magic_number(Square square, int relevant_bits, bool is_bishop) {
        Bitboard occupancies[4096];
        Bitboard attacks[4096];
        Bitboard used_attacks[4096];
        Bitboard attack_mask = is_bishop ? BBS::maskBishopAttacks(square) : BBS::maskRookAttacks(square);
        
        int occupancy_indicies = 1 << relevant_bits;
        
        for (int index = 0; index < occupancy_indicies; index++) {
            occupancies[index] = BBS::set_occupancy(index, relevant_bits, attack_mask);
            attacks[index] = is_bishop ? BBS::bishopAttacks(square, occupancies[index]) 
                                     : BBS::rookAttacks(square, occupancies[index]);
        }
        
        for (int random_count = 0; random_count < 100000000; random_count++) {
            Bitboard magic_number = random_uint64() & random_uint64() & random_uint64();
            
            if (count_bits((attack_mask * magic_number) & 0xFF00000000000000ULL) < 6)
                continue;
                
            memset(used_attacks, 0ULL, sizeof(used_attacks));
            
            int index, fail;
            for (index = 0, fail = 0; !fail && index < occupancy_indicies; index++) {
                int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));
                
                if (used_attacks[magic_index] == 0ULL)
                    used_attacks[magic_index] = attacks[index];
                else if (used_attacks[magic_index] != attacks[index])
                    fail = 1;
            }
            
            if (!fail)
                return magic_number;
        }
        
        return 0ULL;
    }

    // Initialize attack masks
    void init_attack_masks() {
        for (Square square = SQ_A8; square < SQ_COUNT; square++) {
            bishop_masks[square] = BBS::maskBishopAttacks(square);
            rook_masks[square] = BBS::maskRookAttacks(square);
        }
    }

    // Initialize attack tables
    void init_attack_tables() {
        for (Square square = SQ_A8; square < SQ_COUNT; square++) {
            // Initialize bishop attacks
            Bitboard attack_mask = bishop_masks[square];
            int relevant_bits = bishop_relevant_bits[square];
            int occupancy_indicies = 1 << relevant_bits;
            
            for (int index = 0; index < occupancy_indicies; index++) {
                Bitboard occupancy = BBS::set_occupancy(index, relevant_bits, attack_mask);
                int magic_index = (int)((occupancy * bishop_magics[square]) >> (64 - relevant_bits));
                bishop_attacks[square][magic_index] = BBS::bishopAttacks(square, occupancy);
            }
            
            // Initialize rook attacks
            attack_mask = rook_masks[square];
            relevant_bits = rook_relevant_bits[square];
            occupancy_indicies = 1 << relevant_bits;
            
            for (int index = 0; index < occupancy_indicies; index++) {
                Bitboard occupancy = BBS::set_occupancy(index, relevant_bits, attack_mask);
                int magic_index = (int)((occupancy * rook_magics[square]) >> (64 - relevant_bits));
                rook_attacks[square][magic_index] = BBS::rookAttacks(square, occupancy);
            }
        }
    }

    // Initialize magic bitboards
    void init_magic_bitboards() {
        init_attack_masks();
        init_attack_tables();
    }

    // Get bishop attacks using magic bitboards
    Bitboard get_bishop_attacks(Square square, Bitboard occupancy) {
        occupancy &= bishop_masks[square];
        occupancy *= bishop_magics[square];
        occupancy >>= 64 - bishop_relevant_bits[square];
        return bishop_attacks[square][occupancy];
    }

    // Get rook attacks using magic bitboards
    Bitboard get_rook_attacks(Square square, Bitboard occupancy) {
        occupancy &= rook_masks[square];
        occupancy *= rook_magics[square];
        occupancy >>= 64 - rook_relevant_bits[square];
        return rook_attacks[square][occupancy];
    }
} 