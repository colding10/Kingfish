#ifndef PIECES_H_INCLUDED
#define PIECES_H_INCLUDED

#include <string>

typedef unsigned int Piece;
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


piece_color getPieceColor(Piece p);
piece_class getPieceClass(Piece p);
Piece makePiece(piece_class c, piece_color pc);
std::string getPieceFilename(Piece p);

#endif // !PIECES_H_INCLUDED