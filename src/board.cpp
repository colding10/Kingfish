#include "board.hpp"

#include <array>
#include <cctype>
#include <iostream>
#include <limits>
#include <map>
#include <stack>
#include <string>
#include <vector>

#include "game.hpp"
#include "location.hpp"
#include "move.hpp"
#include "pieces.hpp"

// TODO: add other FEN fields
void Board::readFen(std::string fen) {
    int row = 0;
    int col = 0;

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

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (this->board[row][col] == PAWN) {
                c = 'p';
            } else if (this->board[row][col] == ROOK) {
                c = 'r';
            } else if (this->board[row][col] == KNIGHT) {
                c = 'n';
            } else if (this->board[row][col] == BISHOP) {
                c = 'b';
            } else if (this->board[row][col] == QUEEN) {
                c = 'q';
            } else if (this->board[row][col] == KING) {
                c = 'k';
            }

            if (Pieces::getPieceColor(c) == WHITE) {
                c += 32;
            }
            fen += c;
        }
        fen += "/";
    }

    return fen;
}

// TODO: add other info to board printing
void Board::printBoard() {
    std::cout << "CHESSBOARD INFO" << std::endl;
    std::cout << "Turn: " << (this->getActiveColor() == WHITE ? "white" : "black") << std::endl;
    std::cout << "Move: " << this->getMoveNumber() << std::endl;
    std::cout << "FEN: " << this->getFen() << std::endl;
    std::cout << "Hash: " << this->hash() << std::endl;

    this->printEvaluation();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << this->board[i][j] << std::string((board[i][j] >= 10 ? 1 : 2), ' ');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
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
    int board[8][8];

    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            board[7 - i][j] = this->board[i][j];
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->board[i][j] = board[i][j];
        }
    }

    this->is_reversed = !this->is_reversed;
}

bool Board::isReversed() {
    return this->is_reversed;
}

void Board::tryMove(Move move) {
    if (Game::isValidMove(this, move.getStarting(), move.getEnding(), true)) {
        this->makeMove(move, true);
    }
}

void Board::makeMove(Move move, bool push_to_stack) {
    // if (Pieces::getPieceClass(this->getPieceAt(move.getStarting())) == KING) {
    //     if (abs(move.getEnding().X - move.getEnding().Y) == 2) {
    //         if (move.getEnding().Y > move.getStarting().Y) {
    //             this->board[move.getStarting().X][7] = 0x00;
    //             this->board[move.getStarting().X][move.getEnding().Y - 1] = Pieces::makePiece(ROOK, this->getActiveColor());
    //         } else {
    //             this->board[move.getStarting().X][0] = 0x00;
    //             this->board[move.getStarting().X][move.getEnding().Y + 1] = Pieces::makePiece(ROOK, this->getActiveColor());
    //         }
    //     }
    // }

    move.setCaptured(this->board[move.getEnding().X][move.getEnding().Y]);

    this->board[move.getEnding().X][move.getEnding().Y] = this->board[move.getStarting().X][move.getStarting().Y];
    this->board[move.getStarting().X][move.getStarting().Y] = 0x00;

    if (push_to_stack) {
        this->move_stack.push(move);
        this->clearSelectedPiece();

        this->incrementMoveNumber();
        this->toggleActiveColor();
    }

    // if (Pieces::getPieceClass(this->getPieceAt({move.getEnding().X, move.getEnding().Y})) == PAWN) {
    //     if (move.getEnding().X == 0 && Pieces::getPieceColor(this->getPieceAt({move.getEnding().X, move.getEnding().Y})) == WHITE) {
    //         this->board[move.getEnding().X][move.getEnding().Y] = Pieces::makePiece(QUEEN, WHITE);
    //     } else if (move.getEnding().X == 7 && Pieces::getPieceColor(this->getPieceAt({move.getEnding().X, move.getEnding().Y})) == BLACK) {
    //         this->board[move.getEnding().X][move.getEnding().Y] = Pieces::makePiece(QUEEN, BLACK);
    //     }
    // }
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

int Board::getPieceAt(Location location) {
    return this->board[location.X][location.Y];
}

float Board::evaluateBoard(int color) {
    if (Game::isInCheckMate(this, color == WHITE ? BLACK : WHITE)) {
        return std::numeric_limits<float>::max();
    } else if (Game::isInCheckMate(this, color)) {
        return -std::numeric_limits<float>::max();
    }

    std::map<Piece, float> piece_values = {
        {PAWN, 100},
        {KNIGHT, 320},
        {BISHOP, 330},
        {ROOK, 500},
        {QUEEN, 15000},
        {KING, 0}};

    int pawn_table[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {5, 5, 5, -10, -10, 5, 5, 5},
        {5, 0, 0, 10, 10, 0, 0, 5},
        {5, 5, 5, 25, 25, 5, 5, 5},
        {10, 10, 15, 25, 25, 15, 10, 10},
        {20, 20, 20, 30, 30, 30, 20, 20},
        {30, 30, 30, 40, 40, 30, 30, 30},
        {0, 0, 0, 0, 0, 0, 0, 0}};
    int knight_table[8][8] = {
        {-5, -10, -5, -5, -5, -5, -10, -5},
        {-5, 0, 0, 5, 5, 0, 0, -5},
        {-5, 5, 10, 10, 10, 10, 5, -5},
        {-5, 5, 10, 15, 15, 10, 5, -5},
        {-5, 5, 10, 15, 15, 10, 5, -5},
        {-5, 5, 10, 10, 10, 10, 5, -5},
        {-5, 0, 0, 10, 10, 0, 0, -5},
        {-5, -5, -5, -5, -5, -5, -5, -5}};
    int bishop_table[8][8] = {
        {0, 0, -10, 0, 0, -10, 0, 0},
        {0, 10, 0, 10, 10, 0, 10, 0},
        {0, 10, 0, 10, 10, 0, 10, 0},
        {5, 0, 10, 0, 0, 10, 0, 5},
        {0, 10, 0, 0, 0, 0, 10, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}};
    int rook_table[8][8] = {
        {0, 0, 0, 10, 10, 5, 0, 0},
        {0, 0, 0, 10, 10, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {10, 10, 10, 10, 10, 10, 10, 10},
        {10, 10, 10, 10, 10, 10, 10, 10}};
    int queen_table[8][8] = {
        {-20, -10, -10, 0, 0, -10, -10, -20},
        {-10, 0, 5, 0, 0, 0, 0, -10},
        {-10, 5, 5, 5, 5, 5, 0, -10},
        {-5, 0, 5, 5, 5, 5, 0, -5},
        {-5, 0, 5, 5, 5, 5, 0, -5},
        {-10, 0, 5, 0, 0, 0, 0, -10},
        {-20, -10, -10, -5, -5, -10, -10, -20},
    };
    int king_table[8][8] = {
        {0, 0, 20, -5, -5, -5, 20, 0},
        {0, 0, 0, -5, -5, -5, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}};

    float white = 0.0f;
    float black = 0.0f;

    float white_bonuses = 0.0f;
    float black_bonuses = 0.0f;

    int white_pieces = 0;
    int black_pieces = 0;

    float mobility = this->getLegalMoves(color).size();

    if (mobility == 0) {
        return 0.0f;
    }

    Piece p;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            p = this->getPieceAt(Location(i, j));

            if (!p) {
                continue;
            } else if (Pieces::getPieceColor(p) == WHITE) {
                int bonus;
                switch (Pieces::getPieceClass(p)) {
                    case PAWN:
                        bonus = pawn_table[color == BLACK ? i : 7 - i][j];
                        break;
                    case KNIGHT:
                        bonus = knight_table[color == BLACK ? i : 7 - i][j];
                        break;
                    case BISHOP:
                        bonus = bishop_table[color == BLACK ? i : 7 - i][j];
                        break;
                    case ROOK:
                        bonus = rook_table[color == BLACK ? i : 7 - i][j];
                        break;
                    case QUEEN:
                        bonus = queen_table[color == BLACK ? i : 7 - i][j];
                        break;
                    case KING:
                        bonus = king_table[color == BLACK ? i : 7 - i][j];
                        break;
                }
                white += piece_values[Pieces::getPieceClass(p)];
                white_bonuses += bonus / 3;
                white_pieces++;
            } else {
                int bonus;
                switch (Pieces::getPieceClass(p)) {
                    case PAWN:
                        bonus = pawn_table[color == BLACK ? i : 7 - i][j];
                        break;
                    case KNIGHT:
                        bonus = knight_table[color == BLACK ? i : 7 - i][j];
                        break;
                    case BISHOP:
                        bonus = bishop_table[color == BLACK ? i : 7 - i][j];
                        break;
                    case ROOK:
                        bonus = rook_table[color == BLACK ? i : 7 - i][j];
                        break;
                    case QUEEN:
                        bonus = queen_table[color == BLACK ? i : 7 - i][j];
                        break;
                    case KING:
                        bonus = king_table[color == BLACK ? i : 7 - i][j];
                        break;
                }
                black += piece_values[Pieces::getPieceClass(p)];
                black_bonuses += bonus / 3;
                black_pieces++;
            }
        }
    }

    // if (black_pieces + white_pieces > 10) {
    //     white += white_bonuses;
    //     black += black_bonuses;
    // }

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
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!this->getPieceAt(Location(i, j))) {
                continue;
            }
            if (Pieces::getPieceColor(this->getPieceAt(Location(i, j))) != color) {
                continue;
            }
            for (int a = 0; a < 8; a++) {
                for (int b = 0; b < 8; b++) {
                    if (Game::isValidMove(this, Location(i, j), Location(a, b), true)) {
                        Move m(Location(i, j), Location(a, b), this->getPieceAt(Location(a, b)), 0);  // TODO: FIX NUMBER
                        out.push_back(m);
                    }
                }
            }
        }
    }

    return out;
}

Location Board::getSelectedLocation() {
    return this->selected_indices;
}

void Board::setSelectedPiece(int i, int j) {
    this->selected_indices = Location(i, j);
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
    return Game::isInCheck(this, c);
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