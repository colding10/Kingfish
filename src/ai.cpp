#include "ai.hpp"

#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <map>
#include <vector>

#include "board.hpp"
#include "move.hpp"
#include "pieces.hpp"
#include "transtable.hpp"
#include "uci.cpp"

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

        int score = -quiesce(node, Pieces::oppositeColor(color), -beta, -alpha, false,
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
                TranspositionTable &transpositionTable, bool checkmate, bool use_quiescence, bool use_transtable) {
    int original_alpha = alpha;

    if (use_transtable) {
        TranspositionTable::Entry entry;
        if (transpositionTable.get(node->hash(), entry)) {
            if (entry.depth >= depth) {
                if (entry.flag == EXACT) {
                    return entry.value;
                } else if (entry.flag == LOWERBOUND) {
                    alpha = std::max(alpha, entry.value);
                } else if (entry.flag == UPPERBOUND) {
                    beta = std::min(beta, entry.value);
                }

                if (alpha >= beta) {
                    return entry.value;
                }
            }
        }
    }

    if (depth <= 0) {
        std::cout << "eval: " << node->evaluateBoard(color) << " qui: " << AI::quiesce(node, color, alpha, beta, checkmate, transpositionTable, depth) << std::endl;
        if (use_quiescence) {
            return AI::quiesce(node, color, alpha, beta, checkmate, transpositionTable, depth);
        } else {
            return node->evaluateBoard(color);
        }
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

        int value = -negamax(node, Pieces::oppositeColor(color), depth - 1, -beta, -alpha, transpositionTable, checkmate, use_quiescence, use_transtable);

        node->undoLastMove();

        best_value = std::max(best_value, value);
        alpha = std::max(alpha, best_value);

        if (alpha >= beta) {
            break;
        }
    }

    if (use_transtable) {
        TranspositionTable::Entry entry;
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
    }

    return best_value;
}

Move AI::findBestMove(Board *node, PieceColor color, int depth,
                      int time_limit_ms,
                      TranspositionTable &transpositionTable) {
    // auto start_time = std::chrono::high_resolution_clock::now();
    // auto end_time = start_time + std::chrono::milliseconds(time_limit_ms);

    std::vector<Move> moves = node->getLegalMoves(color);
    orderMoves(moves, node);

    Move bestMove = Move(Location(0, 0), Location(0, 0), 0, 0);
    bestMove.value = INT_MIN;

    int move_number = 1;
    for (auto move : moves) {
        if (should_stop) {
            return bestMove;
        }

        std::cout << "[BOT]   "
                  << "searching move #" << move_number << " at depth " << depth << std::endl;
        move_number++;

        node->makeMove(move);
        if (node->isCheckmate(Pieces::oppositeColor(color))) {
            node->undoLastMove();

            std::cout << "found mate" << std::endl;
            return move;
        }
        move.value += -negamax(node, Pieces::oppositeColor(color), depth - 1,
                               INT_MIN, INT_MAX, transpositionTable, false, true, false);

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

    return bestMove;
}
