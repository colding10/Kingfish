#include <iostream>
#include <string>

#include "pieces.hpp"

piece_color getPieceColor(Piece p) {
    int color = p >> 3;

    if (color == 1) {
        return WHITE;
    } else if (color == 2) {
        return BLACK;
    } else {
        return NOCOLOR;
    }
}

piece_class getPieceClass(Piece p) {
    int pc = p & 7;

    return static_cast<Pieces>(pc);
}

std::string getPieceFilename(Piece p) {
    piece_color color = getPieceColor(p);
    piece_class pc = getPieceClass(p);

    std::string filename;

    switch (color) {
        case WHITE:
            filename += "white_";
            break;
        case BLACK:
            filename += "black_";
            break;
    }

    switch (pc) {
        case PAWN:
            filename += "pawn";
            break;
        case KNIGHT:
            filename += "knight";
            break;
        case BISHOP:
            filename += "bishop";
            break;
        case ROOK:
            filename += "rook";
            break;
        case QUEEN:
            filename += "queen";
            break;
        case KING:
            filename += "king";
            break;
    }

    filename += ".bmp";

    return filename;
}

Piece makePiece(piece_class c, piece_color pc) {
    return c | pc;
}