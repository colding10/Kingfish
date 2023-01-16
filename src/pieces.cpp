#include "pieces.hpp"

#include <iostream>
#include <string>

PieceColor getPieceColor(Piece p) {
    int color = p >> 3;

    if (color == 1) {
        return BLACK;
    } else if (color == 2) {
        return WHITE;
    } else {
        return NOCOLOR;
    }
}

PieceClass getPieceClass(Piece p) {
    int pc = p & 7;

    return static_cast<Pieces>(pc);
}

std::string getPieceFilename(Piece p) {
    PieceColor color = getPieceColor(p);
    PieceClass pc = getPieceClass(p);

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

Piece makePiece(PieceClass c, PieceColor pc) {
    return c | pc;
}