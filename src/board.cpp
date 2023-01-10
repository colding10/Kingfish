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
                col = 0;
                continue;
            } else {
                if (isdigit(c)) {
                    for (int _ = 0; _ < static_cast<int>(c); _++) {
                        this->board[row][col] = 0x00;
                        col++;
                    }
                } else {
                    piece_color color = isupper(c) ? WHITE : BLACK;
                    piece p;

                    c = tolower(c);

                    switch (c) {
                        case 'p':
                            p = makePiece(PAWN, color);
                        case 'r':
                            p = makePiece(ROOK, color);
                        case 'n':
                            p = makePiece(KNIGHT, color);
                        case 'b':
                            p = makePiece(BISHOP, color);
                        case 'q':
                            p = makePiece(QUEEN, color);
                        case 'k':
                            p = makePiece(KING, color);
                    }

                    this->board[row][col] = p;
                    col++;
                }
            }
        }
    }
}

