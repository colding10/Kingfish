#ifndef MOVE_HPP_INCLUDED
#define MOVE_HPP_INCLUDED

#include "pieces.hpp"

struct Move
{
    int i;
    int j;

    char prom;

    Move() = default;
    Move(int _i, int _j, char _prom) : i(_i), j(_j), prom(_prom) {}

    inline bool operator==(const Move& m) const
    {
        return (j == m.j) && (i == m.i) && (prom == m.prom);
    }
    inline bool operator!=(const Move& m) const
    {
        return (j != m.j) || (i != m.i) || (prom != m.prom);
    }
    inline bool operator<(const Move& m) const
    {
        return std::tie(i, j, prom) < std::tie(m.i, m.j, m.prom);
    }
    inline bool operator>(const Move& m) const
    {
        return std::tie(i, j, prom) > std::tie(m.i, m.j, m.prom);
    }
};

const Move NULLMOVE = Move(-1, -1, ' ');

#endif // !MOVE_HPP_INCLUDED