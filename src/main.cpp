#include "main.hpp"

#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "ai.hpp"
#include "board.hpp"
#include "game.hpp"
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

    Board board(STARTING_FEN);
    readTables("./assets/");
    TranspositionTable transtable;

    bool running = true;
    bool gameover = false;

    float white_score = 0.0f;
    float black_score = 0.0f;

    return 0;
}

