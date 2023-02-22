#ifndef LOCATION_HPP_INCLUDED
#define LOCATION_HPP_INCLUDED

#include <string>

struct Location {
    int X;
    int Y;
    Location(int x, int y) : X(x), Y(y) {}
    Location(const std::pair<int, int>& loc) : X(loc.first), Y(loc.second) {}

    operator std::pair<int, int>() const { return {X, Y}; }
    bool operator==(const Location& rhs) const { return X == rhs.X && Y == rhs.Y; }
    bool operator!=(const Location& rhs) const { return !(*this == rhs); }
    bool operator<(const Location& rhs) const { return X < rhs.X || (X == rhs.X && Y < rhs.Y); }
    bool operator>(const Location& rhs) const { return X > rhs.X || (X == rhs.X && Y > rhs.Y); }
    bool operator<=(const Location& rhs) const { return !(*this > rhs); }
    bool operator>=(const Location& rhs) const { return !(*this < rhs); }
    bool operator[] (const int& x) const { return x == 0 ? X : Y; }

    std::string to_string() const { return "(" + std::to_string(X) + ", " + std::to_string(Y) + ")"; }
};

#endif  // !LOCATION_HPP_INCLUDED
