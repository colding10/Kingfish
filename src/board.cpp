#include "board.hpp"

#include <cctype>
#include <iostream>
#include <string>

#include "defines.hpp"
#include "game.hpp"
#include "pieces.hpp"

// TODO: add other FEN fields
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
                PieceColor color = isupper(c) ? WHITE : BLACK;
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

void Board::reverse() {
    int board[8][8];

    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            board[7 - i][j] = this->board[i][j];
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->board[i][j] = board[i][j];
        }
    }

    this->is_reversed = !this->is_reversed;
}

bool Board::isReversed() {
    return this->is_reversed;
}

void Board::tryMove(Location starting, Location ending) {
    if (isValidMove(this, starting, ending, true)) {
        this->makeMove(starting, ending);
        this->clearSelectedPiece();
    }
}

void Board::makeMove(Location starting, Location ending) {
    int r1, c1, r2, c2;

    r1 = starting.first;
    c1 = starting.second;

    r2 = ending.first;
    c2 = ending.second;

    this->board[r2][c2] = this->board[r1][c1];
    this->board[r1][c1] = 0x00;
}

int Board::getPieceAt(Location location) {
    return this->board[location.first][location.second];
}

Location Board::getSelectedPiece() {
    return this->selected_piece;
}

void Board::setSelectedPiece(int i, int j) {
    this->selected_piece = makeLocation(i, j);
}

void Board::clearSelectedPiece() {
    this->selected_piece = makeLocation(-1, -1);
}

bool Board::hasSelectedPiece() {
    return (this->selected_piece.first != -1 && this->selected_piece.second != -1);
}