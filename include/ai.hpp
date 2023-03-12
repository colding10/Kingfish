#ifndef AI_HPP_INCLUDED
#define AI_HPP_INCLUDED

#include <map>
#include <string>

#include "board.hpp"
#include "move.hpp"
#include "pieces.hpp"
#include "transtable.hpp"

const unsigned int NULL_MOVE_DEPTH = 4;

namespace AI {
    int negamax(Board *node, PieceColor color, int depth, int alpha, int beta,
                TranspositionTable &transpositionTable, bool checkmate,
                bool use_quiescence, bool use_transtable);
    int quiesce(Board *node, PieceColor color, int alpha, int beta, bool checkmate,
                TranspositionTable &transpositionTable, int depth);

    void orderMoves(std::vector<Move> &moves, Board *node);
    Move findBestMove(Board *node, PieceColor color, int depth,
                      int time_limit_ms,
                      TranspositionTable &transpositionTable);

}  // namespace AI

#endif  // !AI_HPP_INCLUDED