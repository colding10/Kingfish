#include <algorithm>
#include <utility>

#include "consts.hpp"
#include "types.hpp"

struct Entry {
    int lower = -MATE_UPPER;
    int upper = MATE_UPPER;

    Entry() = default;
    Entry(int _lower, int _upper)
        : lower(_lower)
        , upper(_upper) {}
};

struct Key {
    int  pos_hash;
    int  depth;
    bool null_move;

    Key() = delete;
    Key(int _pos_hash, int _depth, bool _null_move)
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
        std::size_t     seed = 0;
        std::hash<int>  pos_hash_hasher;
        std::hash<int>  depth_hasher;
        std::hash<bool> null_move_hasher;

        seed ^= pos_hash_hasher(key.pos_hash) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);
        seed ^=
            depth_hasher(key.depth) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= null_move_hasher(key.null_move) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);

        return seed;
    }
};

template <typename kT, typename eT>
class FixedSizeHashTable {
  public:
    explicit FixedSizeHashTable(int size_in_MB, eT default_value)
        : default_value_(std::move(default_value)) {
        const int num_entries = size_in_MB * 1024 * 1024 / sizeof(Node);
        table_.reserve(num_entries);
    }

    void insert(const kT &key, const eT &value) { table_[key] = value; }

    const eT &operator[](const Key &key) const {
        const auto it = table_.find(key);
        if (it == table_.end()) {
            return default_value_;
        } else {
            return it->second;
        }
    }

  private:
    struct Node {
        kT key;
        eT value;

        Node() = default;
        Node(const kT &_key, const eT &_value)
            : key(_key)
            , value(_value) {}
    };

    std::unordered_map<kT, eT, KeyHasher> table_;
    eT                                    default_value_;
};