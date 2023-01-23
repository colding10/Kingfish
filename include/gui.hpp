#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "board.hpp"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 1200;

const int TILE_SIZE = 150;

namespace GUI {
    void initSDL();
    void cleanupSDL(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font);
    TTF_Font* createTTFFont();

    SDL_Window* createSDLWindow();
    SDL_Renderer* createSDLRenderer(SDL_Window* window);

    void drawChessboard(SDL_Renderer* renderer, Board* board, TTF_Font* font);
    int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius);

    void handleMouseClicked(SDL_MouseButtonEvent event, Board* board);
    void handleKeyPressed(SDL_KeyboardEvent event, Board* board);

    std::pair<int, int> getBoardIndices(int x, int y);

}  // namespace GUI

#endif  // !GUI_H_INCLUDED