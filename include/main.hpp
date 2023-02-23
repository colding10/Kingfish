#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <SDL2/SDL.h>

#include <map>
#include <string>
#include <tuple>

#include "board.hpp"
#include "transtable.hpp"

bool handleEvent(SDL_Event &event, bool &gameover, Board *board);
bool handleEvents(bool &gameover, Board *board);
void blackMove(Board *board, int depth, int time_limit_ms,
               TranspositionTable &transpositionTable,
               std::map<std::string, bool> settings);
void whiteMove(Board *board, int depth, int time_limit_ms,
               TranspositionTable &transpositionTable,
               std::map<std::string, bool> settings);

std::pair<int, int> getDepthAndTime();
std::pair<bool, bool> getPlayerSettings();

std::map<std::string, bool> getSearchSettings();

#endif  // !MAIN_HPP_INCLUDED