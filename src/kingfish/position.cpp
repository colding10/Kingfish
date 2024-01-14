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
#include "zobrist.h"
#include "bitboard.h"

void Position::printBBoards() const {
    for (Color c : {CL_WHITE, CL_BLACK}) {
        for (PieceType p :
             {PT_PAWN, PT_KNIGHT, PT_BISHOP, PT_ROOK, PT_QUEEN, PT_KING}) {
            std::cout << "COLOR: " << (int)c << " PTYPE: " << (int)p
                      << std::endl;
            BBS::printBitboard(this->piece_bitboards[c][p]);
        }
    }
}

Piece Position::getPieceAt(Square square) const {
    if (!this->hasPieceAt(square)) {
        return PIECE_NONE;
    }

    for (Color c = CL_WHITE; c < CL_COUNT; c++) {
        for (PieceType p = PT_PAWN; p < PT_COUNT; p++) {
            if (get_bit(this->piece_bitboards[c][p], square)) {
                return Piece(c, p);
            }
        }
    }

    return PIECE_NONE;
}

bool Position::hasPieceAt(Square square) const {
    for (Color c = CL_WHITE; c < CL_COUNT; c++) {
        for (PieceType p = PT_PAWN; p < PT_COUNT; p++) {
            if (get_bit(this->piece_bitboards[c][p], square)) {
                return true;
            }
        }
    }

    return false;
}

void Position::setPieceAt(Square square, Piece piece) {
    this->popPieceAt(square);
    set_bit(this->piece_bitboards[piece.getColor()][piece.getType()], square);
}

void Position::popPieceAt(Square square) {
    for (Color c = CL_WHITE; c < CL_COUNT; c++) {
        for (PieceType p = PT_PAWN; p < PT_COUNT; p++) {
            pop_bit(this->piece_bitboards[c][p], square);
        }
    }
}

std::vector<Move> Position::genMoves(bool check_king) const {
    std::vector<Move> moves;
    for (int i = 0; i < (int)board.size(); i++) {
        char p = this->board[i];

        if (!std::isupper(p)) {
            continue;
        }

        for (int d : DIRECTIONS[p]) {
            for (int j = i + d;; j += d) {
                char q = this->board[j];
                if (std::isspace(q) || std::isupper(q)) {
                    break;
                }
                if (p == 'P') {
                    if ((d == DIR_NORTH || d == DIR_NORTH + DIR_NORTH) &&
                        q != '.') {
                        break;
                    }
                    if (d == DIR_NORTH + DIR_NORTH) {
                        if (i < A1 + DIR_NORTH ||
                            this->board[i + DIR_NORTH] != '.')
                            break;
                    }
                    if (d == DIR_NORTH + DIR_WEST ||
                        d == DIR_NORTH + DIR_EAST) {
                        if (q == '.' && j != ep && j != kp && j != kp - 1 &&
                            j != kp + 1)
                            break;
                    }
                    if (A8 <= j && j <= H8) {
                        for (char prom : {'N', 'B', 'R', 'Q'}) {
                            if (check_king) {
                                if (this->isValidMove(Move(i, j, prom))) {
                                    moves.push_back(Move(i, j, prom));
                                }
                            } else {
                                moves.push_back(Move(i, j, prom));
                            }
                        }
                        break;
                    }
                }
                if (check_king) {
                    if (this->isValidMove(Move(i, j, ' '))) {
                        moves.push_back(Move(i, j, ' '));
                    }
                } else {
                    moves.push_back(Move(i, j, ' '));
                }

                if (p == 'P' || p == 'N' || p == 'K' || std::islower(q)) {
                    break;
                }

                if (i == A1 && this->board[j + DIR_EAST] == 'K' &&
                    this->wc.first) {
                    moves.push_back(Move(j + DIR_EAST, j + DIR_WEST, ' '));
                }
                if (i == H1 && this->board[j + DIR_WEST] == 'K' &&
                    this->wc.second) {
                    moves.push_back(Move(j + DIR_WEST, j + DIR_EAST, ' '));
                }
            }
        }
    }

    return moves;
}

Position Position::rotate(bool nullmove) const {
    std::string rotated_board(board.rbegin(), board.rend());
    std::transform(rotated_board.begin(),
                   rotated_board.end(),
                   rotated_board.begin(),
                   [](char c) {
                       return std::islower(c) ? std::toupper(c)
                                              : std::tolower(c);
                   });
    return Position(rotated_board,
                    -score,
                    wc,
                    bc,
                    (ep && !nullmove) ? 119 - ep : 0,
                    (kp && !nullmove) ? 119 - kp : 0);
}

Position Position::move(const Move &move) const {
    int  i = move.i, j = move.j;
    char prom = move.prom;
    char p    = board[i];

    auto put = [](std::string b, int i, char p) -> std::string {
        b[i] = p;
        return b;
    };

    std::string new_board = put(std::string(board), j, board[i]);
    new_board             = put(new_board, i, '.');
    int new_score         = score + value(move);

    std::pair<bool, bool> new_wc = this->wc;
    std::pair<bool, bool> new_bc = this->bc;

    int new_kp = 0, new_ep = 0;

    if (i == A1) {
        new_wc = {false, wc.second};
    }
    if (i == H1) {
        new_wc = {wc.first, false};
    }
    if (j == A8) {
        new_bc = {bc.first, false};
    }
    if (j == H8) {
        new_bc = {false, bc.second};
    }

    if (p == 'K') {
        new_wc = {false, false};

        if (std::abs(j - i) == 2) {
            new_kp    = (i + j) / 2;
            new_board = put(std::string(new_board), j < i ? A1 : H1, '.');
            new_board = put(std::string(new_board), new_kp, 'R');
        }
    }

    if (p == 'P') {
        if (A8 <= j && j <= H8) {
            new_board = put(new_board, j, prom);
        }
        if (j - i == 2 * DIR_NORTH) {
            new_ep = i + DIR_NORTH;
        }
        if (j == ep) {
            new_board = put(std::string(new_board), j - DIR_SOUTH, '.');
        }
    }

    Position new_pos(new_board, new_score, new_wc, new_bc, new_ep, new_kp);
    return new_pos.rotate();
}

int Position::value(const Move &move) const {
    int i = move.i;
    int j = move.j;

    char p = board[i];
    char q = board[j];

    if (p == '.') {
        return -1;
    }

    // std::cout << "p: " << p << " i: " << i<<" j: " <<j << std::endl;
    int score =
        PIECE_SQUARE_TABLES.at(p).at(j) - PIECE_SQUARE_TABLES.at(p).at(i);

    // Capture
    if (std::islower(q)) {
        score += PIECE_SQUARE_TABLES[std::toupper(q)][119 - j];
    }

    // Castling check detection
    if (abs(j - kp) < 2) {
        score += PIECE_SQUARE_TABLES['K'][119 - j];
    }

    // Castling
    if (p == 'K' && abs(i - j) == 2) {
        score += PIECE_SQUARE_TABLES['R'][(i + j) / 2];
        score -= PIECE_SQUARE_TABLES['R'][j < i ? A1 : H1];
    }

    // Special pawn stuff
    if (p == 'P') {
        if (A8 <= j && j <= H8) {
            score += PIECE_SQUARE_TABLES.at(move.prom)[j] -
                     PIECE_SQUARE_TABLES['P'][j];
        }
        if (j == ep) {
            score += PIECE_SQUARE_TABLES['P'][119 - (j + DIR_SOUTH)];
        }
    }

    return score;
}

int Position::value() const {
    int score = 0;

    for (int i = H1; i <= A8; i++) {
        char c = this->board[i];

        if (islower(c) || isspace(c) || c == '.') {
            continue;
        }

        score += PIECE_SQUARE_TABLES[c][i];
    }

    return score;
}

bool Position::isValidMove(const Move &move) const {
    Position rotated = this->move(move);
    if (rotated.rotate().isCheck()) {
        return false;
    }
    return true;
}

bool Position::isCheck() const {
    Position          rotated = this->rotate();
    std::vector<Move> moves   = rotated.genMoves(false);

    return std::any_of(moves.cbegin(), moves.cend(), [&rotated](const Move &m) {
        return rotated.board[m.j] == 'k';
    });
}

bool Position::isCheckmate() const {
    if (!this->isCheck()) { // cant be mated while not in check
        return false;
    }

    std::vector<Move> moves = this->rotate().genMoves(false);

    return !std::any_of(moves.cbegin(), moves.cend(), [this](const Move &m) {
        return !this->move(m).isCheck();
    });
}

PositionHash Position::hash() const {
    return zobristHash(*this, true);
}
