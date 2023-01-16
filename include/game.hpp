#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "board.hpp"

bool isValidMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending);

bool isValidPawnMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending);
bool isValidKnightMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending);
bool isValidBishopMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending);
bool isValidRookMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending);
bool isValidQueenMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending);
bool isValidKingMove(Board* board, std::pair<int, int> starting, std::pair<int, int> ending);

#endif // !GAME_HPP_INCLUDED
