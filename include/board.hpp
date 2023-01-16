#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <string>
#include "defines.hpp"

const std::string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

class Board {
   private:
    int color;
    int move_number;

    bool is_reversed = false;

    
    Location selected_piece = {-1, -1};

   public:
    int board[8][8];

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