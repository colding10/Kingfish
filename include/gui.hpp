#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <SDL2/SDL.h>

#include "board.hpp"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 1200;

const int TILE_SIZE = 150;

std::pair<int, int> getBoardIndices(int x, int y);
void drawChessboard(SDL_Renderer& renderer, Board* board);
void handleMouseClicked(SDL_MouseButtonEvent event, Board* board);
void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius);
int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius);

#endif  // !GUI_H_INCLUDED