#ifndef SEARCHER_HPP_INCLUDED
#define SEARCHER_HPP_INCLUDED

#include <map>
#include <set>
#include <string>
#include <tuple>

#include "consts.hpp"
#include "move.hpp"
#include "pieces.hpp"
#include "position.hpp"
#include "transtable.hpp"

typedef std::input_iterator_tag iterator_category;
typedef std::tuple<int, int, int, Move> value_type;
typedef std::ptrdiff_t difference_type;
typedef value_type* pointer;
typedef value_type& reference;

struct Entry {
    int lower = -MATE_UPPER;
    int upper = MATE_UPPER;

    Entry() = default;
    Entry(int _lower, int _upper) : lower(_lower), upper(_upper) {}
};

struct Key {
    Position pos;
    int depth;
    bool null_move;

    Key() = default;
    Key(Position _pos, int _depth, bool _null_move)
        : pos(_pos), depth(_depth), null_move(_null_move) {}
};

class Searcher {
   public:
    std::map<PositionHash, Entry> tp_score;
    std::map<PositionHash, Move> tp_move;

    std::set<Position> history;
    int nodes_searched = 0;

    int bound(Position& pos, int gamma, int depth, bool can_null);
    // std::tuple<int, int, int, Move> search(std::set<Position> history);
    // std::tuple<int, int, int, Move> search(std::set<Position> history);
    void earch(coro_t::push_type& yield, std::set<Position> history);
};

#endif  // !SEARCHER_HPP_INCLUDED