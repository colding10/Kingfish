#include "./position.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "consts.h"
#include "move.h"
#include "piece.h"
#include "types.h"
#include "zobrist.h"

namespace kingfish {

Piece Position::getPieceAt(Square square) {
    for (Color color = CL_WHITE; color != CL_COUNT; color++) {
        for (PieceType piece_type = PT_PAWN; piece_type != PT_COUNT;
             piece_type++) {
            if (this->pieces[color][piece_type]) {
                return Piece(color, piece_type);
            }
        }
    }
    return PIECE_NONE;
}

void Position::setPieceAt(Square square, Piece val) {
    this->pieces[val.getColor()][val.getType()].add(square);
}

void Position::clearPieceAt(Square square) {
    this->pieces[val.getColor()][val.getType()].remove(square);
}

bool Position::isCheckmate() const {
    return isCheck() && genMoves().empty();
}



} // namespace kingfish