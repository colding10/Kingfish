#include "pieces.h"


piece_color getPieceColor(piece p) {
    int color = p >> 3;

    if (color == 1) {
        return WHITE;
    } else if (color == 2) {
        return BLACK;
    } else {
        return NOCOLOR;
    }
}

piece_class getPieceClass(piece p) {
    int pc = p & 7;

    return static_cast<Pieces>(pc);
}

piece makePiece(piece_class c, piece_color pc) {
    return c | pc;
}