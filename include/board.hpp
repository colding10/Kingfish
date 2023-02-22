#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED

#include <stack>
#include <string>
#include <vector>

#include "location.hpp"
#include "move.hpp"
#include "pieces.hpp"

const std::string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
// const std::string STARTING_FEN = "4k3/8/3q4/8/8/6q1/q7/4K3";

class Board {
   private:
    PieceColor active_color = WHITE;
    std::stack<Move> moveStack;

    float move_number = 1.0f;

    bool is_reversed = false;

    Location selected_indices = {-1, -1};

   public:
    Board(const std::string& fen) { this->readFen(fen);}

    int board[8][8];

    PieceColor checkmated_color = NOCOLOR;

    float evaluateBoard(int color);
    std::vector<Move> getLegalMoves(PieceColor color);

    float getMoveNumber();
    void incrementMoveNumber();

    PieceColor getActiveColor();
    void setActiveColor(PieceColor color);
    void toggleActiveColor();

    Location getSelectedLocation();
    int getPieceAt(Location location);

    void setSelectedPiece(int i, int j);
    bool hasSelectedPiece();
    void clearSelectedPiece();

    void tryMove(Move move);
    void makeMove(Move move, bool push_to_stack = true);
    void undoLastMove();

    void makeNullMove();

    void readFen(std::string fen);
    std::string getFen();

    void printBoard();
    void printEvaluation();

    void reverse();
    bool isReversed();

    bool isInCheck(PieceColor c);
    bool isCheckmate(PieceColor c);

    int hash();
};

#endif  // !BOARD_HPP_INCLUDED