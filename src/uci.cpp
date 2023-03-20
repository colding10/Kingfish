#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "consts.hpp"
#include "position.hpp"

int parse(std::string c) {
    int fil = c[0] - 'a';
    int rank = c[1] - 1;

    return A1 + fil - 10 * rank;
}

std::string render(int i) {
    int rank = i - A1 / 10;
    int file = i % 10;

    return (char)(file + 'a') + std::to_string(-rank + 1);
}

void tokenize(std::string const &str, const char delim,
              std::vector<std::string> &out) {
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

int main() {
    std::string in_string;
    const char delim = ' ';

    std::vector<Position> hist = {
        Position(INITIAL, 0, {true, true}, {true, true}, 0, 0)};

    while (true) {
        std::cin >> in_string;
        std::vector<std::string> args;
        tokenize(in_string, delim, args);

        if (args[0] == "uci") {
            std::cout << "id name " << VERSION << "\n";
            std::cout << "uciok"
                      << "\n";
        } else if (args[0] == "isready") {
            std::cout << "readyok"
                      << "\n";
        } else if (args[0] == "quit") {
            break;
        } else if (args[0] == "position" && args[1] == "startpos") {
            hist = {Position(INITIAL, 0, {true, true}, {true, true}, 0, 0)};

            for (int ply = 0; ply < args.size() - 3; ply++) {
                std::string move = args[ply];

                int i = parse(move.substr(0, 2));
                int j = parse(move.substr(2, 2));

                std::string prom = move.substr(4);
                std::transform(prom.begin(), prom.end(), prom.begin(), ::toupper);

                if (ply % 2 == 1) {
                    i = 119 - i, j = 119 - j;
                }

                hist.push_back(hist[hist.size() - 1].move(Move(i, j, prom[0])));
            }
        } else if (args[0] == "go") {
            int wtime, btime, winc, binc;
            wtime = std::stoi(args[2]);
            btime = std::stoi(args[4]);
            winc = std::stoi(args[6]);
            binc = std::stoi(args[8]);

            if (hist.size() % 2 == 0) {
                wtime = btime;
                winc = binc;
            }
            int think = std::min(wtime / 40 + winc, wtime / 2 - 1);
            auto start_time = std::chrono::high_resolution_clock::now();

            std::string move_str = "";
            //         for depth, gamma, score, move in Searcher().search(hist):
            //             # The only way we can be sure to have the real move
            //             in tp_move, # is if we have just failed high. if
            //             score >= gamma:
            //                 i, j = move.i, move.j
            //                 if len(hist) % 2 == 0:
            //                     i, j = 119 - i, 119 - j
            //                 move_str = render(i) + render(j) +
            //                 move.prom.lower() print(f"info depth {depth}
            //                 score cp {score} pv {move_str}")
            //             if move_str and time.time() - start > think * 0.8:
            //                 break

            //         print("bestmove", move_str or '(none)')
        }
    }

    return 0;
}