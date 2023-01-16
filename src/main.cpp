#include "main.hpp"

#include <SDL2/SDL.h>

#include <iostream>

#include "board.hpp"
#include "game.hpp"
#include "gui.hpp"

int main() {
    SDL_Window* window = GUI::createSDLWindow();
    SDL_Renderer* renderer = GUI::createSDLRenderer(window);

    SDL_Event event;

    Board board;

    board.readFen(STARTING_FEN);
    board.printBoard();

    bool running = true;
    bool gameover = false;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONUP && !gameover) {
                GUI::handleMouseClicked(event.button, &board);
            } else if (event.type == SDL_KEYUP && !gameover) {
                GUI::handleKeyPressed(event.key, &board);
            }
        }

        if (gameover) {
            continue;
        }

        if (Game::isInCheckMate(&board, WHITE)) {
            std::cout << "WHITE IS MATED" << std::endl;
            gameover = true;
        }
        if (Game::isInCheckMate(&board, BLACK)) {
            std::cout << "BLACK IS MATED" << std::endl;
            gameover = true;
        }

        SDL_RenderClear(renderer);
        GUI::drawChessboard(renderer, &board);

        SDL_RenderPresent(renderer);
    }

    GUI::cleanupSDL(renderer, window);

    return 0;
}