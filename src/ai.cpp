#include "ai.hpp"

#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <vector>

#include "move.hpp"
#include "position.hpp"

void AI::orderMoves(std::vector<Move> &moves, Position &node) {
    auto isCaptureMove = [&](const Move &m) { return node.board[m.j] != '.'; };

    std::sort(moves.begin(), moves.end(), [&](const Move &m1, const Move &m2) {
        return isCaptureMove(m1) && !isCaptureMove(m2);
    });

    std::sort(moves.begin(), moves.end(), [&](const Move &m1, const Move &m2) {
        return node.value(m1) > node.value(m2);
    });
}

int AI::negamax(
    Position &node, int depth, int alpha, int beta, bool checkmate) {
    if (depth <= 0 || checkmate) {
        return node.value();
    }

    std::vector<Move> moves = node.genMoves();
    orderMoves(moves, node);

    int best_value = INT_MIN;

    for (Move move : moves) {
        Position new_board = node.move(move);
        checkmate          = new_board.isCheckmate() || checkmate;

        int value = -negamax(new_board, depth - 1, -beta, -alpha, checkmate);

        best_value = std::max(best_value, value);
        alpha      = std::max(alpha, best_value);

        if (alpha >= beta) {
            break;
        }
    }

    return best_value;
}

Move AI::findBestMove(Position &node, int depth) {
    std::cout << "joe" << std::endl;

    std::vector<Move> moves = node.genMoves();
    // AI::orderMoves(moves, node);

    Move bestMove;
    bestMove.value = INT_MIN;
    std::cout << moves.size() << std::endl;
    int move_number = 1;
    for (auto move : moves) {
        std::cout << "[BOT]   "
                  << "searching move #" << move_number << " at depth " << depth
                  << std::endl;
        move_number++;

        Position new_node = node.move(move);
        if (new_node.isCheckmate()) {
            std::cout << "found mate" << std::endl;
            return move;
        }

        move.value += -negamax(node, depth - 1, INT_MIN, INT_MAX, false);

        if (move.value > bestMove.value) {
            bestMove = move;
        }
    }

    return bestMove;
}
