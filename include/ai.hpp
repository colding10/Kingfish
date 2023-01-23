#ifndef AI_HPP_INCLUDED
#define AI_HPP_INCLUDED

#include "board.hpp"
#include "move.hpp"
#include "pieces.hpp"

namespace AI {
    int negamax(Board* node, int depth, PieceColor color, int alpha, int beta);
    Move findBestMove(Board* node, int depth, PieceColor color);
}  // namespace AI

#endif  // !AI_HPP_INCLUDED