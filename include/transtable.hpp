#ifndef TRANSTABLE_HPP_INCLUDED
#define TRANSTABLE_HPP_INCLUDED

#include <unordered_map>

class TranspositionTable {
   public:
    struct Entry {
        int depth;
        int value;
        int alpha;
        int beta;
    };

    void put(int key, int depth, int value, int alpha, int beta);

    bool get(int key, int depth, int alpha, int beta, int& value);

   private:
    std::unordered_map<int, Entry> table;
};

#endif // !TRANSTABLE_HPP_INCLUDED