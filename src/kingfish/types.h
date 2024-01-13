#ifndef KINGFISH_TYPES_H
#define KINGFISH_TYPES_H

#include <cstdint>

#define C64(n) n##ULL

//
//  Integer types
//

typedef std::int8_t   i8;
typedef std::int16_t  i16;
typedef std::int32_t  i32;
typedef std::int64_t  i64;
typedef std::uint8_t  ui8;
typedef std::uint16_t ui16;
typedef std::uint32_t ui32;
typedef std::uint64_t ui64;

//
// Chess types
//
typedef i64  PositionHash;
typedef ui64 Bitboard;

typedef i8 Square;
typedef i8 PieceType;
typedef i8 Color;
typedef i8 Side;
typedef i8 Direction;
typedef i8 BoardFile;
typedef i8 BoardRank;

//
// Enums
//

enum Colors {

    CL_WHITE,
    CL_BLACK,

    CL_COUNT = 2

};

enum Sides {

    SIDE_KING,
    SIDE_QUEEN,

    SIDE_COUNT = 2

};

enum Directions {

    DIR_NORTH     = -10,
    DIR_SOUTH     = 10,
    DIR_EAST      = -1,
    DIR_WEST      = 1,
    DIR_NORTHWEST = -9,
    DIR_NORTHEAST = -11,
    DIR_SOUTHWEST = 11,
    DIR_SOUTHEAST = 9,

    DIR_COUNT = 8

};

enum BoardFiles {

    FL_A,
    FL_B,
    FL_C,
    FL_D,
    FL_E,
    FL_F,
    FL_G,
    FL_H,

    FL_COUNT = 8

};

enum BoardRanks {

    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,

    RANK_COUNT = 8

};

enum PieceTypes {

    PT_PAWN,
    PT_KNIGHT,
    PT_BISHOP,
    PT_ROOK,
    PT_QUEEN,
    PT_KING,

    PT_NONE  = PT_KING + 1,
    PT_COUNT = 7

};

// clang-format off
enum Squares {
    SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
    SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
    SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
    SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
    SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
    SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
    SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
    SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,

    SQ_INVALID = SQ_H8 + 1,
    SQ_COUNT   = 64
};
// clang-format on

enum CastlingRightsMask {

    CR_NONE = 0,
    CR_WHITE_OO =
        (1 << (static_cast<int>(CL_WHITE) * 2 + static_cast<int>(SIDE_KING))),
    CR_WHITE_OOO =
        (1 << (static_cast<int>(CL_WHITE) * 2 + static_cast<int>(SIDE_QUEEN))),
    CR_BLACK_OO =
        (1 << (static_cast<int>(CL_BLACK) * 2 + static_cast<int>(SIDE_KING))),
    CR_BLACK_OOO =
        (1 << (static_cast<int>(CL_BLACK) * 2 + static_cast<int>(SIDE_QUEEN))),
    CR_ALL = CR_WHITE_OO | CR_WHITE_OOO | CR_BLACK_OO | CR_BLACK_OOO

};

inline constexpr Color getOppositeColor(Color c) {
    return c ^ 1;
}

inline char getRankIdentifier(BoardRank r) {
    if (r >= RANK_COUNT || r < RANK_1) {
        return '?';
    }
    constexpr char IDS[]{'1', '2', '3', '4', '5', '6', '7', '8'};
    return IDS[r];
}

inline constexpr BoardFile getFile(Square s) {
    return static_cast<BoardFile>(s % 8);
}

inline constexpr BoardRank getRank(Square s) {
    return static_cast<BoardRank>(s / 8);
}

inline constexpr Square getSquare(BoardFile file, BoardRank rank) {
    return static_cast<Square>(static_cast<int>(rank * 8) +
                               static_cast<int>(file));
}

#endif
