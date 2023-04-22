#ifndef KINGFISH_MOVE_H
#define KINGFISH_MOVE_H

#include "pieces.h"

struct Move {
    int i = 0;
    int j = 0;

    int value = 0;

    char prom = ' ';

    Move() = default;
    Move(int _i, int _j, char _prom)
        : i(_i)
        , j(_j)
        , prom(_prom) {}

    inline bool operator==(const Move &m) const {
        return (j == m.j) && (i == m.i) && (prom == m.prom);
    }
    inline bool operator!=(const Move &m) const {
        return (j != m.j) || (i != m.i) || (prom != m.prom);
    }
    inline bool operator<(const Move &m) const {
        return std::tie(i, j, prom) < std::tie(m.i, m.j, m.prom);
    }
    inline bool operator>(const Move &m) const {
        return std::tie(i, j, prom) > std::tie(m.i, m.j, m.prom);
    }
};

const Move NULLMOVE = Move(0, 0, ' ');

#endif // !KINGFISH_MOVE_H