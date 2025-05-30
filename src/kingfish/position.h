#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include <stack>
#include <string>
#include <vector>

#include "move.h"
#include "piece.h"
#include "types.h"
#include "magic.h"

struct Status {
    Color turn;
    Move  last_move;
    i64   zobrist_hash;
};

class Position { // Uses 40 bytes
  public:
    Bitboard piece_bitboards[CL_COUNT][PT_COUNT] = {};
    Bitboard occupied_bitboards[CL_COUNT]        = {};
    Bitboard all_pieces = 0ULL;  // Combined bitboard of all pieces

    CastlingRightsMask wc_rights;
    CastlingRightsMask bc_rights;

    Color turn = CL_WHITE;

    std::string board; // 120 char representation of the board
    int         score; // the board evaluation score

    std::pair<bool, bool>
        wc; // the castling rights, [west/queen side, east/king side]
    std::pair<bool, bool>
        bc; // the opponent rights, [west/king side, east/queen side]
    int ep; // the en passant square
    int kp; // the king passant square

    Position(const std::string    &board,
             int                   score,
             std::pair<bool, bool> wc,
             std::pair<bool, bool> bc,
             int                   ep,
             int                   kp)
        : board(board)
        , score(score)
        , wc(wc)
        , bc(bc)
        , ep(ep)
        , kp(kp) {
        update_bitboards();
    }

    inline bool operator==(const Position &other) const {
        return board == other.board && score == other.score && wc == other.wc &&
               bc == other.bc && ep == other.ep && kp == other.kp;
    }
    inline bool operator>(const Position &other) const {
        if (score > other.score) {
            return true;
        } else if (score == other.score) {
            return board > other.board;
        } else {
            return false;
        }
    }
    inline bool operator<(const Position &other) const {
        if (score < other.score) {
            return true;
        } else if (score == other.score) {
            return board < other.board;
        } else {
            return false;
        }
    }

    std::vector<Move> genMoves(bool check_king = true) const;
    std::vector<Move> genPawnMoves(Square square) const;
    std::vector<Move> genKnightMoves(Square square) const;
    std::vector<Move> genBishopMoves(Square square) const;
    std::vector<Move> genRookMoves(Square square) const;
    std::vector<Move> genQueenMoves(Square square) const;
    std::vector<Move> genKingMoves(Square square) const;

    i8 getPieceAt(Square square) const;
    bool  hasPieceAt(Square square) const;
    void  popPieceAt(Square square);
    void  setPieceAt(Square square, i8 piece);

    bool isCheckmate() const;
    bool isCheck() const;
    bool isValidMove(const Move &move) const;

    Position move(const Move &move) const;
    Position rotate(bool nullmove = false) const;

    int          value(const Move &move) const;
    int          value() const;
    PositionHash hash() const;

    void printBBoards() const;

  private:
    std::stack<Status> status_history;
    
    // Update all bitboards based on the board string
    void update_bitboards();
    
    // Get attack bitboard for a piece
    Bitboard get_attacks(Square square, i8 piece) const;
    
    // Get all attacks for a side
    Bitboard get_side_attacks(Color side) const;
    
    // Check if a square is under attack
    bool is_square_attacked(Square square, Color by_side) const;
};

#endif // !POSITION_H_INCLUDED
