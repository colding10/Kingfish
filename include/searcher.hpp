#ifndef SEARCHER_HPP_INCLUDED
#define SEARCHER_HPP_INCLUDED

#include <map>
#include <set>
#include <string>

#include "consts.hpp"
#include "move.hpp"
#include "pieces.hpp"
#include "position.hpp"
#include "transtable.hpp"

struct Entry {
    int lower = -MATE_UPPER;
    int upper = MATE_UPPER;

    Entry(int _lower, int _upper) : lower(_lower), upper(_upper) {}
};

struct Key {
    Position pos;
    int depth;
    bool null_move;

    Key(Position _pos, int _depth, bool _null_move) : pos(_pos), depth(_depth), null_move(_null_move) {}
};

class Searcher {
    std::map<Position, Entry> tp_score;
    std::map<Position, Move> tp_move;

    std::set<int> history;
    int nodes_searched = 0;

    int bound(Position& pos, int gamma, int depth, bool can_null);
};

#endif  // !SEARCHER_HPP_INCLUDED