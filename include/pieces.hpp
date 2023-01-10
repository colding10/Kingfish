#ifndef PIECES_H_INCLUDED
#define PIECES_H_INCLUDED

typedef unsigned int piece_type;
typedef unsigned int piece_color;
typedef unsigned int piece_class;

enum PieceColor {
    WHITE = 16,
    BLACK = 8,
    NOCOLOR = 0
};

enum Pieces {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};


piece_color getPieceColor(piece_type p);
piece_class getPieceClass(piece_type p);

#endif // !PIECES_H_INCLUDED