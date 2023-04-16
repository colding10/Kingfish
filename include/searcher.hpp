#ifndef SEARCHER_HPP_INCLUDED
#define SEARCHER_HPP_INCLUDED

#include <functional>
#include <iostream>
#include <map>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "consts.hpp"
#include "defaultdict.hpp"
#include "generator.hpp"
#include "hashtable.hpp"
#include "move.hpp"
#include "position.hpp"

const int mb_size = 100;
class Searcher {
  public:
    Searcher()
        : tp_score{mb_size, Entry(-MATE_UPPER, MATE_UPPER)} {}
    FixedSizeHashTable<Key, Entry> tp_score;
    std::map<PositionHash, Move>   tp_move;

    std::vector<Position> history;
    int                   nodes_searched = 0;

    int bound(Position &pos, int gamma, int depth, bool can_null);
    Generator<std::tuple<int, int, Move>> search(std::vector<Position> hist,
                                                 int                   depth);

    void searchInfinite(std::vector<Position> &hist);
    void stopInfiniteSearch();

  private:
    bool stop_search = false;
};

#endif // !SEARCHER_HPP_INCLUDED