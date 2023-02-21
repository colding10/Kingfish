#ifndef LOCATION_HPP_INCLUDED
#define LOCATION_HPP_INCLUDED

struct Location {
    int X;
    int Y;
    Location(int x, int y) : X(x), Y(y) {}
    Location(const std::pair<int, int>& loc) : X(loc.first), Y(loc.second) {}
    
    operator std::pair<int, int>() const { return {X, Y}; }
    bool operator==(const Location& rhs) const { return X == rhs.X && Y == rhs.Y; }
    bool operator!=(const Location& rhs) const { return !(*this == rhs); }
};

#endif  // !LOCATION_HPP_INCLUDED
