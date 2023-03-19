#include "board.hpp"

#include <array>
#include <cassert>
#include <cctype>
#include <climits>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "bonustables.hpp"
#include "game.hpp"
#include "location.hpp"
#include "move.hpp"
#include "pieces.hpp"

// TODO: add other FEN fields
void Board::readFen(std::string fen) {
    int row = 0;
    int col = 0;

    // std::string token;
    // std::stringstream fen(fen);

    // while (std::getline(fen, token, ' ')) {

    // }

    for (char c : fen) {
        if (c == '/') {
            row++;
            col = 0;
            continue;
        } else {
            if (isdigit(c)) {
                for (int _ = 0; _ < static_cast<int>(c - '0'); _++) {
                    this->board[row][col] = 0x00;
                    col++;
                }
            } else {
                PieceColor color = isupper(c) ? WHITE : BLACK;
                Piece p;

                c = tolower(c);

                switch (c) {
                    case 'p':
                        p = Pieces::makePiece(PAWN, color);
                        break;
                    case 'r':
                        p = Pieces::makePiece(ROOK, color);
                        break;
                    case 'n':
                        p = Pieces::makePiece(KNIGHT, color);
                        break;
                    case 'b':
                        p = Pieces::makePiece(BISHOP, color);
                        break;
                    case 'q':
                        p = Pieces::makePiece(QUEEN, color);
                        break;
                    case 'k':
                        p = Pieces::makePiece(KING, color);
                        break;
                    default:
                        p = Pieces::makePiece(PAWN, color);
                }

                this->board[row][col] = p;
                col++;
            }
        }
    }
}

std::string Board::getFen() {
    std::string fen = "";
    char c = ' ';
    int empty_count = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece piece = this->board[row][col];
            PieceClass piece_class = Pieces::getPieceClass(piece);

            if (piece == 0) {
                c = ' ';
            } else if (piece_class == PAWN) {
                c = 'p';
            } else if (piece_class == ROOK) {
                c = 'r';
            } else if (piece_class == KNIGHT) {
                c = 'n';
            } else if (piece_class == BISHOP) {
                c = 'b';
            } else if (piece_class == QUEEN) {
                c = 'q';
            } else if (piece_class == KING) {
                c = 'k';
            }
            if (c == ' ') {
                empty_count++;
                continue;
            } else {
                if (empty_count > 0) {
                    fen += std::to_string(empty_count);
                }
                empty_count = 0;
            }

            if (Pieces::getPieceColor(piece) == WHITE) {
                c = toupper(c);
            }
            fen += c;
        }
        if (empty_count > 0) {
            fen += std::to_string(empty_count);
        }
        empty_count = 0;
        fen += "/";
    }

    return fen;
}

PieceColor Board::getActiveColor() {
    return this->active_color;
}

float Board::getMoveNumber() {
    return this->move_number;
}

void Board::incrementMoveNumber() {
    this->move_number += 0.5f;
}

void Board::toggleActiveColor() {
    this->active_color = this->getActiveColor() == WHITE ? BLACK : WHITE;
}

void Board::setActiveColor(PieceColor color) {
    this->active_color = color;
}

void Board::reverse() {
    Piece new_board[8][8];

    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            new_board[7 - i][j] = this->board[i][j];
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->board[i][j] = new_board[i][j];
        }
    }

    this->is_reversed = !this->is_reversed;
}

bool Board::isReversed() {
    return this->is_reversed;
}

void Board::tryMove(Move move) {
    if (Game::isValidMove(this, move.getStarting(), move.getEnding(), true)) {
        if (Pieces::getPieceClass(this->getPieceAt(move.getStarting())) == KING && abs(move.getEnding().Y - move.getEnding().Y) == 2) {
            std::cout << "joe" << std::endl;
            if (move.getEnding().Y > move.getStarting().Y) {
                this->board[move.getStarting().X][7] = 0x00;
                this->board[move.getStarting().X][move.getEnding().Y - 1] = Pieces::makePiece(ROOK, this->getActiveColor());
            } else {
                this->board[move.getStarting().X][0] = 0x00;
                this->board[move.getStarting().X][move.getEnding().Y + 1] = Pieces::makePiece(ROOK, this->getActiveColor());
            }

        } else {
            this->makeMove(move, true);
        }
        this->clearSelectedPiece();
    }
}

void Board::makeMove(Move move, bool push_to_stack) {  // TODO: fix castling and promoting
    move.setCaptured(this->board[move.getEnding().X][move.getEnding().Y]);

    this->board[move.getEnding().X][move.getEnding().Y] = this->board[move.getStarting().X][move.getStarting().Y];
    this->board[move.getStarting().X][move.getStarting().Y] = 0x00;

    if (push_to_stack) {
        this->move_stack.push(move);

        this->incrementMoveNumber();
        this->toggleActiveColor();
    }

    if (Pieces::getPieceClass(this->getPieceAt({move.getEnding().X, move.getEnding().Y})) == PAWN) {
        if (move.getEnding().X == 0 && Pieces::getPieceColor(this->getPieceAt({move.getEnding().X, move.getEnding().Y})) == WHITE) {
            this->board[move.getEnding().X][move.getEnding().Y] = Pieces::makePiece(QUEEN, WHITE);
        } else if (move.getEnding().X == 7 && Pieces::getPieceColor(this->getPieceAt({move.getEnding().X, move.getEnding().Y})) == BLACK) {
            this->board[move.getEnding().X][move.getEnding().Y] = Pieces::makePiece(QUEEN, BLACK);
        }
    }
}

void Board::undoLastMove() {
    Move last_move = this->move_stack.top();

    this->move_stack.pop();

    this->board[last_move.getStarting().X][last_move.getStarting().Y] = this->board[last_move.getEnding().X][last_move.getEnding().Y];
    this->board[last_move.getEnding().X][last_move.getEnding().Y] = last_move.getCaptured();

    this->toggleActiveColor();
}

void Board::makeNullMove() {
    this->makeMove(Move(Location(0, 0), Location(0, 0), 0, -1), true);
}

Piece Board::getPieceAt(Location location) {
    return this->board[location.X][location.Y];
}

float Board::evaluateBoard(PieceColor color) {
    if (Game::isInCheckMate(this, color == WHITE ? BLACK : WHITE)) {
        return INT_MAX;
    } else if (Game::isInCheckMate(this, color)) {
        return INT_MIN;
    }

    float white = 0.0f;
    float black = 0.0f;

    float white_bonuses = 0.0f;
    float black_bonuses = 0.0f;

    int white_pieces = 0;
    int black_pieces = 0;

    float mobility = this->getLegalMoves(color).size();

    assert(mobility != 0);

    Piece p;
    for (Location l : ALL_LOCATIONS) {
        p = this->getPieceAt(l);

        if (!p) {
            continue;
        } else if (Pieces::getPieceColor(p) == WHITE) {
            int bonus = 0;
            switch (Pieces::getPieceClass(p)) {
                case PAWN:
                    bonus = Tables::pawn_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
                case KNIGHT:
                    bonus = Tables::knight_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
                case BISHOP:
                    bonus = Tables::bishop_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
                case ROOK:
                    bonus = Tables::rook_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
                case QUEEN:
                    bonus = Tables::queen_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
                case KING:
                    bonus = Tables::king_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
            }
            white += Game::getPieceValue(Pieces::getPieceClass(p));
            white_bonuses += bonus / 3;
            white_pieces++;
        } else {
            int bonus = 0;
            switch (Pieces::getPieceClass(p)) {
                case PAWN:
                    bonus = Tables::pawn_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
                case KNIGHT:
                    bonus = Tables::knight_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
                case BISHOP:
                    bonus = Tables::bishop_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
                case ROOK:
                    bonus = Tables::rook_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
                case QUEEN:
                    bonus = Tables::queen_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
                case KING:
                    bonus = Tables::king_table[color == BLACK ? l.X : 7 - l.X][l.Y];
                    break;
            }
            black += Game::getPieceValue(Pieces::getPieceClass(p));
            black_bonuses += bonus / 3;
            black_pieces++;
        }
    }

    if (black_pieces + white_pieces > 10) {
        white += white_bonuses;
        black += black_bonuses;
    }

    return (white - black) * (color == WHITE ? 1 : -1) + mobility / 2 - 10;
}

void Board::printEvaluation() {
    float white = this->evaluateBoard(WHITE);
    float black = this->evaluateBoard(BLACK);

    std::cout << (white >= black ? "WHITE" : "BLACK") << " is winning!" << std::endl;
    std::cout << "White: " << white << std::endl;
    std::cout << "Black: " << black << std::endl;
}

std::vector<Move> Board::getLegalMoves(PieceColor color) {
    std::vector<Move> out;

    for (Location starting_location : ALL_LOCATIONS) {
        if (!this->getPieceAt(starting_location)) {
            continue;
        }
        if (Pieces::getPieceColor(this->getPieceAt(starting_location)) != color) {
            continue;
        }
        for (Location ending_location : ALL_LOCATIONS) {
            if (Game::isValidMove(this, starting_location, ending_location, true)) {
                Move m(starting_location, ending_location, this->getPieceAt(ending_location), 0);  // TODO: FIX NUMBER
                out.push_back(m);
            }
        }
    }

    return out;
}

Location Board::getSelectedLocation() {
    return this->selected_indices;
}

void Board::setSelectedPiece(Location loc) {
    this->selected_indices = loc;
}

void Board::clearSelectedPiece() {
    this->selected_indices = Location(-1, -1);
}

bool Board::hasSelectedPiece() {
    return (this->selected_indices.X != -1 && this->selected_indices.Y != -1);
}

bool Board::isInCheck(PieceColor c) {
    return Game::isInCheck(this, c);
}

bool Board::isCheckmate(PieceColor c) {
    return Game::isInCheckMate(this, c);
}

bool Board::checkCheckmates() {
    if (this->isCheckmate(WHITE)) {
        this->setCheckmate(WHITE);
        this->setGameOver();
        return true;
    } else if (this->isCheckmate(BLACK)) {
        this->setCheckmate(BLACK);
        this->setGameOver();
        return true;
    }
    return false;
}

PieceColor Board::getCheckmatedColor() {
    return this->checkmated_color;
}

bool Board::isGameOver() {
    return this->is_game_over;
}

void Board::setGameOver() {
    this->is_game_over = true;
}

void Board::setCheckmate(PieceColor c) {
    this->clearSelectedPiece();
    this->checkmated_color = c;
}

int Board::hash() {
    int hash_value = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            hash_value ^= (this->board[i][j] + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2));
        }
    }
    return hash_value;
}