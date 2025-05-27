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
#include "magic.h"

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
        return NO_PIECE;
    }

    for (Color c = CL_WHITE; c < CL_COUNT; c++) {
        for (PieceType p = PT_PAWN; p < PT_COUNT; p++) {
            if (get_bit(this->piece_bitboards[c][p], square)) {
                return make_piece(c, p);
            }
        }
    }

    return NO_PIECE;
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

void Position::setPieceAt(Square square, i8 piece) {
    this->popPieceAt(square);
    set_bit(this->piece_bitboards[piece_color(piece)][piece_type(piece)], square);
}

void Position::popPieceAt(Square square) {
    for (Color c = CL_WHITE; c < CL_COUNT; c++) {
        for (PieceType p = PT_PAWN; p < PT_COUNT; p++) {
            pop_bit(this->piece_bitboards[c][p], square);
        }
    }
}

void Position::update_bitboards() {
    // Clear all bitboards
    for (int color = 0; color < CL_COUNT; color++) {
        for (int piece = 0; piece < PT_COUNT; piece++) {
            piece_bitboards[color][piece] = 0ULL;
        }
        occupied_bitboards[color] = 0ULL;
    }
    all_pieces = 0ULL;

    // Update bitboards based on board string
    for (int square = 0; square < 64; square++) {
        Piece piece = getPieceAt(square);
        if (piece != NO_PIECE) {
            Color color = piece_color(piece);
            PieceType type = piece_type(piece);
            set_bit(piece_bitboards[color][type], square);
            set_bit(occupied_bitboards[color], square);
            set_bit(all_pieces, square);
        }
    }
}

Bitboard Position::get_attacks(Square square, i8 piece) const {
    Color color = piece_color(piece);
    PieceType type = piece_type(piece);
    Bitboard attacks = 0ULL;

    switch (type) {
        case PT_PAWN:
            attacks = BBS::pawnAttacks(color, square);
            break;
        case PT_KNIGHT:
            attacks = BBS::knightAttacks(square);
            break;
        case PT_BISHOP:
            attacks = Magic::get_bishop_attacks(square, all_pieces);
            break;
        case PT_ROOK:
            attacks = Magic::get_rook_attacks(square, all_pieces);
            break;
        case PT_QUEEN:
            attacks = Magic::get_queen_attacks(square, all_pieces);
            break;
        case PT_KING:
            attacks = BBS::kingAttacks(square);
            break;
        default:
            break;
    }

    return attacks;
}

Bitboard Position::get_side_attacks(Color side) const {
    Bitboard attacks = 0ULL;
    for (int piece_type = PT_PAWN; piece_type <= PT_KING; piece_type++) {
        Bitboard pieces = piece_bitboards[side][piece_type];
        while (pieces) {
            Square square = BBS::get_ls1b_index(pieces);
            attacks |= get_attacks(square, make_piece(side, piece_type));
            pop_bit(pieces, square);
        }
    }
    return attacks;
}

bool Position::is_square_attacked(Square square, Color by_side) const {
    return get_bit(get_side_attacks(by_side), square);
}

std::vector<Move> Position::genPawnMoves(Square square) const {
    std::vector<Move> moves;
    Color color = turn;
    Bitboard pawn = piece_bitboards[color][PT_PAWN] & (1ULL << square);
    if (!pawn) return moves;

    // Forward moves
    Bitboard forward = color == CL_WHITE ? (pawn << 8) : (pawn >> 8);
    forward &= ~all_pieces;
    if (forward) {
        Square target = BBS::get_ls1b_index(forward);
        if (target / 8 == (color == CL_WHITE ? 7 : 0)) {
            // Promotion
            moves.push_back(Move(square, target, 'Q'));
            moves.push_back(Move(square, target, 'R'));
            moves.push_back(Move(square, target, 'B'));
            moves.push_back(Move(square, target, 'N'));
        } else {
            moves.push_back(Move(square, target, ' '));
        }
    }

    // Double push
    if (color == CL_WHITE && square / 8 == 1) {
        Bitboard double_push = pawn << 16;
        double_push &= ~all_pieces;
        if (double_push) {
            moves.push_back(Move(square, BBS::get_ls1b_index(double_push), ' '));
        }
    } else if (color == CL_BLACK && square / 8 == 6) {
        Bitboard double_push = pawn >> 16;
        double_push &= ~all_pieces;
        if (double_push) {
            moves.push_back(Move(square, BBS::get_ls1b_index(double_push), ' '));
        }
    }

    // Captures
    Bitboard attacks = BBS::pawnAttacks(color, square);
    attacks &= occupied_bitboards[!color];
    while (attacks) {
        Square target = BBS::get_ls1b_index(attacks);
        if (target / 8 == (color == CL_WHITE ? 7 : 0)) {
            // Promotion captures
            moves.push_back(Move(square, target, 'Q'));
            moves.push_back(Move(square, target, 'R'));
            moves.push_back(Move(square, target, 'B'));
            moves.push_back(Move(square, target, 'N'));
        } else {
            moves.push_back(Move(square, target, ' '));
        }
        pop_bit(attacks, target);
    }

    // En passant
    if (ep != -1) {
        Bitboard ep_attacks = BBS::pawnAttacks(color, square) & (1ULL << ep);
        if (ep_attacks) {
            moves.push_back(Move(square, ep, ' '));
        }
    }

    return moves;
}

std::vector<Move> Position::genKnightMoves(Square square) const {
    std::vector<Move> moves;
    Bitboard knight = piece_bitboards[turn][PT_KNIGHT] & (1ULL << square);
    if (!knight) return moves;

    Bitboard attacks = BBS::knightAttacks(square);
    attacks &= ~occupied_bitboards[turn];
    while (attacks) {
        Square target = BBS::get_ls1b_index(attacks);
        moves.push_back(Move(square, target, ' '));
        pop_bit(attacks, target);
    }
    return moves;
}

std::vector<Move> Position::genBishopMoves(Square square) const {
    std::vector<Move> moves;
    Bitboard bishop = piece_bitboards[turn][PT_BISHOP] & (1ULL << square);
    if (!bishop) return moves;

    Bitboard attacks = Magic::get_bishop_attacks(square, all_pieces);
    attacks &= ~occupied_bitboards[turn];
    while (attacks) {
        Square target = BBS::get_ls1b_index(attacks);
        moves.push_back(Move(square, target, ' '));
        pop_bit(attacks, target);
    }
    return moves;
}

std::vector<Move> Position::genRookMoves(Square square) const {
    std::vector<Move> moves;
    Bitboard rook = piece_bitboards[turn][PT_ROOK] & (1ULL << square);
    if (!rook) return moves;

    Bitboard attacks = Magic::get_rook_attacks(square, all_pieces);
    attacks &= ~occupied_bitboards[turn];
    while (attacks) {
        Square target = BBS::get_ls1b_index(attacks);
        moves.push_back(Move(square, target, ' '));
        pop_bit(attacks, target);
    }
    return moves;
}

std::vector<Move> Position::genQueenMoves(Square square) const {
    std::vector<Move> moves;
    Bitboard queen = piece_bitboards[turn][PT_QUEEN] & (1ULL << square);
    if (!queen) return moves;

    Bitboard attacks = Magic::get_queen_attacks(square, all_pieces);
    attacks &= ~occupied_bitboards[turn];
    while (attacks) {
        Square target = BBS::get_ls1b_index(attacks);
        moves.push_back(Move(square, target, ' '));
        pop_bit(attacks, target);
    }
    return moves;
}

std::vector<Move> Position::genKingMoves(Square square) const {
    std::vector<Move> moves;
    Bitboard king = piece_bitboards[turn][PT_KING] & (1ULL << square);
    if (!king) return moves;

    Bitboard attacks = BBS::kingAttacks(square);
    attacks &= ~occupied_bitboards[turn];
    while (attacks) {
        Square target = BBS::get_ls1b_index(attacks);
        moves.push_back(Move(square, target, ' '));
        pop_bit(attacks, target);
    }

    // Castling
    if (turn == CL_WHITE) {
        if (wc.first && !(all_pieces & 0x60ULL) && !is_square_attacked(5, CL_BLACK) && !is_square_attacked(6, CL_BLACK)) {
            moves.push_back(Move(4, 6, ' ')); // Kingside
        }
        if (wc.second && !(all_pieces & 0xEULL) && !is_square_attacked(3, CL_BLACK) && !is_square_attacked(2, CL_BLACK)) {
            moves.push_back(Move(4, 2, ' ')); // Queenside
        }
    } else {
        if (bc.first && !(all_pieces & 0x6000000000000000ULL) && !is_square_attacked(61, CL_WHITE) && !is_square_attacked(62, CL_WHITE)) {
            moves.push_back(Move(60, 62, ' ')); // Kingside
        }
        if (bc.second && !(all_pieces & 0x0E00000000000000ULL) && !is_square_attacked(59, CL_WHITE) && !is_square_attacked(58, CL_WHITE)) {
            moves.push_back(Move(60, 58, ' ')); // Queenside
        }
    }

    return moves;
}

std::vector<Move> Position::genMoves(bool check_king) const {
    std::vector<Move> moves;
    Color side = turn;

    // Generate moves for each piece type
    for (int piece_type = PT_PAWN; piece_type <= PT_KING; piece_type++) {
        Bitboard pieces = piece_bitboards[side][piece_type];
        while (pieces) {
            Square square = BBS::get_ls1b_index(pieces);
            std::vector<Move> piece_moves;

            switch (piece_type) {
                case PT_PAWN:
                    piece_moves = genPawnMoves(square);
                    break;
                case PT_KNIGHT:
                    piece_moves = genKnightMoves(square);
                    break;
                case PT_BISHOP:
                    piece_moves = genBishopMoves(square);
                    break;
                case PT_ROOK:
                    piece_moves = genRookMoves(square);
                    break;
                case PT_QUEEN:
                    piece_moves = genQueenMoves(square);
                    break;
                case PT_KING:
                    piece_moves = genKingMoves(square);
                    break;
            }

            moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
            pop_bit(pieces, square);
        }
    }

    // Filter out moves that leave the king in check
    if (check_king) {
        std::vector<Move> legal_moves;
        for (const Move& move : moves) {
            Position new_pos = this->move(move);
            if (!new_pos.isCheck()) {
                legal_moves.push_back(move);
            }
        }
        return legal_moves;
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
