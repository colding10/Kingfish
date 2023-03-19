#include "main.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "ai.hpp"
#include "board.hpp"
#include "game.hpp"
#include "gui.hpp"
#include "location.hpp"
#include "move.hpp"
#include "bonustables.hpp"
#include "openingbook.hpp"
#include "pieces.hpp"

/*
 * the main entry point for the application
 * @return void
 */
int main() {
    int depth_limit, time_limit_ms;
    bool white_is_bot, black_is_bot;

    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;

    std::tie(depth_limit, time_limit_ms) = getDepthAndTime();
    std::tie(white_is_bot, black_is_bot) = getPlayerSettings();

    std::map<std::string, bool> search_settings = getSearchSettings();

    std::tie(window, renderer, font) = GUI::createObjects();

    Board board(STARTING_FEN);
    readTables("./assets/");
    TranspositionTable transtable;

    bool running = true;
    bool gameover = false;

    float white_score = 0.0f;
    float black_score = 0.0f;

    while (running) {
        running = handleEvents(gameover, &board);

        GUI::drawChessboard(renderer, &board, font);

        SDL_RenderPresent(renderer);

        if (board.checkCheckmates()) {
            gameover = true;
        }

        if (!board.isGameOver()) {
            if (board.getActiveColor() == WHITE && white_is_bot) {
                whiteMove(&board, depth_limit, time_limit_ms, transtable, search_settings);
            } else if (board.getActiveColor() == BLACK && black_is_bot) {
                blackMove(&board, depth_limit, time_limit_ms, transtable, search_settings);
            }
        }
    }

    GUI::cleanupSDL(renderer, window, font);

    return 0;
}

/*
 * Handles a singular `SDL_Event` and returns a boolean value

 * @param event a refrence to a event it should handle
 * @param gameover a boolean if the game is over, to know if some handlers should be called
 * @param board a pointer to a `Board` object

 * @return `false` if the game loop should end, `true` otherwise
 */
bool handleEvent(SDL_Event &event, bool &gameover, Board *board) {
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

 * @param gameover a boolean if the game is over, to know if some handlers should be called
 * @param board a pointer to a `Board` object

 * @return `false` if the game loop should end, `true` otherwise
 */
bool handleEvents(bool &gameover, Board *board) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (!handleEvent(event, gameover, board)) {
            return false;
        }
    }
    return true;
}

/*
 * makes a move for `black` in the GUI. calls `AI:findBestMove`.

 * @param board a pointer to a `Board` object
 * @param depth an integer describing the search depth
 * @param time_limit_ms an integer with the time limit in milliseconds
 * @param transpositionTable an activated transposition table for the negamax

 * @return void
 */
void blackMove(Board *board, int depth, int time_limit_ms,
               TranspositionTable &transpositionTable, std::map<std::string, bool> settings) {
    Move bm =
        AI::findBestMove(board, BLACK, depth, time_limit_ms, transpositionTable, settings);
    board->makeMove(bm);
    board->setActiveColor(WHITE);  // TODO: fix this, toggle should be working but it doesnt
}
/*
 * makes a move for `white` in the GUI. calls `AI:findBestMove`.

 * @param board a pointer to a `Board` object
 * @param depth an integer describing the search depth
 * @param time_limit_ms an integer with the time limit in milliseconds
 * @param transpositionTable an activated transposition table for the negamax

 * @return void
 */
void whiteMove(Board *board, int depth, int time_limit_ms,
               TranspositionTable &transpositionTable, std::map<std::string, bool> settings) {
    Move bm =
        AI::findBestMove(board, WHITE, depth, time_limit_ms, transpositionTable, settings);
    board->makeMove(bm);
    board->setActiveColor(BLACK); // TODO: fix this, toggle should be working but it doesnt
}

std::pair<int, int> getDepthAndTime() {
    int depth_limit = 1;
    int time_limit_ms = 1000;

    std::cout << "enter negamax depth limit: ";
    std::cin >> depth_limit;
    std::cout << "enter negamax time limit: ";
    std::cin >> time_limit_ms;

    std::cout << "negamax depth limit: " << depth_limit << std::endl;
    std::cout << "negamax time limit: " << time_limit_ms << std::endl;

    return std::make_pair(depth_limit, time_limit_ms);
}

std::pair<bool, bool> getPlayerSettings() {
    bool white_is_bot = false;
    bool black_is_bot = false;

    std::cout << "\ndo you want white as bot? ";
    std::cin >> white_is_bot;
    std::cout << "do you want black as bot? ";
    std::cin >> black_is_bot;

    return std::make_pair(white_is_bot, black_is_bot);
}

std::map<std::string, bool> getSearchSettings() {
    std::map<std::string, bool> settings;

    std::cout << "\ndo you want quiescence search on? ";
    std::cin >> settings["quiescence"];
    std::cout << "do you want to use the transposition table? ";
    std::cin >> settings["transposition"];

    return settings;
}