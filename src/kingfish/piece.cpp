#include "piece.h"



char Piece::getIdentifier() const {
    return PIECE_IDENTIFIER[getColor()][getType()];
}

Piece Piece::fromIdentifier(char ident) {
    switch (ident) {
        case 'P': return WHITE_PAWN;
        case 'N': return WHITE_KNIGHT;
        case 'B': return WHITE_BISHOP;
        case 'R': return WHITE_ROOK;
        case 'Q': return WHITE_QUEEN;
        case 'K': return WHITE_KING;

        case 'p': return BLACK_PAWN;
        case 'n': return BLACK_KNIGHT;
        case 'b': return BLACK_BISHOP;
        case 'r': return BLACK_ROOK;
        case 'q': return BLACK_QUEEN;
        case 'k': return BLACK_KING;

        default: return PIECE_NONE;
    }
}
