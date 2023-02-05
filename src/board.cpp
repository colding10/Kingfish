#include "board.hpp"

#include <array>
#include <cctype>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "defines.hpp"
#include "game.hpp"
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
// TODO: add other info to board printing
void Board::printBoard() {
    std::cout << "CHESSBOARD INFO" << std::endl;
    std::cout << "Turn: " << (this->getActiveColor() == WHITE ? "white" : "black") << std::endl;
    std::cout << "Move: " << this->getMoveNumber() << std::endl;
    std::cout << "White value: " << this->evaluateBoard(WHITE) << std::endl;

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

void Board::tryMove(Location starting, Location ending) {
    if (this->getActiveColor() != Pieces::getPieceColor(this->getPieceAt(starting))) {  // out of turn
        return;
    }

    int r1, c1, r2, c2;

    r1 = starting.first;
    c1 = starting.second;

    r2 = ending.first;
    c2 = ending.second;

    Move m;
    m.startX = r1;
    m.startY = c1;
    m.endX = r2;
    m.endY = c2;
    m.captured = this->board[r2][c2];

    if (Game::isValidMove(this, starting, ending, true)) {
        this->makeMove(m);
        this->clearSelectedPiece();

        this->incrementMoveNumber();
        this->toggleActiveColor();
    }
}

void Board::makeMove(Location starting, Location ending) {
    int r1, c1, r2, c2;

    r1 = starting.first;
    c1 = starting.second;

    r2 = ending.first;
    c2 = ending.second;

    Move m;
    m.startX = r1;
    m.startY = c1;
    m.endX = r2;
    m.endY = c2;
    m.captured = this->board[r2][c2];

    this->last_move = m;

    this->board[r2][c2] = this->board[r1][c1];
    this->board[r1][c1] = 0x00;
}

void Board::makeMove(Move move) {
    if (Pieces::getPieceClass(this->getPieceAt({move.startX, move.startY})) == KING) {
        if (abs(move.startY - move.endY) == 2) {
            if (move.endY > move.startY) {
                this->board[move.startX][7] = 0x00;
                this->board[move.startX][move.endY - 1] = Pieces::makePiece(ROOK, this->getActiveColor());
            } else {
                this->board[move.startX][0] = 0x00;
                this->board[move.startX][move.endY + 1] = Pieces::makePiece(ROOK, this->getActiveColor());
            }
        }
    }

    move.captured = this->board[move.endX][move.endY];
    this->board[move.endX][move.endY] = this->board[move.startX][move.startY];
    this->board[move.startX][move.startY] = 0x00;

    this->last_move = move;

    if (Pieces::getPieceClass(this->getPieceAt({move.endX, move.endY})) == PAWN) {
        if (move.endX == 0 && Pieces::getPieceColor(this->getPieceAt({move.endX, move.endY})) == WHITE) {
            this->board[move.endX][move.endY] = Pieces::makePiece(QUEEN, WHITE);
        } else if (move.endX == 7 && Pieces::getPieceColor(this->getPieceAt({move.endX, move.endY})) == BLACK) {
            this->board[move.endX][move.endY] = Pieces::makePiece(QUEEN, BLACK);
        }
    }
}

void Board::undoMove() {
    this->board[this->last_move.startX][this->last_move.startY] = this->board[this->last_move.endX][this->last_move.endY];
    this->board[this->last_move.endX][this->last_move.endY] = this->last_move.captured;
}

int Board::getPieceAt(Location location) {
    return this->board[location.first][location.second];
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

    float mobility = this->getAllMoves(color).size();

    if (mobility == 0) {
        return 0.0f;
    }

    Piece p;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            p = this->getPieceAt(makeLocation(i, j));

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

std::vector<Move> Board::getAllMoves(PieceColor color) {
    std::vector<Move> out;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int a = 0; a < 8; a++) {
                for (int b = 0; b < 8; b++) {
                    if (!this->getPieceAt({i, j})) {
                        continue;
                    } else if (!(Pieces::getPieceColor(this->getPieceAt({i, j})) == color)) {
                        continue;
                    }

                    if (Game::isValidMove(this, {i, j}, {a, b}, true)) {
                        Move m;

                        m.startX = i;
                        m.startY = j;
                        m.endX = a;
                        m.endY = b;

                        m.captured = this->getPieceAt(makeLocation(a, b));

                        if (this->getPieceAt({a, b})) {
                            m.is_capture = true;
                        } else {
                            m.is_capture = false;
                        }

                        out.push_back(m);
                    }
                }
            }
        }
    }

    return out;
}

Location Board::getSelectedPiece() {
    return this->selected_piece;
}

void Board::setSelectedPiece(int i, int j) {
    this->selected_piece = makeLocation(i, j);
}

void Board::clearSelectedPiece() {
    this->selected_piece = makeLocation(-1, -1);
}

bool Board::hasSelectedPiece() {
    return (this->selected_piece.first != -1 && this->selected_piece.second != -1);
}