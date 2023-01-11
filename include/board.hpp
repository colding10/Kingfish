#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <string>

class Board {
   private:
    int color;
    int move_number;

   public:
    int board[8][8];
    std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

    void readFen(std::string fen);

    void printBoard();
};

#endif  // !BOARD_H_INCLUDED