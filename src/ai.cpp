#include "ai.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include "board.hpp"
#include "game.hpp"
#include "move.hpp"
#include "pieces.hpp"

/*
function negamax(node, depth, color) is
    if depth = 0 or node is a terminal node then
        return color × the heuristic value of node
    value := −∞
    for each child of node do
        value := max(value, −negamax(child, depth − 1, −color))
    return value
 */

bool isCaptureMove(Move move, Board* node) {
    return node->getPieceAt({move.endX, move.endY});
}

void AI::orderMoves(std::vector<Move>& moves, Board* node) {
    std::sort(moves.begin(), moves.end(), [=](const Move& m1, const Move& m2) {
        return isCaptureMove(m1, node) && !isCaptureMove(m2, node);
    });
}

int AI::quiesce(Board* node, PieceColor color, int alpha, int beta, bool checkmate) {
    int stand_pat = node->evaluateBoard(color);
    int score;

    if (checkmate || stand_pat >= beta) {
        return stand_pat;
    }
    if (alpha < stand_pat) {
        alpha = stand_pat;
    }

    std::vector<Move> moves = node->getAllMoves(color);

    for (Move move : moves) {
        if (!isCaptureMove(move, node)) {
            continue;
        }

        int b[8][8];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                b[i][j] = node->board[i][j];
            }
        }
        node->makeMove(move);
        if (Game::isInCheckMate(node, color)) {
            checkmate = true;
        }
        score = -AI::quiesce(node, color == WHITE ? BLACK : WHITE, -beta, -alpha, checkmate);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                node->board[i][j] = b[i][j];
            }
        }

        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

int AI::negamax(Board* node, int depth, PieceColor color, int alpha, int beta, bool checkmate) {
    if (depth == 0) {
        std::cout << "qui: " << AI::quiesce(node, color, alpha, beta, checkmate) << " eval: " << node->evaluateBoard(color) << std::endl;
        // return AI::quiesce(node, color, alpha, beta, checkmate);
        return node->evaluateBoard(color);
    }
    if (checkmate) {
        return node->evaluateBoard(color);
    }

    std::vector<Move> moves = node->getAllMoves(color);
    AI::orderMoves(moves, node);

    int bestValue = INT_MIN;

    for (Move move : moves) {
        int b[8][8];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                b[i][j] = node->board[i][j];
            }
        }
        node->makeMove(move);
        if (Game::isInCheckMate(node, WHITE) || Game::isInCheckMate(node, BLACK)) {
            checkmate = true;
        }

        int value = -negamax(node, depth - 1, color == WHITE ? BLACK : WHITE, -beta, -alpha, checkmate);

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                node->board[i][j] = b[i][j];
            }
        }

        bestValue = std::max(bestValue, value);
        alpha = std::max(alpha, value);

        if (alpha >= beta) {
            break;
        }
    }

    return bestValue;
}

Move AI::findBestMove(Board* node, int depth, PieceColor color) {
    std::vector<Move> moves = node->getAllMoves(color);

    Move bestMove;
    bestMove.value = INT_MIN;

    for (Move move : moves) {
        int b[8][8];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                b[i][j] = node->board[i][j];
            }
        }
        node->makeMove(move);
        if (Game::isInCheckMate(node, color == WHITE ? BLACK : WHITE)) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    node->board[i][j] = b[i][j];
                }
            }
            std::cout << "found mate" << std::endl;
            return move;
        }
        move.value += -negamax(node, depth - 1, color == WHITE ? BLACK : WHITE, INT_MIN, INT_MAX, false);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                node->board[i][j] = b[i][j];
            }
        }

        if (move.value > bestMove.value) {
            bestMove = move;
        }
    }

    return bestMove;
}