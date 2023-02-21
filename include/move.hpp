#ifndef MOVE_HPP_INCLUDED
#define MOVE_HPP_INCLUDED

#include "location.hpp"
#include "pieces.hpp"

class Move {
   public:
    // Constructor
    Move(Location starting, Location ending, Piece captured, int number) : m_starting(starting),
                                                                           m_ending(ending),
                                                                           m_captured(captured),
                                                                           m_number(number) {}

    // Getters
    Location getStarting() const { return m_starting; }
    Location getEnding() const { return m_ending; }
    Piece getCaptured() const { return m_captured; }
    int getNumber() const { return m_number; }

    // Setters
    void setCaptured(Piece p) { m_captured = p; }
    void setNumber(int n) { m_number = n; }

   private:
    Location m_starting;  // Starting location of the move
    Location m_ending;    // Ending location of the move

    Piece m_captured;  // Piece that was captured (0 if none)
    int m_number;      // Move number
};

#endif  // !MOVE_HPP_INCLUDED