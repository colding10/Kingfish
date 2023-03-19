#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED

#include <stack>
#include <string>
#include <vector>

#include "location.hpp"
#include "move.hpp"
#include "pieces.hpp"

// const std::string STARTING_FEN = "4k3/8/P7/8/8/p7/PPPPPPPP/RNBQKBNR";
const std::string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

class Board {
   private:
    PieceColor active_color = PieceColor(WHITE);
    PieceColor checkmated_color = PieceColor(NOCOLOR);

    std::stack<Move> move_stack;

    float move_number = 1.0f;

    bool is_reversed = false;
    bool is_game_over = false;

    Location selected_indices = Location(-1, -1);

    Piece board[8][8];

   public:
    Board() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = 0x00;
            }
        }
    };
    Board(const std::string &fen) { this->readFen(fen); }
    const Piece *operator[](int row) const { return board[row]; }
    Piece *operator[](int row) { return board[row]; }

    float evaluateBoard(PieceColor color);
    std::vector<Move> getLegalMoves(PieceColor color);

    float getMoveNumber();
    void incrementMoveNumber();

    PieceColor getActiveColor();
    void setActiveColor(PieceColor color);
    void toggleActiveColor();

    Location getSelectedLocation();
    Piece getPieceAt(Location location);

    void setSelectedPiece(Location loc);
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
    bool checkCheckmates();
    void setCheckmate(PieceColor c);
    PieceColor getCheckmatedColor();

    bool isGameOver();
    void setGameOver();

    std::uint64_t hash();
};

#endif  // !BOARD_HPP_INCLUDED