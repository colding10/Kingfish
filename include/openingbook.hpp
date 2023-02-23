#ifndef OPENINGBOOK_HPP_INCLUDED
#define OPENINGBOOK_HPP_INCLUDED

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "move.hpp"

struct BookMove {
    uint16_t move;
    uint16_t weight;
    uint32_t learn;
    uint32_t recent;
};

typedef std::unordered_map<uint64_t, std::vector<BookMove>> Book;

Book readBook(const std::string &filepath);
bool getBestMove(Book *book, int hash, Move &move);
Move parseMove(std::uint16_t *move_bytes);

#endif  // !OPENINGBOOK_HPP_INCLUDED
