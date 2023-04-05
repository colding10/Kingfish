#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "consts.hpp"
#include "position.hpp"
#include "searcher.hpp"

int parse(std::string c) {
    int fil  = c[0] - 'a';
    int rank = int(c[1] - '0') - 1;

    return A1 + fil - 10 * rank;
}

std::string render(int i) {
    int rank = (i - A1) / 10;
    int fil  = (i - A1) % 10;

    if (fil < 0)
        fil += 10;
    if (rank < 0)
        rank -= 1;

    return (char)(fil + 'a') + std::to_string(-rank + 1);
}

void tokenize(std::string const        &str,
              const char                delim,
              std::vector<std::string> &out) {
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

int main() {
    const char delim = ' ';

    std::vector<Position> hist = {
        Position(INITIAL, 0, {true, true}, {true, true}, 0, 0)};
    Searcher searcher;

    for (std::string line; std::getline(std::cin, line);) {
        std::vector<std::string> args;
        tokenize(line, delim, args);

        if (args[0] == "uci") {
            std::cout << "id name " << VERSION << "\n";
            std::cout << "id author Colin D"
                      << "\n";
            std::cout << "uciok"
                      << "\n";
        } else if (args[0] == "isready") {
            std::cout << "readyok"
                      << "\n";
        } else if (args[0] == "quit") {
            break;
        } else if (args[0] == "position" && args[1] == "startpos") {
            hist = {Position(INITIAL, 0, {true, true}, {true, true}, 0, 0)};

            if (args.size() > 2) {
                for (int ply = 0; ply < (int)args.size() - 3; ply++) {
                    std::string move = args[ply + 3];

                    int i = parse(move.substr(0, 2));
                    int j = parse(move.substr(2, 2));

                    std::string prom = move.substr(4);
                    std::transform(
                        prom.begin(), prom.end(), prom.begin(), ::toupper);

                    if (ply % 2 == 1) {
                        i = 119 - i;
                        j = 119 - j;
                    }
                    Position to_add = hist.back().move(Move(i, j, prom[0]));
                    hist.push_back(to_add);
                }
            }
        } else if (args[0] == "go") {
            int wtime, btime;

            wtime = std::stoi(args[2]);
            btime = std::stoi(args[4]);

            if (hist.size() % 2 == 0) {
                wtime = btime;
            }

            float think   = wtime / 70;
            int   ms_time = think * 0.8;

            auto start_time = std::chrono::high_resolution_clock::now();
            auto end_time   = start_time + std::chrono::milliseconds(ms_time);

            int         gamma, score;
            Move        move;
            std::string move_str = "";

            bool flag = false;
            for (int depth = 1; depth < 1000; depth++) {
                if (flag) {
                    break;
                }

                std::vector<std::tuple<int, int, Move>> result_moves =
                    searcher.search(hist, depth);
                for (auto result : result_moves) {
                    std::tie(gamma, score, move) = result;

                    if (score >= gamma) {
                        int i = move.i, j = move.j;
                        if (hist.size() % 2 == 0) {
                            i = 119 - i, j = 119 - j;
                        }
                        move_str =
                            render(i) + render(j) + (char)tolower(move.prom);
                        std::cout << "info depth " << depth << " score cp "
                                  << score << " pv " << move_str << "\n";
                    }
                    if (move_str.length() &&
                        std::chrono::high_resolution_clock::now() > end_time) {
                        flag = true;
                        break;
                    }
                }
            }

            std::cout << "bestmove "
                      << (move_str.length() ? move_str : "(none)") << "\n";
        } else if (args[0] == "debug") {
            if (args.size() > 1) {
                if (args[1] == "board") {
                    std::cout << "board:\n" << hist.back().board << "\n";
                }
            }
        }
    }

    return 0;
}