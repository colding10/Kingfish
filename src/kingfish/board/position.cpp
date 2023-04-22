#include "./position.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../book/zobrist.h"
#include "../consts.h"
#include "../move.h"
#include "../piece.h"
#include "bitboard.h"

void Position::printBBoards() {
    for (Color c : {CL_WHITE, CL_BLACK}) {
        for (PieceType p :
             {PT_PAWN, PT_KNIGHT, PT_BISHOP, PT_ROOK, PT_QUEEN, PT_KING}) {
            std::cout << "COLOR: " << (int)c << " PTYPE: " << (int)p
                      << std::endl;
            BBS::printBitboard(this->piece_bitboards[c][p]);
        }
    }
}

std::vector<Move> Position::genMoves(bool check_king) {
    // set starting pos
    for (Square i = 0; i < SQ_COUNT; i++) {
        if (STARTING_POSITION[i] != PIECE_NONE) {
            Piece p = STARTING_POSITION[i];
            set_bit(this->piece_bitboards[p.getColor()][p.getType()], i);
            set_bit(this->occupied_bitboards[p.getColor()], i);
        }
    }

    std::vector<Move> moves;

    for (Square i = SQ_A1; i < SQ_INVALID; i = static_cast<Squares>(i + 1)) {
        if (!this->hasPieceAt(
                i)) { //  || this->getPieceAt(i).getColor() != this->turn
            continue;
        }

        Piece p = this->getPieceAt(i);

        Bitboard attack_bb = 0;

        switch (p.getType()) {
            case PT_PAWN: attack_bb = BBS::pawnAttacks(this->turn, i); break;
            case PT_KNIGHT: attack_bb = BBS::knightAttacks(i); break;
            case PT_BISHOP:
                attack_bb = BBS::bishopAttacks(i,
                                               this->occupied_bitboards[0] |
                                                   this->occupied_bitboards[1]);
                break;
            case PT_ROOK:
                attack_bb = BBS::bishopAttacks(i,
                                               this->occupied_bitboards[0] |
                                                   this->occupied_bitboards[1]);
                break;
            case PT_QUEEN:
                attack_bb = BBS::queenAttacks(i,
                                              this->occupied_bitboards[0] |
                                                  this->occupied_bitboards[1]);
                break;
            case PT_KING: attack_bb = BBS::kingAttacks(i);
        }

        BBS::printBitboard(attack_bb);
        if (p.getType() == PT_PAWN) {
            if (this->isValidMove(Move(i, i + DIR_NORTH, ' '))) {
                moves.push_back(Move(i, i + DIR_NORTH, ' '));
            }

            for (Square s = 0; s < SQ_COUNT; s++) {
                if (!get_bit(attack_bb, s)) {
                    continue;
                }
                std::cout << "h: " << this->hasPieceAt(s) << std::endl;
                if (this->isValidMove(Move(i, s, ' ')) && this->hasPieceAt(s)) {
                    moves.push_back(Move(i, s, ' '));
                }
            }

            if (getRank(i) == RANK_7 && this->turn == CL_WHITE) {
                for (PieceType prom :
                     {PT_KNIGHT, PT_BISHOP, PT_ROOK, PT_QUEEN}) {
                    moves.push_back(Move(
                        i, i + DIR_NORTH, PIECE_IDENTIFIER[CL_BLACK][prom]));
                    // std::cout << "Move(" << i << ", " << i + DIR_NORTH << ", "
                    //   << prom << ")\n";
                }
            } else if (getRank(i) == RANK_2 && this->turn == CL_BLACK) {
                for (PieceType prom :
                     {PT_KNIGHT, PT_BISHOP, PT_ROOK, PT_QUEEN}) {
                    moves.push_back(Move(
                        i, i + DIR_NORTH, PIECE_IDENTIFIER[CL_BLACK][prom]));
                }
            } else {
                if (getRank(i) == RANK_2 && this->turn == CL_WHITE) {
                    if (this->isValidMove(
                            Move(i, i + DIR_NORTH + DIR_NORTH, ' '))) {
                        if (!this->hasPieceAt(i) &&
                            !this->hasPieceAt(i + DIR_NORTH + DIR_NORTH))
                            moves.push_back(
                                Move(i, i + DIR_NORTH + DIR_NORTH, ' '));
                    }
                } else if (getRank(i) == RANK_7 && this->turn == CL_BLACK) {
                    if (this->isValidMove(
                            Move(i, i + DIR_SOUTH + DIR_SOUTH, ' '))) {
                        if (!this->hasPieceAt(i) &&
                            !this->hasPieceAt(i + DIR_SOUTH + DIR_SOUTH))
                            moves.push_back(
                                Move(i, i + DIR_SOUTH + DIR_SOUTH, ' '));
                    }
                }
            }
        } else {
            for (Square s = 0; s < SQ_COUNT; s++) {
                if (!get_bit(attack_bb, s)) {
                    continue;
                }

                if (this->isValidMove(Move(i, s, ' '))) {
                    moves.push_back(Move(i, s, ' '));
                }

                // std::cout << "Move(" << (int)i << ", " << (int)s << ", "
                //           << ")" << std::endl;
            }
        }
        //         if (p == 'P') {
        //             if ((d == DIR_NORTH || d == DIR_NORTH + DIR_NORTH) &&
        //                 q != '.') {
        //                 break;
        //             }
        //             if (d == DIR_NORTH + DIR_NORTH) {
        //                 if (i < A1 + DIR_NORTH ||
        //                     this->board[i + DIR_NORTH] != '.')
        //                     break;
        //             }
        //             if (d == DIR_NORTH + DIR_WEST ||
        //                 d == DIR_NORTH + DIR_EAST) {
        //                 if (q == '.' && j != ep && j != kp && j != kp - 1 &&
        //                     j != kp + 1)
        //                     break;
        //             }
        //             if (A8 <= j && j <= H8) {
        //                 for (char prom : "NBRQ") {
        //                     if (prom == '\x00') {
        //                         break;
        //                     }

        //                     if (check_king) {
        //                         if (this->isValidMove(Move(i, j, prom))) {
        //                             moves.push_back(Move(i, j, prom));
        //                         }
        //                     } else {
        //                         moves.push_back(Move(i, j, prom));
        //                     }
        //                 }
        //                 break;
        //             }
        //         }
        //         if (check_king) {
        //             if (this->isValidMove(Move(i, j, ' '))) {
        //                 moves.push_back(Move(i, j, ' '));
        //             }
        //         } else {
        //             moves.push_back(Move(i, j, ' '));
        //         }

        //         if (p == 'P' || p == 'N' || p == 'K' || std::islower(q)) {
        //             break;
        //         }

        //         if (i == A1 && this->board[j + DIR_EAST] == 'K' &&
        //             this->wc.first) {
        //             moves.push_back(Move(j + DIR_EAST, j + DIR_WEST, ' '));
        //         }
        //         if (i == H1 && this->board[j + DIR_WEST] == 'K' &&
        //             this->wc.second) {
        //             moves.push_back(Move(j + DIR_WEST, j + DIR_EAST, ' '));
        //         }
        //     }
        // }
    }

    return moves;
}

Piece Position::getPieceAt(Square square) {
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

bool Position::hasPieceAt(Square square) {
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

Position Position::rotate(bool nullmove) {
    // std::string rotated_board(board.rbegin(), board.rend());
    // std::transform(rotated_board.begin(),
    //                rotated_board.end(),
    //                rotated_board.begin(),
    //                [](char c) {
    //                    return std::islower(c) ? std::toupper(c)
    //                                           : std::tolower(c);
    //                });
    // return Position(rotated_board,
    //                 -score,
    //                 wc,
    //                 bc,
    //                 (ep && !nullmove) ? 119 - ep : 0,
    //                 (kp && !nullmove) ? 119 - kp : 0);
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

int Position::value(const Move &move) {
    int i = move.i;
    int j = move.j;

    char p = board[i];
    char q = board[j];

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
    if (!isValidSquare(move.i) || !isValidSquare(move.j)) {
        return false;
    }

    if (this->getPieceAt(move.i) == PIECE_NONE) {
        return false;
    }

    if (this->getPieceAt(move.i).getColor() ==
        this->getPieceAt(move.j).getColor()) {
        return false;
    }

    return true;
}

bool Position::isCheck() {
    Position          rotated = this->rotate();
    std::vector<Move> moves   = rotated.genMoves(false);

    return std::any_of(moves.cbegin(), moves.cend(), [&rotated](const Move &m) {
        return rotated.board[m.j] == 'k';
    });
}

bool Position::isCheckmate() {
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

PositionHash Position::hash() {
    return zobristHash(*this, true);
}