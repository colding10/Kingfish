#ifndef HASHTABLE_h_INCLUDED
#define HASHTABLE_h_INCLUDED

#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>

#include "../consts.h"

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

    bool operator==(const Key& other) const {
        return pos_hash == other.pos_hash && depth == other.depth &&
               null_move == other.null_move;
    }
};

struct KeyHasher {
    std::size_t operator()(const Key& key) const noexcept {
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

constexpr size_t MB = 1024 * 1024;

template <typename K, typename V>
class FixedSizeHashTable {
  public:
    using key_type    = K;
    using mapped_type = V;
    using value_type  = std::pair<const K, V>;

    FixedSizeHashTable(size_t max_size_MB, const V& default_value)
        : max_size_(max_size_MB * MB / sizeof(value_type))
        , default_value_(default_value) {}

    void set(const K& key, const V& value) {
        auto it = hash_table_.find(key);
        if (it != hash_table_.end()) {
            // key already exists in hash table
            // move it to the front of the LRU list
            lru_list_.splice(lru_list_.begin(), lru_list_, it->second);
            it->second->second = value;
        } else {
            // key does not exist in hash table
            // create a new entry and add it to the hash table and LRU list
            if (hash_table_.size() >= max_size_) {
                // hash table is full, remove the least recently used item
                hash_table_.erase(lru_list_.back().first);
                lru_list_.pop_back();
            }
            lru_list_.emplace_front(key, value);
            hash_table_[key] = lru_list_.begin();
        }
    }

    const V& get(const K& key) const {
        auto it = hash_table_.find(key);
        if (it != hash_table_.end()) {
            // key exists in hash table
            // move it to the front of the LRU list
            lru_list_.splice(lru_list_.begin(), lru_list_, it->second);
            return it->second->second;
        } else {
            // key does not exist in hash table
            // return default value
            return default_value_;
        }
    }

    int getPermillFull() const {
        return (hash_table_.size() * 1000 / max_size_);
    }

  private:
    std::unordered_map<K, typename std::list<value_type>::iterator, KeyHasher>
                                  hash_table_;
    mutable std::list<value_type> lru_list_;
    size_t                        max_size_;
    V                             default_value_;
};

#endif