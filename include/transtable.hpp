#ifndef TRANSTABLE_HPP_INCLUDED
#define TRANSTABLE_HPP_INCLUDED

#include <unordered_map>

const int EXACT = 0;
const int UPPERBOUND = 1;
const int LOWERBOUND = 2;

class TranspositionTable
{
public:
    struct Entry
    {
        int depth;
        int value;
        int flag;
    };

    void put(long long hash, Entry entry);
    bool get(long long hash, Entry& entry);

private:
    std::unordered_map<int, Entry> table;
};

#endif // !TRANSTABLE_HPP_INCLUDED