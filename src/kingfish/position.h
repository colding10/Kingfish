#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include <stack>
#include <string>
#include <vector>

#include "move.h"
#include "piece.h"
#include "pieces.h"
#include "types.h"
#include "bitboard.h"

struct Status {
    Move last_move;
};

class Position { // Uses 40 bytes
  public:
    Bitboard piece_bitboards[CL_COUNT][PT_COUNT] = {};
    Bitboard occupied_bitboards[CL_COUNT]        = {};

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
        , kp(kp) {}
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

    std::vector<Move> genMoves(bool check_king = true);

    Piece getPieceAt(Square square);
    bool  hasPieceAt(Square square);
    void  popPieceAt(Square square);
    void  setPieceAt(Square square, Piece p);

    bool isCheckmate();
    bool isCheck();
    bool isValidMove(const Move &move);

    Position move(const Move &move);
    Position rotate(bool nullmove = false);

    int          value(const Move &move);
    int          value();
    PositionHash hash();

    void printBBoards();

  private:
    std::stack<Status> status_history;
};

#endif // !POSITION_H_INCLUDED