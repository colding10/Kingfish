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
        move = parseMove(&best_move.move);
        return true;
    }
    return false;
}

// Parse a 16-byte move from a Polyglot opening book
Move parseMove(std::uint16_t* move_bytes) {
    // Extract the starting and ending squares
    int starting_square = move_bytes[0];
    int ending_square = move_bytes[1];

    // Extract the captured piece and promotion piece
    Piece captured_piece = static_cast<Piece>(move_bytes[2]);
    Piece promotion_piece = static_cast<Piece>(move_bytes[3]);

    // Compute the move number
    int move_number = (move_bytes[4] << 24) | (move_bytes[5] << 16) |
                      (move_bytes[6] << 8) | move_bytes[7];

    // Compute the move value
    int move_value = (move_bytes[8] << 24) | (move_bytes[9] << 16) |
                     (move_bytes[10] << 8) | move_bytes[11];

    // Return the move as a Move object
    return Move(Location(starting_square), Location(ending_square),
                captured_piece, move_number);
}
