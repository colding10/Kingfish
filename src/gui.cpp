#include "gui.hpp"

#include <SDL2/SDL.h>

#include <fstream>
#include <iostream>
#include <string>

#include "board.hpp"
#include "pieces.hpp"

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("C++ Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Board board;
    // board.readFen(board.starting_fen);

    // board.printBoard();

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);  // Clear the screen
        drawChessboard(*renderer);  // Draw the chessboard

        SDL_RenderPresent(renderer);  // Update the screen
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void drawChessboard(SDL_Renderer& renderer) {
    SDL_SetRenderDrawColor(&renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    Piece p;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // p = board[i][j];

            SDL_Rect tile;

            tile.x = i * TILE_SIZE;
            tile.y = j * TILE_SIZE;
            tile.w = TILE_SIZE;
            tile.h = TILE_SIZE;

            // SDL_Surface* image = SDL_LoadBMP("../data/png/black_pawn.png");
            // SDL_Texture* texture = SDL_CreateTextureFromSurface(&renderer, image);
            // SDL_RenderCopy(&renderer, texture, nullptr, &tile);

            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(&renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            } else {
                SDL_SetRenderDrawColor(&renderer, 0x00, 0x00, 0x00, 0xFF);
            }
            SDL_RenderFillRect(&renderer, &tile);
        }
    }
}
