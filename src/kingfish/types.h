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

enum Directions { // TODO: directions are now off because of 64 board

    DIR_NORTH = -10,
    DIR_SOUTH = 10,
    DIR_EAST  = -1,
    DIR_WEST  = 1,

    DIR_NORTHEAST = DIR_NORTH + DIR_EAST,
    DIR_NORTHWEST = DIR_NORTH + DIR_WEST,
    DIR_SOUTHEAST = DIR_SOUTH + DIR_EAST,
    DIR_SOUTHWEST = DIR_SOUTH + DIR_WEST,

    DIR_COUNT = 4

};

enum BoardFiles {

    FL_H,
    FL_G,
    FL_F,
    FL_E,
    FL_D,
    FL_C,
    FL_B,
    FL_A,

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

	SQ_A1 = 00,
	SQ_B1 = 01,
	SQ_C1 = 02,
	SQ_D1 = 03,
	SQ_E1 = 04,
	SQ_F1 = 05,
	SQ_G1 = 06,
	SQ_H1 = 07,

	SQ_A2 = 010,
	SQ_B2 = 011,
	SQ_C2 = 012,
	SQ_D2 = 013,
	SQ_E2 = 014,
	SQ_F2 = 015,
	SQ_G2 = 016,
	SQ_H2 = 017,

	SQ_A3 = 020,
	SQ_B3 = 021,
	SQ_C3 = 022,
	SQ_D3 = 023,
	SQ_E3 = 024,
	SQ_F3 = 025,
	SQ_G3 = 026,
	SQ_H3 = 027,

	SQ_A4 = 030,
	SQ_B4 = 031,
	SQ_C4 = 032,
	SQ_D4 = 033,
	SQ_E4 = 034,
	SQ_F4 = 035,
	SQ_G4 = 036,
	SQ_H4 = 037,

	SQ_A5 = 040,
	SQ_B5 = 041,
	SQ_C5 = 042,
	SQ_D5 = 043,
	SQ_E5 = 044,
	SQ_F5 = 045,
	SQ_G5 = 046,
	SQ_H5 = 047,

	SQ_A6 = 050,
	SQ_B6 = 051,
	SQ_C6 = 052,
	SQ_D6 = 053,
	SQ_E6 = 054,
	SQ_F6 = 055,
	SQ_G6 = 056,
	SQ_H6 = 057,

	SQ_A7 = 060,
	SQ_B7 = 061,
	SQ_C7 = 062,
	SQ_D7 = 063,
	SQ_E7 = 064,
	SQ_F7 = 065,
	SQ_G7 = 066,
	SQ_H7 = 067,

	SQ_A8 = 070,
	SQ_B8 = 071,
	SQ_C8 = 072,
	SQ_D8 = 073,
	SQ_E8 = 074,
	SQ_F8 = 075,
	SQ_G8 = 076,
	SQ_H8 = 077,

	SQ_INVALID = SQ_H8 + 1,
	SQ_COUNT = 64

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

inline constexpr bool isValidSquare(Square s) {
    return s & 0x88;
}

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
    return static_cast<BoardFile>(s & 7);
}

inline constexpr BoardRank getRank(Square s) {
    return static_cast<BoardRank>(s >> 4);
}

inline constexpr Square get0x88Square(BoardFile file, BoardRank rank) {
    return static_cast<Square>(static_cast<int>(rank * 16) +
                               static_cast<int>(file));
}

inline constexpr Square get0x88Square(Square square_8x8) {
    return square_8x8 + (square_8x8 & ~7);
}

inline constexpr Square get8x8Square(Square square_0x88) {
    return (square_0x88 + (square_0x88 & 7)) >> 1;
}

Square getSquare(std::string_view str);

#endif