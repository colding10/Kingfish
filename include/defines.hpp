#ifndef DEFINES_HPP_INCLUDED
#define DEFINES_HPP_INCLUDED

typedef std::pair<int, int> Location;

inline std::pair<int, int> makeLocation(int i, int j) {
    return std::make_pair(i, j);
}

#endif // !DEFINES_HPP_INCLUDED
