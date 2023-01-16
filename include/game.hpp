#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "board.hpp"
#include "pieces.hpp"

bool isValidMove(Board* board, Location starting, Location ending);

bool isValidPawnMove(Board* board, Location starting, Location ending, PieceColor starting_color);
bool isValidKnightMove(Board* board, Location starting, Location ending, PieceColor starting_color);
bool isValidBishopMove(Board* board, Location starting, Location ending, PieceColor starting_color);
bool isValidRookMove(Board* board, Location starting, Location ending, PieceColor starting_color);
bool isValidQueenMove(Board* board, Location starting, Location ending, PieceColor starting_color);
bool isValidKingMove(Board* board, Location starting, Location ending, PieceColor starting_color);

#endif  // !GAME_HPP_INCLUDED
