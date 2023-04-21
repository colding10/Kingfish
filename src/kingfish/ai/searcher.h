#ifndef KINGFISH_SEARCHER_H
#define KINGFISH_SEARCHER_H

#include <iostream>
#include <map>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../consts.h"
#include "../move.h"
#include "../position.h"
#include "../utils/generator.h"
#include "../utils/hashtable.h"

namespace kingfish {
const int mb_size = 16; // TODO: move the mb_size to the UCI options

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

} // namespace kingfish
#endif // !KINGFISH_SEARCHER_H