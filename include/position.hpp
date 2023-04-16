#ifndef POSITION_HPP_INCLUDED
#define POSITION_HPP_INCLUDED

#include <string>
#include <vector>

#include "move.hpp"
#include "pieces.hpp"
#include "types.hpp"

class Position {
  public:
    std::string board; // 120 char representation of the board
    int         score; // the board evaluation score
    std::pair<bool, bool>
        wc;            // the castling rights, [west/queen side, east/king side]
    std::pair<bool, bool>
        bc;            // the opponent rights, [west/king side, east/queen side]
    int ep;            // the en passant square
    int kp;            // the king passant square

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

    bool isCheckmate();
    bool isCheck();
    bool isValidMove(const Move &move);

    Position rotate(bool nullmove = false);
    Position move(const Move &move);

    int value(const Move &move);
    int value();
    int hash();
};

#endif // !POSITION_HPP_INCLUDED