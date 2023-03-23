#ifndef POSITION_HPP_INCLUDED
#define POSITION_HPP_INCLUDED

#include <string>
#include <vector>

#include "move.hpp"
#include "pieces.hpp"

typedef int PositionHash;

class Position {
   public:
    std::string board;  // 120 char representation of the board
    int score;          // the board evaluation score
    std::pair<bool, bool>
        wc;  // the castling rights, [west/queen side, east/king side]
    std::pair<bool, bool>
        bc;  // the opponent rights, [west/king side, east/queen side]
    int ep;  // the en passant square
    int kp;  // the king passant square

    Position(std::string board, int score, std::pair<bool, bool> wc,
             std::pair<bool, bool> bc, int ep, int kp)
        : board(board), score(score), wc(wc), bc(bc), ep(ep), kp(kp) {}
    inline bool operator==(Position other) const {
        return board == other.board && score == other.score && wc == other.wc &&
               bc == other.bc && ep == other.ep && kp == other.kp;
    }
    inline bool operator>(const Position other) const { return false; }
    inline bool operator<(const Position &other) const { return false; }
    std::vector<Move> genMoves();

    Position rotate(bool nullmove = false);
    Position move(Move move);

    int value(Move move);
    int hash();
};

#endif  // !POSITION_HPP_INCLUDED