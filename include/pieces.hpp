#ifndef PIECES_H_INCLUDED
#define PIECES_H_INCLUDED

#include <cctype>
#include <string>

enum PieceColorEnum { WHITE = 16,
                      BLACK = 8,
                      NOCOLOR = 0 };

enum PieceClassEnum { PAWN,
                      KNIGHT,
                      BISHOP,
                      ROOK,
                      QUEEN,
                      KING,
                      EMPTY };

class PieceColor {
   public:
    PieceColor() { this->color = NOCOLOR; }
    PieceColor(PieceColorEnum color) { this->color = color; }

    operator bool() const { return this->color != NOCOLOR; }

    PieceColorEnum getColor() { return color; }
    PieceColorEnum oppositeColor() {
        return color == WHITE ? BLACK : (color == BLACK ? WHITE : NOCOLOR);
    }
    void reverseColor() {
        this->color =
            this->color == WHITE ? BLACK : (color == BLACK ? WHITE : NOCOLOR);
    }

   private:
    PieceColorEnum color;
};

class Piece {
   public:
    Piece() {
        this->color = NOCOLOR;
        this->type = PAWN;
    }
    Piece(PieceColor color, PieceClassEnum type) {
        this->color = color;
        this->type = type;
    }

    operator int() { return toInt(); }

    operator PieceClassEnum() const { return this->type; }
    operator PieceColor() const { return this->color; }

    int toInt() { return this->color | this->type; }
    void reverseColor() { return this->color.reverseColor(); }
    PieceColorEnum oppositeColor() { return this->color.oppositeColor(); }

    // Setters
    void setColor(PieceColor color) { this->color = color; }
    void setClass(PieceClassEnum type) { this->type = type; }

    // Getters
    PieceColorEnum getColor() { return this->color.getColor(); }
    PieceClassEnum getClass() { return this->type; }

    char getFENChar() {
        char c;

        if (this->color.getColor() == NOCOLOR) {
            c = ' ';
        } else if (this->type == PAWN) {
            c = 'p';
        } else if (this->type == ROOK) {
            c = 'r';
        } else if (this->type == KNIGHT) {
            c = 'n';
        } else if (this->type == BISHOP) {
            c = 'b';
        } else if (this->type == QUEEN) {
            c = 'q';
        } else if (this->type == KING) {
            c = 'k';
        }

        if (this->color.getColor() == WHITE) {
            c = toupper(c);
        }
        return c;
    }
    std::string getFilename() {
        PieceColorEnum color = this->getColor();
        PieceClassEnum pc = this->getClass();

        std::string filename;

        switch (color) {
            case WHITE:
                filename += "white_";
                break;
            case BLACK:
                filename += "black_";
                break;
        }

        switch (pc) {
            case PAWN:
                filename += "pawn";
                break;
            case KNIGHT:
                filename += "knight";
                break;
            case BISHOP:
                filename += "bishop";
                break;
            case ROOK:
                filename += "rook";
                break;
            case QUEEN:
                filename += "queen";
                break;
            case KING:
                filename += "king";
                break;
        }

        filename += ".bmp";

        return filename;
    }

   private:
    PieceClassEnum type;
    PieceColor color;
};

#endif  // !PIECES_H_INCLUDED