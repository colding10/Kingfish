#include "position.hpp"

#include <string.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "consts.hpp"
#include "move.hpp"
 // TODO: fix gen move, check, and checkmate`
std::vector<Move> Position::genMoves(bool check_king) {
    std::vector<Move> moves;
    for (int i = 0; i < (int)board.size(); ++i) {
        char p = this->board[i];

        if (!isupper(p)) {
            continue;
        }

        for (int d : DIRECTIONS[p]) {
            for (int j = i + d;; j += d) {
                char q = board[j];
                if (isspace(q) || isupper(q)) {
                    break;
                }
                if (p == 'P') {
                    if ((d == N || d == N + N) && q != '.') {
                        break;
                    }
                    if (d == N + N) {
                        if (i < A1 + N || board[i + N] != '.')
                            break;
                    }
                    if (d == N + W || d == N + E) {
                        if (q == '.' && j != ep && j != kp && j != kp - 1 &&
                            j != kp + 1)
                            break;
                    }
                    if (A8 <= j && j <= H8) {
                        for (char prom : "NBRQ") {
                            if (check_king &&
                                this->isValidMove(Move(i, j, prom))) {
                                moves.push_back(Move(i, j, prom));
                            }
                        }
                        break;
                    }
                }
                if (check_king && this->isValidMove(Move(i, j, ' '))) {
                    moves.push_back(Move(i, j, ' '));
                }

                if (p == 'P' || p == 'N' || p == 'K' || islower(q)) {
                    break;
                }

                if (i == A1 && board[j + E] == 'K' && this->wc.first) {
                    moves.push_back(Move(j + E, j + W, ' '));
                }
                if (i == H1 && board[j + W] == 'K' && this->wc.second) {
                    moves.push_back(Move(j + W, j + E, ' '));
                }
            }
        }
    }

    return moves;
}

Position Position::rotate(bool nullmove) {
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

Position Position::move(const Move &move) {
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
        if (j - i == 2 * N) {
            new_ep = i + N;
        }
        if (j == ep) {
            new_board = put(std::string(new_board), j - S, '.');
        }
    }

    Position new_pos(new_board, new_score, new_wc, new_bc, new_ep, new_kp);
    return new_pos.rotate();
}

int Position::value(const Move &move) {
    int i = move.i;
    int j = move.j;

    char p = board[i];
    char q = board[j];

    if (PIECE_SQUARE_TABLES.count(p) == 0) {
        std::cout << "p: " << p << " q: " << q << std::endl;
    }

    int score =
        PIECE_SQUARE_TABLES.at(p).at(j) - PIECE_SQUARE_TABLES.at(p).at(i);

    // Capture
    if (islower(q)) {
        score += PIECE_SQUARE_TABLES[toupper(q)][119 - j];
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
            score +=
                PIECE_SQUARE_TABLES[move.prom][j] - PIECE_SQUARE_TABLES['P'][j];
        }
        if (j == ep) {
            score += PIECE_SQUARE_TABLES['P'][119 - (j + S)];
        }
    }

    return score;
}

int Position::value() {
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

bool Position::isValidMove(const Move &move) {
    return true;
    Position rotated = this->move(move);
    if (rotated.rotate().isCheck()) {
        return false;
    }
    return true;
}

bool Position::isCheck() {
    Position rotated = this->rotate();
    for (Move m : rotated.genMoves(false)) {
        if (rotated.board[m.j] == 'k') {
            return false;
        }
    }

    return true;
}

bool Position::isCheckmate() {
    return false;
    if (!this->isCheck()) { // cant be mated while not in check
        return false;
    }

    for (Move m : this->genMoves()) {
        if (!this->move(m).isCheck()) { // we can get out of check with a move
            return false;
        }
    }

    return true; // we are mated
}

int Position::hash() {
    auto str_hasher = std::hash<std::string>();
    auto int_hasher = std::hash<int>();

    return (str_hasher(board) ^ int_hasher(score) ^ int_hasher(wc.first) ^
            int_hasher(wc.second) ^ int_hasher(bc.first) ^
            int_hasher(bc.second) ^ int_hasher(ep) ^ int_hasher(kp));
}