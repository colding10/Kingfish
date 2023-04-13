#ifndef AI_HPP_INCLUDED
#define AI_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>

#include "move.hpp"
#include "pieces.hpp"
#include "position.hpp"

namespace AI {
int negamax(Position &node, int depth, int alpha, int beta, bool checkmate);

void orderMoves(std::vector<Move> &moves, Position &node);
Move findBestMove(Position &node, int depth, int time_limit_ms);

} // namespace AI

#endif // !AI_HPP_INCLUDED