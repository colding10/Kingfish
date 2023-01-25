#include "ai.hpp"

#include <iostream>
#include <vector>

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


int AI::negamax(Board* node, int depth, PieceColor color, int alpha, int beta) {
    if (depth == 0 || Game::isInCheckMate(node, WHITE) || Game::isInCheckMate(node, BLACK)) {
        return node->evaluateBoard(color);
    }

    std::vector<Move> moves = node->getAllMoves(color);
    AI::orderMoves(moves, node);

    int bestValue = INT_MIN;

    for (Move move : moves) {
        node->makeMove(move);

        int value = -negamax(node, depth - 1, color == WHITE ? BLACK : WHITE, -beta, -alpha);
        node->undoMove();

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

        move.value = -negamax(node, depth - 1, color == WHITE ? BLACK : WHITE, INT_MIN, INT_MAX);
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