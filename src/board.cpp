#include "board.hpp"

#include <cctype>
#include <string>

#include "pieces.hpp"

void Board::readFen(std::string fen) {
    int row = 0;
    int col = 0;

    bool reading_board = true;

    for (char c : fen) {
        if (reading_board) {
            if (c == '/') {
                row++;
                continue;
            } else {
                if (isdigit(c)) {
                    for (int _ = 0; _ < static_cast<int>(c); _++) {
                        this->board[row][col] = 0x00;
                        col++;
                    }
                } else {
                    bool is_white = isupper(c);
                    c = tolower(c);
                    piece p;

                    switch (c) {
                        case 'p':
                            p = 
                    }
                }
            }
        }
    }
}