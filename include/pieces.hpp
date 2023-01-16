#ifndef PIECES_H_INCLUDED
#define PIECES_H_INCLUDED

#include <string>

typedef unsigned int Piece;
typedef unsigned int PieceColor;
typedef unsigned int PieceClass;

enum PieceColors {
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


PieceColor getPieceColor(Piece p);
PieceClass getPieceClass(Piece p);
Piece makePiece(PieceClass c, PieceColor pc);
std::string getPieceFilename(Piece p);

#endif // !PIECES_H_INCLUDED