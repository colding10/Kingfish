#ifndef AI_HPP_INCLUDED
#define AI_HPP_INCLUDED

#include "board.hpp"
#include "move.hpp"
#include "pieces.hpp"

namespace AI {
    int negamax(Board* node, int depth, PieceColor color, int alpha, int beta, bool checkmate);
    int quiesce(Board* node, PieceColor color, int alpha, int beta, bool checkmate);

    void orderMoves(std::vector<Move>& moves, Board* node);
    Move findBestMove(Board* node, int depth, PieceColor color);
}  // namespace AI

#endif  // !AI_HPP_INCLUDED