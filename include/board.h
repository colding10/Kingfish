#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <string>


class Board {
   private:
    int color;
    int move_number;

    int board[8][8];

   public:
    Board() {
        return;
    }

    void readFen(std::string fen);
    

};

#endif  // !BOARD_H_INCLUDED