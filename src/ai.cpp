#include "ai.hpp"

#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "board.hpp"
#include "move.hpp"
#include "pieces.hpp"
#include "transtable.hpp"

bool isCaptureMove(Move move, Board *node) {
    return node->getPieceAt(move.getEnding());
}

void AI::orderMoves(std::vector<Move> &moves, Board *node) {
    std::sort(moves.begin(), moves.end(), [=](const Move &m1, const Move &m2) {
        return isCaptureMove(m1, node) && !isCaptureMove(m2, node);
    });
}

int AI::quiesce(Board *node, PieceColor color, int alpha, int beta,
                bool checkmate, TranspositionTable &transpositionTable,
                int depth) {
    int stand_pat = node->evaluateBoard(color);
    int score;

    if (depth <= 0 || checkmate || stand_pat >= beta) {
        return stand_pat;
    }

    if (alpha < stand_pat) {
        alpha = stand_pat;
    }

    std::vector<Move> moves = node->getLegalMoves(color);

    orderMoves(moves, node);

    for (Move move : moves) {
        if (!move.getCaptured()) {
            continue;
        }

        node->makeMove(move);

        if (node->isCheckmate(color)) {
            node->undoLastMove();
            return INT_MAX - depth;
        }

        score = -quiesce(node, Pieces::oppositeColor(color), -beta, -alpha, false,
                         transpositionTable, depth - 1);
        node->undoLastMove();

        if (score >= beta) {
            return beta;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

int AI::negamax(Board *node, PieceColor color, int depth, int alpha, int beta,
                TranspositionTable &transpositionTable, bool checkmate) {
    int original_alpha = alpha;

    TranspositionTable::Entry entry;
    // if (transpositionTable.get(node->hash(), entry)) {
    //     if (entry.depth >= depth) {
    //         if (entry.flag == EXACT) {
    //             return entry.value;
    //         } else if (entry.flag == LOWERBOUND) {
    //             alpha = std::max(alpha, entry.value);
    //         } else if (entry.flag == UPPERBOUND) {
    //             beta = std::min(beta, entry.value);
    //         }

    //         if (alpha >= beta) {
    //             return entry.value;
    //         }
    //     }
    // }

    if (depth <= 0) {
        std::cout << "eval: " << node->evaluateBoard(color) << " qui: " << AI::quiesce(node, color, alpha, beta, checkmate, transpositionTable, depth) << std::endl;
        // return node->evaluateBoard(color);
        return AI::quiesce(node, color, alpha, beta, checkmate, transpositionTable, depth);
    }
    if (checkmate) {
        return node->evaluateBoard(color);
    }

    std::vector<Move> moves = node->getLegalMoves(color);
    orderMoves(moves, node);

    int best_value = INT_MIN;

    for (Move move : moves) {
        node->makeMove(move);
        if (node->isCheckmate(WHITE) || node->isCheckmate(BLACK)) {
            checkmate = true;
        }

        int value = -negamax(node, Pieces::oppositeColor(color), depth - 1, -beta, -alpha, transpositionTable, checkmate);

        node->undoLastMove();

        best_value = std::max(best_value, value);
        alpha = std::max(alpha, best_value);

        if (alpha >= beta) {
            break;
        }
    }

    entry.value = best_value;
    entry.depth = depth;
    if (best_value >= original_alpha) {
        entry.flag = UPPERBOUND;
    } else if (best_value >= beta) {
        entry.flag = LOWERBOUND;
    } else {
        entry.flag = EXACT;
    }

    transpositionTable.put(node->hash(), entry);

    return best_value;
}

Move AI::findBestMove(Board *node, PieceColor color, int depth,
                      int time_limit_ms,
                      TranspositionTable &transpositionTable) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = start_time + std::chrono::milliseconds(time_limit_ms);

    std::vector<Move> moves = node->getLegalMoves(color);
    orderMoves(moves, node);

    Move bestMove = Move(Location(0, 0), Location(0, 0), 0, 0);
    bestMove.value = INT_MIN;

    for (int d = depth; d <= depth; d++) {  // TODO: start from 1 again
        bool checkmate = false;

        std::cout << "[BOT]   "
                  << "searched to depth " << d << std::endl;

        int move_number = 1;
        for (auto move : moves) {
            std::cout << "[BOT]   "
                      << "searching move #" << move_number << " at depth " << d << std::endl;
            move_number++;

            node->makeMove(move);
            move.value += -negamax(node, Pieces::oppositeColor(color), d - 1,
                                   INT_MIN, INT_MAX, transpositionTable, checkmate);

            node->undoLastMove();

            if (move.value > bestMove.value) {
                bestMove = move;
            }

            // Stop the search if we've exceeded the time limit
            // auto current_time = std::chrono::high_resolution_clock::now();
            // if (current_time > end_time) {
            //     break;
            // }
        }
    }

    return bestMove;
}
