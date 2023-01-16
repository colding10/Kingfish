#include "game.hpp"

#include <iostream>

#include "board.hpp"
#include "pieces.hpp"

bool isValidMove(Board* board, Location starting, Location ending) {
    Piece starting_piece = board->getPieceAt(starting);
    Piece ending_piece = board->getPieceAt(ending);

    PieceColor starting_color = getPieceColor(starting_piece);

    if (ending_piece != 0x00 && starting_color == getPieceColor(ending_piece)) {
        return false;  // can't capture your own piece
    }

    switch (getPieceClass(starting_piece)) {
        case PAWN:
            return isValidPawnMove(board, starting, ending, starting_color);
        case KNIGHT:
            return isValidKnightMove(starting, ending);
        case BISHOP:
            return isValidBishopMove(board, starting, ending, starting_color);
        case ROOK:
            return isValidRookMove(board, starting, ending, starting_color);
        case QUEEN:
            return isValidQueenMove(board, starting, ending, starting_color);
        case KING:
            return isValidKingMove(starting, ending);
    }

    throw "bad excaped";
}

bool isValidPawnMove(Board* board, Location starting, Location ending, PieceColor starting_color) {
    Piece ending_piece = board->getPieceAt(ending);

    if (ending_piece == 0) {  // open piece
        if (starting_color == WHITE) {
            if (
                starting.first == 6 && starting.first - ending.first == 2 && starting.second == ending.second) {
                return true;
            }
            if (
                starting.first - ending.first == 1 && starting.second == ending.second) {
                return true;
            }
        } else if (starting_color == BLACK) {
            if (
                starting.first == 1 && ending.first - starting.first == 2 && starting.second == ending.second) {
                return true;
            }
            if (
                ending.first - starting.first == 1 && starting.second == ending.second) {
                return true;
            }
        }
    } else {  // piece is occupied
        if (starting_color == WHITE) {
            if (ending.first == starting.first - 1) {
                if (abs(ending.second - starting.second) == 1) {
                    return true;
                }
            }
        } else if (starting_color == BLACK) {
            if (ending.first == starting.first + 1) {
                if (abs(ending.second - starting.second) == 1) {
                    return true;
                }
            }
        }

        return false;
    }
    return false;
}

bool isValidKnightMove(Location starting, Location ending) {
    int x_diff = abs(ending.first - starting.first);
    int y_diff = abs(ending.second - starting.second);

    if (x_diff == 1 && y_diff == 2) {
        return true;
    }
    if (x_diff == 2 && y_diff == 1) {
        return true;
    }

    return false;
}

bool isValidBishopMove(Board* board, Location starting, Location ending, PieceColor starting_color) {
    if (starting.first + starting.second != ending.first + ending.second && starting.first - starting.second != ending.first - ending.second) {
        return false;
    }

    int x_change = ending.second < starting.second ? -1 : 1;
    int y_change = ending.first < starting.first ? -1 : 1;

    Location indices = starting;

    while (0 <= indices.first <= 7 && 0 <= indices.second <= 7 && indices != ending) {
        indices.second += x_change;
        indices.first += y_change;

        if (board->getPieceAt(indices)) {
            if (getPieceColor(board->getPieceAt(indices)) == starting_color) {
                indices.second -= x_change;
                indices.first -= y_change;
            }
            break;
        }
    }

    if (indices == ending) {
        return true;
    }

    return false;
}

bool isValidRookMove(Board* board, Location starting, Location ending, PieceColor starting_color) {
    if (
        starting.first != ending.first && starting.second != ending.second) {
        return false;
    }

    int x_change, y_change;

    x_change = ending.second < starting.second ? -1 : 1;
    y_change = ending.first < starting.first ? -1 : 1;

    x_change = ending.second == starting.second ? 0 : x_change;
    y_change = ending.first == starting.first ? 0 : y_change;

    Location indices = (starting);

    while (0 <= indices.first <= 7 && 0 <= indices.second <= 7 && indices != ending) {
        indices.second += x_change;
        indices.first += y_change;

        if (board->getPieceAt(indices)) {
            if (getPieceColor(board->getPieceAt(indices)) == starting_color) {
                indices.second -= x_change;
                indices.first -= y_change;
            }

            break;
        }
    }
    if (indices == ending) {
        return true;
    }

    return false;
}

bool isValidQueenMove(Board* board, Location starting, Location ending, PieceColor starting_color) {
    return isValidBishopMove(
               board, starting, ending, starting_color) ||
           isValidRookMove(board, starting, ending, starting_color);
}

bool isValidKingMove(Location starting, Location ending) {
    int x_diff = abs(starting.first - ending.first);
    int y_diff = abs(starting.second - ending.second);

    if (x_diff == 1 and y_diff == 1) {
        return true;
    }
    if (x_diff == 1 and y_diff == 0) {
        return true;
    }
    if (x_diff == 0 and y_diff == 1) {
        return true;
    }

    return false;
}