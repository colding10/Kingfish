#ifndef SEARCHER_HPP_INCLUDED
#define SEARCHER_HPP_INCLUDED

#include <functional>
#include <map>
#include <tuple>
#include <vector>

#include "consts.hpp"
#include "defaultdict.hpp"
#include "generator.hpp"
#include "move.hpp"
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
    PositionHash pos_hash;
    int          depth;
    bool         null_move;

    Key() = delete;
    Key(PositionHash _pos_hash, int _depth, bool _null_move)
        : pos_hash(_pos_hash)
        , depth(_depth)
        , null_move(_null_move) {}

    bool operator==(const Key &other) const {
        return pos_hash == other.pos_hash && depth == other.depth &&
               null_move == other.null_move;
    }
};

struct KeyHasher {
    std::size_t operator()(const Key &key) const noexcept {
        std::size_t             seed = 0;
        std::hash<PositionHash> pos_hash_hasher;
        std::hash<int>          depth_hasher;
        std::hash<bool>         null_move_hasher;

        seed ^= pos_hash_hasher(key.pos_hash) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);
        seed ^=
            depth_hasher(key.depth) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= null_move_hasher(key.null_move) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);

        return seed;
    }
};

class Searcher {
  public:
    Searcher()
        : tp_score{Entry(-MATE_UPPER, MATE_UPPER)} {}
    DefaultDict<Key, Entry, KeyHasher> tp_score;
    std::map<PositionHash, Move>       tp_move;

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