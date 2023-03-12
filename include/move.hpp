#ifndef MOVE_HPP_INCLUDED
#define MOVE_HPP_INCLUDED

#include <ostream>
#include <string>

#include "location.hpp"
#include "pieces.hpp"

class Move {
   public:
    // Constructor
    Move(Location starting, Location ending, Piece captured, int number)
        : m_starting(starting), m_ending(ending), m_captured(captured), m_number(number) {}

    // Getters
    Location getStarting() const { return m_starting; }
    Location getEnding() const { return m_ending; }
    Piece getCaptured() const { return m_captured; }
    Piece getPromoted() const { return m_promoted; }
    int getNumber() const { return m_number; }

    // Setters
    void setCaptured(Piece p) { m_captured = p; }
    void setPromoted(Piece p) { m_promoted = p; }
    void setNumber(int n) { m_number = n; }

    // Operators
    friend std::ostream &operator<<(std::ostream &os, const Move &obj) {
        os << "Move(" << obj.m_starting.to_string() << ", "
           << obj.m_ending.to_string() << " " << obj.m_captured << ")";
        return os;
    }

    unsigned int hash() {
        unsigned int hashValue = 0;

        // Hash the starting and ending locations
        hashValue ^= std::hash<Location>()(this->getStarting());
        hashValue ^= std::hash<Location>()(this->getEnding());

        // Hash the captured piece (if any)
        hashValue ^= std::hash<Piece>()(this->getCaptured());

        return hashValue;
    }

    int value = 0;

   private:
    Location m_starting;  // Starting location of the move
    Location m_ending;    // Ending location of the move

    Piece m_captured;  // Piece that was captured (0 if none)
    Piece m_promoted;  // Piece that was promoted (0 if none)
    int m_number;      // Move number
};

#endif  // !MOVE_HPP_INCLUDED