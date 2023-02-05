#include "main.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <thread>

#include "ai.hpp"
#include "board.hpp"
#include "game.hpp"
#include "gui.hpp"

void blackMove(Board* board, int depth) {
    Move bm = AI::findBestMove(board, depth, BLACK);
    board->makeMove(bm);
    board->toggleActiveColor();
}

void whiteMove(Board* board, int depth) {
    Move bm = AI::findBestMove(board, depth, SDL_WINDOWEVENT_HIT_TEST);
    board->makeMove(bm);
    board->toggleActiveColor();
}

int main() {
    int depth = 1;
    std::cout << "enter negamax depth: ";
    std::cin >> depth;

    std::cout << "negamax depth: " << depth << std::endl;

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

    bool running = true;
    bool gameover = false;

    float white_score;
    float black_score;

    while (running) {
        if (board.evaluateBoard(WHITE) != white_score || board.evaluateBoard(BLACK) != black_score) {
            std::cout << "white score: " << board.evaluateBoard(WHITE) << std::endl;
            white_score = board.evaluateBoard(WHITE);
            std::cout << "black score: " << board.evaluateBoard(BLACK) << std::endl;
            black_score = board.evaluateBoard(BLACK);
        }

        SDL_RenderClear(renderer);
        GUI::drawChessboard(renderer, &board, font);

        SDL_RenderPresent(renderer);

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

        if (!gameover) {
            SDL_RenderClear(renderer);
            GUI::drawChessboard(renderer, &board, font);

            SDL_RenderPresent(renderer);
            if (board.getActiveColor() == BLACK) {
                blackMove(&board, depth);
            } else {
                // whiteMove(&board, 2);
            }
        }
    }

    GUI::cleanupSDL(renderer, window, font);

    return 0;
}