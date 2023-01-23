#include "main.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <thread>

#include "ai.hpp"
#include "board.hpp"
#include "game.hpp"
#include "gui.hpp"

void blackMove(Board* board) {
    Move bm = AI::findBestMove(board, 3, BLACK);
    board->makeMove(bm);
    board->toggleActiveColor();
    std::cout << "best move for black: "
              << "(" << bm.startX << ", " << bm.startY << ") -> (" << bm.endX
              << ", " << bm.endY << ")" << std::endl;
}

int main() {
    GUI::initSDL();

    SDL_Window* window = GUI::createSDLWindow();
    SDL_Renderer* renderer = GUI::createSDLRenderer(window);
    TTF_Font* font = GUI::createTTFFont();

    if (font == NULL) {
        std::cout << TTF_GetError() << std::endl;
    }
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

        if (Game::isInCheckMate(&board, WHITE)) {
            board.checkmated_color = WHITE;
            gameover = true;
        }
        if (Game::isInCheckMate(&board, BLACK)) {
            board.checkmated_color = BLACK;
            gameover = true;
        }

        if (board.getActiveColor() == BLACK) {
            blackMove(&board);
        }

        SDL_RenderClear(renderer);
        GUI::drawChessboard(renderer, &board, font);

        SDL_RenderPresent(renderer);
    }

    GUI::cleanupSDL(renderer, window, font);

    return 0;
}