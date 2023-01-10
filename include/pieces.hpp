#ifndef PIECES_H_INCLUDED
#define PIECES_H_INCLUDED

typedef unsigned int piece;
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


piece_color getPieceColor(piece p);
piece_class getPieceClass(piece p);
piece makePiece(piece_class c, piece_color pc);

#endif // !PIECES_H_INCLUDED