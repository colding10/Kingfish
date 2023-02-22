#include "main.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <thread>
#include <tuple>

#include "ai.hpp"
#include "board.hpp"
#include "game.hpp"
#include "gui.hpp"

/*
 * Handles a singular `SDL_Event` and returns a boolean value
 *
 * @return `false` if the game loop should end, `true` otherwise
 */
bool handleEvent(SDL_Event& event, bool gameover, Board* board) {
    if (event.type == SDL_QUIT) {
        return false;
    } else if (event.type == SDL_MOUSEBUTTONUP && !gameover) {
        GUI::handleMouseClicked(event.button, board);
    } else if (event.type == SDL_KEYUP && !gameover) {
        GUI::handleKeyPressed(event.key, board);
    }
    return true;
}

/*
 * Handles multiple `SDL_Event` and returns a boolean value
 *
 * @return `false` if the game loop should end, `true` otherwise
 */
bool handleEvents(bool gameover, Board* board) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (handleEvent(event, gameover, board)) {
            return false;
        }
    }
    return true;
}

void blackMove(Board* board, int depth, int time_limit_ms, TranspositionTable& transpositionTable) {
    Move bm = AI::findBestMove(board, BLACK, depth, time_limit_ms, transpositionTable);
    board->makeMove(bm);
    board->setActiveColor(WHITE);
}

void whiteMove(Board* board, int depth, int time_limit_ms, TranspositionTable& transpositionTable) {
    Move bm = AI::findBestMove(board, WHITE, depth, time_limit_ms, transpositionTable);
    board->makeMove(bm);
    board->toggleActiveColor();
}

std::tuple<int, int> getDepthAndTime() {
    int depth_limit = 1;
    int time_limit_ms = 1000;

    std::cout << "enter negamax depth limit: ";
    std::cin >> depth_limit;
    std::cout << "enter negamax time limit: ";
    std::cin >> depth_limit;

    std::cout << "negamax depth limit: " << depth_limit << std::endl;
    std::cout << "negamax time limit: " << time_limit_ms << std::endl;

    return std::make_tuple(depth_limit, time_limit_ms);
}


int main() {
    int depth_limit, time_limit_ms;
    std::tie(depth_limit, time_limit_ms) = getDepthAndTime();

    GUI::initSDL();

    SDL_Window* window; 
    SDL_Renderer* renderer;
    TTF_Font* font;

    std::tie(window, renderer, font) = GUI::createObjects();

    Board board;

    board.readFen(STARTING_FEN);
    std::cout << board.getLegalMoves(WHITE).size() << std::endl;

    bool running = true;
    bool gameover = false;

    float white_score = 0.0f;
    float black_score = 0.0f;

    TranspositionTable transtable;

    bool bot_turn = false;

    while (running) {
        if (board.evaluateBoard(WHITE) != white_score || board.evaluateBoard(BLACK) != black_score) {
            white_score = board.evaluateBoard(WHITE);
            black_score = board.evaluateBoard(BLACK);

            board.printBoard();
        }

        running = handleEvents(gameover, &board);

        GUI::drawChessboard(renderer, &board, font);

        SDL_RenderPresent(renderer);

        if (Game::isInCheckMate(&board, WHITE)) {
            board.checkmated_color = WHITE;
            gameover = true;
        }
        if (Game::isInCheckMate(&board, BLACK)) {
            board.checkmated_color = BLACK;
            gameover = true;
        }

        if (!gameover) {
            if (board.getActiveColor() == BLACK) {
                blackMove(&board, depth_limit, time_limit_ms, transtable);
            } else {
                // whiteMove(&board, 4, time_limit_ms, transtable);
            }
        }
    }

    GUI::cleanupSDL(renderer, window, font);

    return 0;
}
