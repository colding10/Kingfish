#ifndef SEARCHER_HPP_INCLUDED
#define SEARCHER_HPP_INCLUDED

#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "consts.hpp"
#include "defaultdict.hpp"
#include "generator.hpp"
#include "move.hpp"
#include "pieces.hpp"
#include "position.hpp"
struct Entry {
    int lower = -MATE_UPPER;
    int upper = MATE_UPPER;

    Entry() = default;
    Entry(int _lower, int _upper)
        : lower(_lower)
        , upper(_upper) {}
};

struct Key {
    Position pos;
    int      depth;
    bool     null_move;

    Key() = delete;
    Key(Position _pos, int _depth, bool _null_move)
        : pos(_pos)
        , depth(_depth)
        , null_move(_null_move) {}
};

class Searcher {
  public:
    Searcher()
        : tp_score{Entry(-MATE_UPPER, MATE_UPPER)} {}
    DefaultDict<PositionHash, Entry> tp_score;
    std::map<PositionHash, Move>     tp_move;

    std::vector<Position> history;
    int                   nodes_searched = 0;

    int bound(Position &pos, int gamma, int depth, bool can_null);
    generator<std::tuple<int, int, Move>> search(std::vector<Position> hist,
                                                 int                   depth);

    void searchInfinite(std::vector<Position> &hist);
    void stopInfiniteSearch();

  private:
    bool stop_search = false;
};

#endif // !SEARCHER_HPP_INCLUDED