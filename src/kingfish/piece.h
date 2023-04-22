#ifndef KINGFISH_PIECE_H
#define KINGFISH_PIECE_H

#include <iostream>
#include <ostream>

#include "types.h"

#define WHITE_PAWN (Piece(CL_WHITE, PT_PAWN))
#define WHITE_KNIGHT (Piece(CL_WHITE, PT_KNIGHT))
#define WHITE_BISHOP (Piece(CL_WHITE, PT_BISHOP))
#define WHITE_ROOK (Piece(CL_WHITE, PT_ROOK))
#define WHITE_QUEEN (Piece(CL_WHITE, PT_QUEEN))
#define WHITE_KING (Piece(CL_WHITE, PT_KING))

#define BLACK_PAWN (Piece(CL_BLACK, PT_PAWN))
#define BLACK_KNIGHT (Piece(CL_BLACK, PT_KNIGHT))
#define BLACK_BISHOP (Piece(CL_BLACK, PT_BISHOP))
#define BLACK_ROOK (Piece(CL_BLACK, PT_ROOK))
#define BLACK_QUEEN (Piece(CL_BLACK, PT_QUEEN))
#define BLACK_KING (Piece(CL_BLACK, PT_KING))

#define PIECE_NONE (Piece(0, PT_NONE))

class Piece {
    //
    // Encoding:
    //	bit 0: Color
    //	bits 1-3: Type
    //

  public:
    inline Color getColor() const {
        return static_cast<Color>(data & ((1ul << (1)) - 1ul));
    }
    inline PieceType getType() const {
        return static_cast<PieceType>(data >> 1);
    }
    inline ui32 getRaw() const { return data; }

    char getIdentifier() const;

    inline bool operator==(Piece other) const { return data == other.data; }
    inline bool operator!=(Piece other) const { return data != other.data; }

    inline explicit constexpr Piece(ui8 raw = 0)
        : data(raw) {}
    inline constexpr Piece(Color color, PieceType type)
        : data((type << 1) | color) {}

    Piece(const Piece& rhs)            = default;
    Piece(Piece&& rhs)                 = default;
    Piece& operator=(const Piece& rhs) = default;
    ~Piece()                           = default;

    static Piece fromIdentifier(char c);

  private:
    ui8 data = 0;
};

inline std::ostream& operator<<(std::ostream& stream, Piece p) {
    stream << p.getColor() << ' ' << p.getType();
    return stream;
}

static Piece STARTING_POSITION[64] = {
    BLACK_ROOK,   BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN,  BLACK_KING,
    BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK,   BLACK_PAWN,   BLACK_PAWN,
    BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN,
    BLACK_PAWN,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,
    PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,
    PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,
    PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,
    PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,
    PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   PIECE_NONE,
    PIECE_NONE,   PIECE_NONE,   PIECE_NONE,   WHITE_PAWN,   WHITE_PAWN,
    WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,
    WHITE_PAWN,   WHITE_ROOK,   WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN,
    WHITE_KING,   WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK};

static const char PIECE_IDENTIFIER[CL_COUNT][PT_COUNT] = {
    {'.', 'P', 'N', 'B', 'R', 'Q', 'K'}, {'.', 'p', 'n', 'b', 'r', 'q', 'k'}};

#endif