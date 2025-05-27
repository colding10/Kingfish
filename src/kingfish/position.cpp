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

// Convert between 120-square and 64-square representations
inline Square to_64(Square square) {
    return (square / 10 - 2) * 8 + (square % 10 - 1);
}

inline Square to_120(Square square) {
    return (square / 8 + 2) * 10 + (square % 8 + 1);
}

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

i8 Position::getPieceAt(Square square) const {
    if (!this->hasPieceAt(square)) {
        return NO_PIECE;
    }

    char piece = board[square];
    if (piece == '.' || piece == ' ') {
        return NO_PIECE;
    }

    Color color = std::isupper(piece) ? CL_WHITE : CL_BLACK;
    PieceType type;
    switch (std::toupper(piece)) {
        case 'P': type = PT_PAWN; break;
        case 'N': type = PT_KNIGHT; break;
        case 'B': type = PT_BISHOP; break;
        case 'R': type = PT_ROOK; break;
        case 'Q': type = PT_QUEEN; break;
        case 'K': type = PT_KING; break;
        default: return NO_PIECE;
    }

    return make_piece(color, type);
}

bool Position::hasPieceAt(Square square) const {
    char piece = board[square];
    return piece != '.' && piece != ' ';
}

void Position::setPieceAt(Square square, i8 piece) {
    this->popPieceAt(square);
    if (piece != NO_PIECE) {
        Color color = piece_color(piece);
        PieceType type = piece_type(piece);
        char piece_char;
        switch (type) {
            case PT_PAWN: piece_char = 'P'; break;
            case PT_KNIGHT: piece_char = 'N'; break;
            case PT_BISHOP: piece_char = 'B'; break;
            case PT_ROOK: piece_char = 'R'; break;
            case PT_QUEEN: piece_char = 'Q'; break;
            case PT_KING: piece_char = 'K'; break;
            default: return;
        }
        if (color == CL_BLACK) {
            piece_char = std::tolower(piece_char);
        }
        board[square] = piece_char;
        set_bit(this->piece_bitboards[color][type], to_64(square));
    }
}

void Position::popPieceAt(Square square) {
    char piece = board[square];
    if (piece != '.' && piece != ' ') {
        Color color = std::isupper(piece) ? CL_WHITE : CL_BLACK;
        PieceType type;
        switch (std::toupper(piece)) {
            case 'P': type = PT_PAWN; break;
            case 'N': type = PT_KNIGHT; break;
            case 'B': type = PT_BISHOP; break;
            case 'R': type = PT_ROOK; break;
            case 'Q': type = PT_QUEEN; break;
            case 'K': type = PT_KING; break;
            default: return;
        }
        pop_bit(this->piece_bitboards[color][type], to_64(square));
        board[square] = '.';
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
    for (Square square = A8; square <= H1; square++) {
        if (square % 10 == 0 || square % 10 == 9) continue; // Skip padding
        i8 piece = getPieceAt(square);
        if (piece != NO_PIECE) {
            Color color = piece_color(piece);
            PieceType type = piece_type(piece);
            set_bit(piece_bitboards[color][type], to_64(square));
            set_bit(occupied_bitboards[color], to_64(square));
            set_bit(all_pieces, to_64(square));
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
    Square square_64 = to_64(square);
    Bitboard pawn = piece_bitboards[color][PT_PAWN] & (1ULL << square_64);
    if (!pawn) return moves;

    // Forward moves
    Bitboard forward = color == CL_WHITE ? (pawn << 8) : (pawn >> 8);
    forward &= ~all_pieces;
    if (forward) {
        Square target_64 = BBS::get_ls1b_index(forward);
        Square target = to_120(target_64);
        if (target / 10 == (color == CL_WHITE ? 9 : 2)) {
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
    if (color == CL_WHITE && square / 10 == 3) {
        Bitboard double_push = pawn << 16;
        double_push &= ~all_pieces;
        if (double_push) {
            Square target_64 = BBS::get_ls1b_index(double_push);
            moves.push_back(Move(square, to_120(target_64), ' '));
        }
    } else if (color == CL_BLACK && square / 10 == 8) {
        Bitboard double_push = pawn >> 16;
        double_push &= ~all_pieces;
        if (double_push) {
            Square target_64 = BBS::get_ls1b_index(double_push);
            moves.push_back(Move(square, to_120(target_64), ' '));
        }
    }

    // Captures
    Bitboard attacks = BBS::pawnAttacks(color, square_64);
    attacks &= occupied_bitboards[!color];
    while (attacks) {
        Square target_64 = BBS::get_ls1b_index(attacks);
        Square target = to_120(target_64);
        if (target / 10 == (color == CL_WHITE ? 9 : 2)) {
            // Promotion captures
            moves.push_back(Move(square, target, 'Q'));
            moves.push_back(Move(square, target, 'R'));
            moves.push_back(Move(square, target, 'B'));
            moves.push_back(Move(square, target, 'N'));
        } else {
            moves.push_back(Move(square, target, ' '));
        }
        pop_bit(attacks, target_64);
    }

    // En passant
    if (ep != -1) {
        Bitboard ep_attacks = BBS::pawnAttacks(color, square_64) & (1ULL << to_64(ep));
        if (ep_attacks) {
            moves.push_back(Move(square, ep, ' '));
        }
    }

    return moves;
}

std::vector<Move> Position::genKnightMoves(Square square) const {
    std::vector<Move> moves;
    Square square_64 = to_64(square);
    Bitboard knight = piece_bitboards[turn][PT_KNIGHT] & (1ULL << square_64);
    if (!knight) return moves;

    Bitboard attacks = BBS::knightAttacks(square_64);
    attacks &= ~occupied_bitboards[turn];
    while (attacks) {
        Square target_64 = BBS::get_ls1b_index(attacks);
        moves.push_back(Move(square, to_120(target_64), ' '));
        pop_bit(attacks, target_64);
    }
    return moves;
}

std::vector<Move> Position::genBishopMoves(Square square) const {
    std::vector<Move> moves;
    Square square_64 = to_64(square);
    Bitboard bishop = piece_bitboards[turn][PT_BISHOP] & (1ULL << square_64);
    if (!bishop) return moves;

    Bitboard attacks = Magic::get_bishop_attacks(square_64, all_pieces);
    attacks &= ~occupied_bitboards[turn];
    while (attacks) {
        Square target_64 = BBS::get_ls1b_index(attacks);
        moves.push_back(Move(square, to_120(target_64), ' '));
        pop_bit(attacks, target_64);
    }
    return moves;
}

std::vector<Move> Position::genRookMoves(Square square) const {
    std::vector<Move> moves;
    Square square_64 = to_64(square);
    Bitboard rook = piece_bitboards[turn][PT_ROOK] & (1ULL << square_64);
    if (!rook) return moves;

    Bitboard attacks = Magic::get_rook_attacks(square_64, all_pieces);
    attacks &= ~occupied_bitboards[turn];
    while (attacks) {
        Square target_64 = BBS::get_ls1b_index(attacks);
        moves.push_back(Move(square, to_120(target_64), ' '));
        pop_bit(attacks, target_64);
    }
    return moves;
}

std::vector<Move> Position::genQueenMoves(Square square) const {
    std::vector<Move> moves;
    Square square_64 = to_64(square);
    Bitboard queen = piece_bitboards[turn][PT_QUEEN] & (1ULL << square_64);
    if (!queen) return moves;

    Bitboard attacks = Magic::get_queen_attacks(square_64, all_pieces);
    attacks &= ~occupied_bitboards[turn];
    while (attacks) {
        Square target_64 = BBS::get_ls1b_index(attacks);
        moves.push_back(Move(square, to_120(target_64), ' '));
        pop_bit(attacks, target_64);
    }
    return moves;
}

std::vector<Move> Position::genKingMoves(Square square) const {
    std::vector<Move> moves;
    Square square_64 = to_64(square);
    Bitboard king = piece_bitboards[turn][PT_KING] & (1ULL << square_64);
    if (!king) return moves;

    Bitboard attacks = BBS::kingAttacks(square_64);
    attacks &= ~occupied_bitboards[turn];
    while (attacks) {
        Square target_64 = BBS::get_ls1b_index(attacks);
        moves.push_back(Move(square, to_120(target_64), ' '));
        pop_bit(attacks, target_64);
    }

    // Castling
    if (turn == CL_WHITE) {
        // Kingside castling
        if (wc.first && !(all_pieces & 0x60ULL)) {
            // Check if squares between king and rook are empty
            bool can_castle = true;
            for (int sq = 5; sq <= 6; sq++) {
                if (is_square_attacked(sq, CL_BLACK)) {
                    can_castle = false;
                    break;
                }
            }
            if (can_castle) {
                moves.push_back(Move(91, 93, ' ')); // Kingside
            }
        }
        // Queenside castling
        if (wc.second && !(all_pieces & 0xEULL)) {
            // Check if squares between king and rook are empty
            bool can_castle = true;
            for (int sq = 2; sq <= 3; sq++) {
                if (is_square_attacked(sq, CL_BLACK)) {
                    can_castle = false;
                    break;
                }
            }
            if (can_castle) {
                moves.push_back(Move(91, 89, ' ')); // Queenside
            }
        }
    } else {
        // Kingside castling
        if (bc.first && !(all_pieces & 0x6000000000000000ULL)) {
            // Check if squares between king and rook are empty
            bool can_castle = true;
            for (int sq = 61; sq <= 62; sq++) {
                if (is_square_attacked(sq, CL_WHITE)) {
                    can_castle = false;
                    break;
                }
            }
            if (can_castle) {
                moves.push_back(Move(21, 23, ' ')); // Kingside
            }
        }
        // Queenside castling
        if (bc.second && !(all_pieces & 0x0E00000000000000ULL)) {
            // Check if squares between king and rook are empty
            bool can_castle = true;
            for (int sq = 58; sq <= 59; sq++) {
                if (is_square_attacked(sq, CL_WHITE)) {
                    can_castle = false;
                    break;
                }
            }
            if (can_castle) {
                moves.push_back(Move(21, 19, ' ')); // Queenside
            }
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
            Square square_64 = BBS::get_ls1b_index(pieces);
            Square square = to_120(square_64);
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
            pop_bit(pieces, square_64);
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
    // First validate that this is a legal move
    std::vector<Move> legal_moves = this->genMoves(true);
    if (std::find(legal_moves.begin(), legal_moves.end(), move) == legal_moves.end()) {
        // If move is not legal, return the same position
        return *this;
    }

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
