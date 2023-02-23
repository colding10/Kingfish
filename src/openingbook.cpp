#include "openingbook.hpp"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "location.hpp"
#include "move.hpp"

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
    uint64_t key;
    std::vector<BookMove> moves;
    while (file.read(reinterpret_cast<char*>(&key), sizeof(key))) {
        uint16_t move_count;
        file.read(reinterpret_cast<char*>(&move_count), sizeof(move_count));
        moves.resize(move_count);
        file.read(reinterpret_cast<char*>(moves.data()), sizeof(BookMove) * move_count);
        book[key] = moves;
    }

    return book;
}

bool getBestMove(Book* book, int hash, Move& move) {
    auto it = book->find(hash);
    BookMove best_move;

    if (it != book->end()) {
        std::vector<BookMove>& moves = it->second;
        // Choose the best move from the list of moves based on the weight and other statistics
        BookMove best_move = moves[0];
        for (const BookMove& move : moves) {
            if (move.weight > best_move.weight) {
                best_move = move;
            }
        }
        // best_move.move is the best move in the opening book for the current position
    } else {
        // The position is not in the opening book, generate all possible moves and choose the one with the highest weight
    }

    std::cout << best_move.move << std::endl;
    return false;
}
