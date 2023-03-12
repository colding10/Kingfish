#ifndef LOCATION_HPP_INCLUDED
#define LOCATION_HPP_INCLUDED

#include <string>
#include <vector>
#include <stdexcept>


struct Location {
    int X;
    int Y;
    Location(int square) : X(square / 8), Y(square % 8) {}
    Location(int x, int y) : X(x), Y(y) {}
    Location(const std::pair<int, int> &loc) : X(loc.first), Y(loc.second) {}
    Location(const std::string &algebra) {
        if (algebra.size() != 2) {
            throw std::invalid_argument("Invalid algebraic notation");
        }
        char file = algebra[0];
        char rank = algebra[1];
        if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
            throw std::invalid_argument("Invalid algebraic notation");
        }
        X = rank - '1';
        Y = file - 'a';
    }
    
    operator std::pair<int, int>() const { return {X, Y}; }
    bool operator==(const Location &rhs) const {
        return X == rhs.X && Y == rhs.Y;
    }
    bool operator!=(const Location &rhs) const { return !(*this == rhs); }
    bool operator<(const Location &rhs) const {
        return X < rhs.X || (X == rhs.X && Y < rhs.Y);
    }
    bool operator>(const Location &rhs) const {
        return X > rhs.X || (X == rhs.X && Y > rhs.Y);
    }
    bool operator<=(const Location &rhs) const { return !(*this > rhs); }
    bool operator>=(const Location &rhs) const { return !(*this < rhs); }
    bool operator[](const int &x) const { return x == 0 ? X : Y; }

    std::string to_string() const {
        return "(" + std::to_string(X) + ", " + std::to_string(Y) + ")";
    }
};

static std::vector<Location> ALL_LOCATIONS = {Location(0, 0), Location(0, 1), Location(0, 2), Location(0, 3), Location(0, 4), Location(0, 5), Location(0, 6), Location(0, 7), Location(1, 0), Location(1, 1), Location(1, 2), Location(1, 3), Location(1, 4), Location(1, 5), Location(1, 6), Location(1, 7), Location(2, 0), Location(2, 1), Location(2, 2), Location(2, 3), Location(2, 4), Location(2, 5), Location(2, 6), Location(2, 7), Location(3, 0), Location(3, 1), Location(3, 2), Location(3, 3), Location(3, 4), Location(3, 5), Location(3, 6), Location(3, 7), Location(4, 0), Location(4, 1), Location(4, 2), Location(4, 3), Location(4, 4), Location(4, 5), Location(4, 6), Location(4, 7), Location(5, 0), Location(5, 1), Location(5, 2), Location(5, 3), Location(5, 4), Location(5, 5), Location(5, 6), Location(5, 7), Location(6, 0), Location(6, 1), Location(6, 2), Location(6, 3), Location(6, 4), Location(6, 5), Location(6, 6), Location(6, 7), Location(7, 0), Location(7, 1), Location(7, 2), Location(7, 3), Location(7, 4), Location(7, 5), Location(7, 6), Location(7, 7)};

namespace std {
    template <>
    struct hash<Location> {
        std::size_t operator()(const Location &loc) const {
            // Combine the hashes of the x and y coordinates
            return std::hash<int>()(loc.X) ^ std::hash<int>()(loc.Y);
        }
    };
}  // namespace std

#endif  // !LOCATION_HPP_INCLUDED
