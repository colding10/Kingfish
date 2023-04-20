#include "timemanager.h"

#include <iostream>

#include "../position.h"

int getSearchTime(const std::vector<std::string> &args,
                  const std::vector<Position>    &hist) {
    int wtime = 0, btime = 0, winc = 0, binc = 0;

    // Parse time control options
    for (size_t i = 1; i < args.size(); i++) {
        if (args[i] == "wtime") {
            wtime = std::stoi(args[i + 1]);
        } else if (args[i] == "btime") {
            btime = std::stoi(args[i + 1]);
        } else if (args[i] == "winc") {
            winc = std::stoi(args[i + 1]);
        } else if (args[i] == "binc") {
            binc = std::stoi(args[i + 1]);
        }
    }

    // Determine remaining time for current player
    int remaining_time;
    int moves_left =
        std::max(1, 50 - (int)hist.size() / 2); // don't go negative

    if (hist.size() % 2 == 1) {
        remaining_time = wtime + winc * moves_left;
        if (remaining_time <=
            10000) { // less than or equal to 10 seconds remaining
            moves_left     = 10;
            remaining_time = wtime + winc;
        }
    } else {
        remaining_time = btime + binc * moves_left;
        if (remaining_time <=
            10000) { // less than or equal to 10 seconds remaining
            moves_left     = 10;
            remaining_time = btime + binc;
        }
    }

    // Calculate time to allocate for searching (75% of remaining time)
    int move_time   = remaining_time / moves_left;
    int search_time = static_cast<int>(move_time * 0.75);

    std::cout << "info string remaining time: " << remaining_time
              << " time allocated: " << search_time << std::endl;

    return search_time;
}