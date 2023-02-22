#include "game.hpp"

#include <iostream>
#include <map>

#include "board.hpp"
#include "location.hpp"
#include "pieces.hpp"

bool Game::isValidMove(Board *board, Location starting, Location ending,
                       bool check_king) {
    Piece starting_piece = board->getPieceAt(starting);
    Piece ending_piece = board->getPieceAt(ending);

    PieceColor starting_color = Pieces::getPieceColor(starting_piece);

    if (ending_piece != 0x00 &&
        starting_color == Pieces::getPieceColor(ending_piece)) {
        return false;  // can't capture your own piece
    }

    bool result = true;
    switch (Pieces::getPieceClass(starting_piece)) {
        case PAWN:
            result = isValidPawnMove(board, starting, ending, starting_color);
            break;
        case KNIGHT:
            result = isValidKnightMove(starting, ending);
            break;
        case BISHOP:
            result = isValidBishopMove(board, starting, ending, starting_color);
            break;
        case ROOK:
            result = isValidRookMove(board, starting, ending, starting_color);
            break;
        case QUEEN:
            result = isValidQueenMove(board, starting, ending, starting_color);
            break;
        case KING:
            result = isValidKingMove(board, starting, ending);
            break;
    }

    // TODO: remove this later
    // return result;
    
    if (!check_king) {
        return result;
    } else if (result == false) {
        return result;
    }

    board->makeMove(Move(starting, ending, board->getPieceAt(ending), 0), true);

    if (isInCheck(board, starting_color)) {
        board->undoLastMove();
        return false;
    }

    board->undoLastMove();
    return true;
}

bool Game::isValidPawnMove(Board *board, Location starting, Location ending,
                           PieceColor starting_color) {
    Piece ending_piece = board->getPieceAt(ending);

    PieceColor opposite_color = starting_color == WHITE ? BLACK : WHITE;

    if (!board->isReversed()) {
        opposite_color = starting_color;
    }
    if (ending_piece == 0) {  // open piece
        if (opposite_color == WHITE) {
            if (starting.X == 6 && starting.X - ending.X == 2 &&
                starting.Y == ending.Y && !board->getPieceAt({5, starting.Y})) {
                return true;
            }
            if (starting.X - ending.X == 1 && starting.Y == ending.Y) {
                return true;
            }
        } else if (opposite_color == BLACK) {
            if (starting.X == 1 && ending.X - starting.X == 2 &&
                starting.Y == ending.Y && !board->getPieceAt({2, starting.Y})) {
                return true;
            }
            if (ending.X - starting.X == 1 && starting.Y == ending.Y) {
                return true;
            }
        }
    } else {  // piece is occupied
        if (starting_color == WHITE) {
            if (ending.X == starting.X - 1) {
                if (abs(ending.Y - starting.Y) == 1) {
                    return true;
                }
            }
        } else if (starting_color == BLACK) {
            if (ending.X == starting.X + 1) {
                if (abs(ending.Y - starting.Y) == 1) {
                    return true;
                }
            }
        }

        return false;
    }
    return false;
}

bool Game::isValidKnightMove(Location starting, Location ending) {
    int x_diff = abs(ending.X - starting.X);
    int y_diff = abs(ending.Y - starting.Y);

    if (x_diff == 1 && y_diff == 2) {
        return true;
    }
    if (x_diff == 2 && y_diff == 1) {
        return true;
    }

    return false;
}

bool Game::isValidBishopMove(Board *board, Location starting, Location ending,
                             PieceColor starting_color) {
    if (starting.X + starting.Y != ending.X + ending.Y &&
        starting.X - starting.Y != ending.X - ending.Y) {
        return false;
    }

    int x_change = ending.Y < starting.Y ? -1 : 1;
    int y_change = ending.X < starting.X ? -1 : 1;

    Location indices = starting;

    while (0 <= indices.X && indices.X <= 7 && 0 <= indices.Y && indices.Y <= 7 &&
           indices != ending) {
        indices.Y += x_change;
        indices.X += y_change;

        if (board->getPieceAt(indices)) {
            if (Pieces::getPieceColor(board->getPieceAt(indices)) == starting_color) {
                indices.Y -= x_change;
                indices.X -= y_change;
            }
            break;
        }
    }

    if (indices == ending) {
        return true;
    }

    return false;
}

bool Game::isValidRookMove(Board *board, Location starting, Location ending,
                           PieceColor starting_color) {
    if (starting.X != ending.X && starting.Y != ending.Y) {
        return false;
    }

    int x_change, y_change;

    x_change = ending.Y < starting.Y ? -1 : 1;
    y_change = ending.X < starting.X ? -1 : 1;

    x_change = ending.Y == starting.Y ? 0 : x_change;
    y_change = ending.X == starting.X ? 0 : y_change;

    Location indices = (starting);

    while (0 <= indices.X && indices.X <= 7 && 0 <= indices.Y && indices.Y <= 7 &&
           indices != ending) {
        indices.Y += x_change;
        indices.X += y_change;

        if (board->getPieceAt(indices)) {
            if (Pieces::getPieceColor(board->getPieceAt(indices)) == starting_color) {
                indices.Y -= x_change;
                indices.X -= y_change;
            }

            break;
        }
    }
    if (indices == ending) {
        return true;
    }

    return false;
}

bool Game::isValidQueenMove(Board *board, Location starting, Location ending,
                            PieceColor starting_color) {
    return isValidBishopMove(board, starting, ending, starting_color) ||
           isValidRookMove(board, starting, ending, starting_color);
}

bool Game::isValidKingMove(Board *board, Location starting, Location ending) {
    int x_diff = abs(starting.X - ending.X);
    int y_diff = abs(starting.Y - ending.Y);

    if (x_diff == 1 and y_diff == 1) {
        return true;
    }
    if (x_diff == 1 and y_diff == 0) {
        return true;
    }
    if (x_diff == 0 and y_diff == 1) {
        return true;
    }

    if (starting.X == 0 && starting.Y == 4) {
        if (ending.X == 0 && ending.Y == 6 && !board->getPieceAt({0, 5}) &&
            Pieces::getPieceClass(board->getPieceAt({0, 7})) == ROOK) {
            return true;
        }
        if (ending.X == 0 && ending.Y == 2 && !board->getPieceAt({0, 3}) &&
            Pieces::getPieceClass(board->getPieceAt({0, 0})) == ROOK) {
            return true;
        }
    }
    if (starting.X == 7 && starting.Y == 4) {
        if (ending.X == 7 && ending.Y == 6 && !board->getPieceAt({7, 5}) &&
            Pieces::getPieceClass(board->getPieceAt({7, 7})) == ROOK) {
            return true;
        }
        if (ending.X == 7 && ending.Y == 2 && !board->getPieceAt({7, 3}) &&
            Pieces::getPieceClass(board->getPieceAt({7, 0})) == ROOK) {
            return true;
        }
    }

    return false;
}

bool Game::isInCheck(Board *board, PieceColor color) {
    Piece piece;

    Location king_location = Location(-1, -1);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            piece = board->getPieceAt(Location(i, j));
            if (Pieces::getPieceClass(piece) == KING &&
                Pieces::getPieceColor(piece) == color) {
                king_location = Location(i, j);
                break;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            piece = board->getPieceAt(Location(i, j));
            if (piece == 0x00) {
                continue;
            }

            if (Pieces::getPieceColor(piece) != color) {
                if (Game::isValidMove(board, Location(i, j), king_location, false)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Game::isInCheckMate(Board *board, PieceColor color) {
    Location king_location = Location(-1, -1);
    Piece piece;

    if (!isInCheck(board, color)) {
        return false;  // can't be in mate if ur not checked
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            piece = board->getPieceAt(Location(i, j));
            if (Pieces::getPieceClass(piece) == KING &&
                Pieces::getPieceColor(piece) == color) {
                king_location = Location(i, j);
                break;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            piece = board->getPieceAt(Location(i, j));
            if (piece == 0x00) {
                continue;
            }

            if (Pieces::getPieceColor(piece) == color) {
                Location starting_location = Location(i, j);

                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        Location ending_location = Location(x, y);

                        if (Game::isValidMove(board, starting_location, ending_location,
                                              true)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

float Game::getPieceValue(PieceClass p) {
    std::map<Piece, float> piece_values = {{PAWN, 100}, {KNIGHT, 320}, {BISHOP, 330}, {ROOK, 500}, {QUEEN, 15000}, {KING, 0}};

    return piece_values[Pieces::getPieceClass(p)];
}