#ifndef TYPES_HPP_INCLUDED
#define TYPES_HPP_INCLUDED

#include <unordered_map>
#include <vector>

#include "polyglot.hpp"

typedef int                                                 PositionHash;
typedef int                                                 Direction;
typedef std::unordered_map<uint64_t, std::vector<BookMove>> Book;

#endif