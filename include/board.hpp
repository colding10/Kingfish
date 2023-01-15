#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <string>

const std::string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

class Board {
   private:
    int color;
    int move_number;

   public:
    int board[8][8];
    std::pair<int, int> selected_piece;

    void readFen(std::string fen);

    void printBoard();
};

#endif  // !BOARD_H_INCLUDED