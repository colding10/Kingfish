#ifndef AI_HPP_INCLUDED
#define AI_HPP_INCLUDED

#include "board.hpp"
#include "move.hpp"
#include "pieces.hpp"
#include "transtable.hpp"

const unsigned int NULL_MOVE_DEPTH = 4;

namespace AI {
    int negamax(Board *node, PieceColor color, int depth, int alpha, int beta,
                TranspositionTable &transpositionTable, bool checkmate);
    int quiesce(Board *node, PieceColor color, int alpha, int beta, bool checkmate,
                TranspositionTable &transpositionTable, int depth);

    void orderMoves(std::vector<Move> &moves, Board *node);
    Move findBestMove(Board *node, PieceColor color, int depth, int timeLimit,
                      TranspositionTable &transpositionTable);

    Move findBestMove(Board* node, int depth, PieceColor color);
    int negamax(Board* node, int depth, int alpha, int beta, PieceColor color);

}  // namespace AI

#endif  // !AI_HPP_INCLUDED