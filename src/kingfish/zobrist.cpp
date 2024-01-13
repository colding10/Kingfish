#include "zobrist.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "position.h"
#include "uci.h"

long long int zobristHash(const Position& pos, bool white_turn) {
    // key=piece^castle^enpassant^turn;
    long long int piece = 0;
    int           index = -1;

    for (char c : pos.board) {
        index++;
        if (isspace(c) || c == '.') {
            continue;
        }

        std::string algebra_notation = render(index);
        char        file             = algebra_notation[0];
        char        row              = algebra_notation[1];

        piece ^= ZOBRIST_KEYS[getPieceOffset(c, file - 'a', row - '0')];
    }

    return piece ^ ZOBRIST_KEYS[768 + encodeCastle(pos)] ^
           ZOBRIST_KEYS[render(pos.ep)[0] - 'a'] ^
           (white_turn ? ZOBRIST_KEYS[780] : 0);
}

long long int getPieceOffset(char piece, int row, int file) {
    // offset_piece=64*kind_of_piece+8*row+file;
    return KIND_OF_PIECE[piece] + 8 * row + file;
}

int encodeCastle(const Position& pos) {
    // white can castle short     0
    // white can castle long      1
    // black can castle short     2
    // black can castle long      3
    // If none of these flags apply then castle=0.
    int out = 0;

    if (pos.wc.first) {
        out ^= 0;
    } else if (pos.wc.second) {
        out ^= 1;
    } else if (pos.bc.first) {
        out ^= 2;
    } else if (pos.bc.second) {
        out ^= 3;
    }

    return out;
}

Book readBook(const std::string& filepath) {
    Book book;

    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }

    // Read header
    char header[84];
    file.read(header, sizeof(header));

    // Read moves
    uint64_t              key;
    std::vector<BookMove> moves;
    while (file.read(reinterpret_cast<char*>(&key), sizeof(key))) {
        uint16_t move_count;
        file.read(reinterpret_cast<char*>(&move_count), sizeof(move_count));
        moves.resize(move_count);
        file.read(reinterpret_cast<char*>(moves.data()),
                  sizeof(BookMove) * move_count);
        book[key] = moves;
    }

    return book;
}

bool getBestMove(Book* book, long long int hash, Move& move) {
    auto it = book->find(hash);

    if (it != book->end()) {
        std::vector<BookMove>& moves = it->second;

        BookMove best_move = moves[0];
        for (const BookMove& move : moves) {
            if (move.weight > best_move.weight) {
                best_move = move;
            }
        }
        move = parseMove(&best_move.move);
        return true;
    }
    return false;
}

Move parseMove(std::uint16_t* move_bytes) {
    int starting_square = move_bytes[0];
    int ending_square   = move_bytes[1];

    int  promotion_piece = move_bytes[3];
    char prom            = ' ';

    for (std::pair<char, int> p : KIND_OF_PIECE) {
        if (p.second == promotion_piece) {
            prom = p.first;
            break;
        }
    }

    return Move(starting_square, ending_square, prom);
}
