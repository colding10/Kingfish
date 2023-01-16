#include "game.hpp"

#include <iostream>

#include "board.hpp"
#include "pieces.hpp"

bool isValidMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending) {
    Piece starting_piece = board->getPieceAt(starting);
    Piece ending_piece = board->getPieceAt(ending);

    if (ending_piece != 0x00 && getPieceColor(starting_piece) == getPieceColor(ending_piece)) {
        return false;  // can't capture your own piece
    }

    switch (starting_piece) {
        case PAWN:
            return isValidPawnMove(board, starting, ending);
        case KNIGHT:
            return isValidKnightMove(board, starting, ending);
        case BISHOP:
            return isValidBishopMove(board, starting, ending);
        case ROOK:
            return isValidRookMove(board, starting, ending);
        case QUEEN:
            return isValidQueenMove(board, starting, ending);
        case KING:
            return isValidKingMove(board, starting, ending);
    }
}

bool isValidPawnMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending) {
}
bool isValidKnightMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending) {
}
bool isValidBishopMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending) {
}
bool isValidRookMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending) {
}
bool isValidQueenMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending) {
}
bool isValidKingMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending) {
}