#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "consts.hpp"
#include "position.hpp"
#include "searcher.hpp"

// TODO: move search time into own file (timemanager)
// TODO: create uci options dict, then export to make global

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
    // assume 50 move games
    int remaining_time;
    int moves_left =
        std::max(0, 50 - (int)hist.size() / 2); // don't go negative
    if (hist.size() % 2 == 1) {
        remaining_time = wtime + winc * moves_left;
    } else {
        remaining_time = btime + binc * moves_left;
    }
    std::cout << "remaining time: " << remaining_time << std::endl;

    // Calculate time to allocate for searching (75% of remaining time)
    int move_time   = remaining_time / moves_left;
    int search_time = static_cast<int>(move_time * 0.75);

    return search_time;
}

int parse(const std::string &c) {
    int fil  = c[0] - 'a';
    int rank = int(c[1] - '0') - 1;

    return A1 + fil - 10 * rank;
}

std::string render(int i) {
    auto modulo = [](int dividend, int divisor) {
        return (dividend % divisor + divisor) % divisor;
    };
    auto floor_division = [](int dividend, int divisor) {
        int quotient = dividend / divisor;
        if ((dividend < 0) != (divisor < 0) && (dividend % divisor) != 0) {
            quotient--;
        }
        return quotient;
    };

    int rank = floor_division((i - A1), 10);
    int fil  = modulo((i - A1), 10);

    return (char)(fil + 'a') + std::to_string(-rank + 1);
}

void tokenize(const std::string        &str,
              const char                delim,
              std::vector<std::string> &out) {
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

int main() {
    // TESTING HASH SIZES
    // Key: 12 bytes
    // Entry: 8 bytes
    // Hashed Entry: 8 bytes

    // I need 20 bytes to store one pair
    // I can only store 6.4 million pairs w/ 128 MB

    // That's alr

    // std::cout << sizeof(Key) << std::endl;
    // std::cout << sizeof(std::size_t) << std::endl;
    // std::cout << sizeof(Entry) << std::endl;
    // return 0;

    // TODO: commands to add

    // RECIEVING
    // setoption name <id> [value <x>]
    // position fen <fenstring>
    // go
    // go ponder
    // go searchmoves
    // go movestogo
    // go depth
    // go nodes
    // go mate
    // go movetime
    // ponderhit

    // SENDING
    // bestmove <move> ponder <move2>

    // info seldepth
    // info pv (line)
    // info multipv <num>
    // info score cp <x> mate <y>
    // info currmove <move>
    // info currmovenumber <x>
    // info hashfull <x>
    // info nps <x>
    // info tbhits <x>
    // info sbhits <x>
    // info cpuload <x>
    // info currline <cpunr> <move1> ... <movei>
    // option
    // option name

    const char delim = ' ';

    std::vector<Position> hist = {
        Position(INITIAL, 0, {true, true}, {true, true}, 0, 0)};
    Searcher searcher;

    for (std::string line; std::getline(std::cin, line);) {
        std::vector<std::string> args;
        tokenize(line, delim, args);

        if (args[0] == "uci") {
            std::cout << "id name " << VERSION << std::endl;
            std::cout << "id author Colin D" << std::endl;
            std::cout << "uciok" << std::endl;
        } else if (args[0] == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (args[0] == "quit") {
            break;
        } else if ((args[0] == "position" && args[1] == "startpos") ||
                   args[0] == "ucinewgame") {
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
            searcher.nodes_searched = 0;
            bool infinite =
                std::find(args.begin(), args.end(), "infinite") != args.end();

            // Determine search time from time control options
            int ms_time = getSearchTime(args, hist);

            std::cout << "time allocated: " << ms_time << std::endl;
            auto start_time = std::chrono::high_resolution_clock::now();
            auto end_time   = start_time + std::chrono::milliseconds(ms_time);

            Move move;

            if (!infinite) {
                std::future<void> search_result =
                    std::async(std::launch::async, [&]() {
                        int         gamma, score = 0;
                        bool        flag     = false;
                        std::string move_str = "";

                        for (int depth = 1; depth < 1000; depth++) {
                            if (flag) {
                                break;
                            }

                            auto result_moves_gen =
                                searcher.search(hist, depth);
                            for (; result_moves_gen.next();) {
                                auto result = result_moves_gen.value();
                                std::tie(gamma, score, move) = result;

                                int i = move.i, j = move.j;
                                if (hist.size() % 2 == 0) {
                                    i = 119 - i, j = 119 - j;
                                }
                                move_str = render(i) + render(j) +
                                           (char)tolower(move.prom);
                                std::cout
                                    << "info depth " << depth << " nodes "
                                    << searcher.nodes_searched << " time "
                                    << std::chrono::duration_cast<
                                           std::chrono::milliseconds>(
                                           std::chrono::high_resolution_clock::
                                               now() -
                                           start_time)
                                           .count()
                                    << " score cp " << score << " pv "
                                    << move_str << std::endl;

                                if (move_str.length() &&
                                    std::chrono::high_resolution_clock::now() >
                                        end_time) {
                                    flag = true;
                                    break;
                                }
                            }
                        }
                        std::cout << "bestmove "
                                  << (move_str.length() ? move_str : "(none)")
                                  << std::endl;
                    });

                // Wait for the search to complete
                search_result.wait();
            } else {
                std::thread infinite_thread(
                    std::mem_fn(&Searcher::searchInfinite),
                    &searcher,
                    std::ref(hist));

                while (std::getline(std::cin, line) && line != "stop") {
                    // Keep reading input until the "stop" command is received.
                }
                searcher.stopInfiniteSearch();
                infinite_thread.join();
            }
        } else if (args[0] == "debug") {
            if (args.size() > 1) {
                if (args[1] == "board") {
                    std::cout << "board:\n" << hist.back().board << std::endl;
                }
                if (args[1] == "moves") {
                    std::cout << "moves: {";
                    for (Move m : hist.back().genMoves(true)) {
                        std::cout << render(m.i) + render(m.j) + m.prom;
                    }
                    std::cout << "}" << std::endl;
                }
            }
        }
    }

    return 0;
}