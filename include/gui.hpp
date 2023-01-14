#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <SDL2/SDL.h>

#include "board.hpp"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;

const int TILE_SIZE = 80;

void drawChessboard(SDL_Renderer& renderer);
void drawChessboard(SDL_Renderer& renderer, Board* board);

#endif  // !GUI_H_INCLUDED