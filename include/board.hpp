#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <string>
#include "defines.hpp"
#include "pieces.hpp"

const std::string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

class Board {
   private:
    PieceColor active_color = WHITE;
    float move_number = 1.0f;

    bool is_reversed = false;

    Location selected_piece = {-1, -1};

   public:
    int board[8][8];

    float getMoveNumber();
    void incrementMoveNumber();

    PieceColor getActiveColor();
    void toggleActiveColor();

    Location getSelectedPiece();
    int getPieceAt(Location location);

    void setSelectedPiece(int i, int j);
    bool hasSelectedPiece();
    void clearSelectedPiece();


    void tryMove(Location starting, Location ending);
    void makeMove(Location starting, Location ending);


    void readFen(std::string fen);
    void printBoard();

    void reverse();
    bool isReversed();
};

#endif  // !BOARD_H_INCLUDED