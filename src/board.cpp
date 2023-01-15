#include "board.hpp"

#include <cctype>
#include <iostream>
#include <string>

#include "pieces.hpp"

// FIXME: fix fen reading
void Board::readFen(std::string fen) {
    int row = 0;
    int col = 0;

    for (char c : fen) {
        if (c == '/') {
            row++;
            col = 0;
            continue;
        } else {
            if (isdigit(c)) {
                for (int _ = 0; _ < static_cast<int>(c - '0'); _++) {
                    this->board[row][col] = 0x00;
                    col++;
                }
            } else {
                piece_color color = isupper(c) ? WHITE : BLACK;
                Piece p;

                c = tolower(c);

                switch (c) {
                    case 'p':
                        p = makePiece(PAWN, color);
                        break;
                    case 'r':
                        p = makePiece(ROOK, color);
                        break;
                    case 'n':
                        p = makePiece(KNIGHT, color);
                        break;
                    case 'b':
                        p = makePiece(BISHOP, color);
                        break;
                    case 'q':
                        p = makePiece(QUEEN, color);
                        break;
                    case 'k':
                        p = makePiece(KING, color);
                        break;
                    default:
                        p = makePiece(PAWN, color);
                }

                this->board[row][col] = p;
                col++;
            }
        }
    }
}

void Board::printBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << this->board[i][j] << std::string((board[i][j] >= 10 ? 1 : 2), ' ');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}