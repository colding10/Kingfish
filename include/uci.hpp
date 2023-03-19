#ifndef UCI_HPP_INCLUDED
#define UCI_HPP_INCLUDED

#include "move.hpp"
#include "pieces.hpp"

int uciLoop();
Piece pieceFromChar(const char& c);
Move parseMove(const std::string& move_str, const PieceColor& current_color);
std::vector<std::string> split(const std::string& str, const std::string& delimiter);
std::tuple<int, std::string> handleCommand(const std::string& command, Board& board);

#endif  //! UCI_HPP_INCLUDED