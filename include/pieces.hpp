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

enum PieceEnum {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

namespace Pieces {
    PieceColor getPieceColor(Piece p);
    PieceClass getPieceClass(Piece p);
    Piece makePiece(PieceClass c, PieceColor pc);
    std::string getPieceFilename(Piece p);
}  // namespace Pieces

#endif  // !PIECES_H_INCLUDED