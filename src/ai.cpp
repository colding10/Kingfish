#include "ai.hpp"

#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "board.hpp"
#include "game.hpp"
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
                TranspositionTable &transpositionTable, bool allowNullMove) {
    bool checkmate = node->isCheckmate(color);
    if (checkmate) {
        return INT_MIN + depth;
    }

    int hashValue;
    if (transpositionTable.get(node->hash(), depth, alpha, beta, hashValue)) {
        return hashValue;
    }

    if (depth <= 0) {
        // return node->evaluateBoard(color);
        return AI::quiesce(node, color, alpha, beta, checkmate, transpositionTable,
                           depth);
    }

    int bestValue = INT_MIN;
    bool foundPV = false;
    int bestMoveIndex = -1;
    std::vector<Move> moves = node->getLegalMoves(color);
    orderMoves(moves, node);

    // Null move pruning
    if (!allowNullMove && moves.size() > 0 && !isCaptureMove(moves[0], node)) {
        node->makeNullMove();
        int value =
            -negamax(node, Pieces::oppositeColor(color), depth - 1 - NULL_MOVE_DEPTH,
                     -beta, -beta + 1, transpositionTable, false);
        node->undoLastMove();

        if (value >= beta) {
            return beta;
        }
    }

    // Iterative deepening
    int numMovesSearched = 0;
    for (int i = 0; i < (int)moves.size(); i++) {
        Move move = moves[i];

        node->makeMove(move);

        if (!node->isInCheck(color)) {
            int value;
            if (!foundPV) {
                value = -negamax(node, Pieces::oppositeColor(color), depth - 1, -beta,
                                 -alpha, transpositionTable, true);
            } else {
                value = -negamax(node, Pieces::oppositeColor(color), depth - 1,
                                 -alpha - 1, -alpha, transpositionTable, true);
                if (value > alpha && value < beta) {
                    value = -negamax(node, Pieces::oppositeColor(color), depth - 1, -beta,
                                     -alpha, transpositionTable, true);
                }
            }

            node->undoLastMove();

            if (value > bestValue) {
                bestValue = value;
                bestMoveIndex = i;
            }

            if (value > alpha) {
                foundPV = true;
                alpha = value;
                if (alpha >= beta) {
                    break;
                }
            }
        } else {
            node->undoLastMove();
        }

        ++numMovesSearched;
    }

    // Store the best move in the transposition table
    if (bestMoveIndex != -1) {
        transpositionTable.put(node->hash(), depth, bestValue, alpha, beta);
    }

    return bestValue;
}

Move AI::findBestMove(Board *node, PieceColor color, int depth,
                      int time_limit_ms,
                      TranspositionTable &transpositionTable) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = start_time + std::chrono::milliseconds(time_limit_ms);
    int alpha = INT_MIN;
    int beta = INT_MAX;
    int value;

    std::vector<Move> moves = node->getLegalMoves(color);
    orderMoves(moves, node);

    Move bestMove = moves[0];

    for (int current_depth = 1; current_depth <= depth; current_depth++) {
        for (auto move : moves) {
            node->makeMove(move);
            value = -negamax(node, Pieces::oppositeColor(color), current_depth - 1,
                             -beta, -alpha, transpositionTable,
                             false);  // TODO: check if null should be ok
            node->undoLastMove();

            if (value >= beta) {
                bestMove = move;
                return bestMove;
            }

            if (value > alpha) {
                alpha = value;
                bestMove = move;
            }

            if (std::chrono::high_resolution_clock::now() >= end_time) {
                return bestMove;
            }
        }
    }

    return bestMove;
}