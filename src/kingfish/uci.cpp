#include <algorithm>
#include <future>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "./ai/searcher.h"
#include "./ai/timemanager.h"
#include "./clock.h"
#include "./consts.h"
#include "position.h"

// TODO: create uci options dict, then export to make global

int parse(const std::string &c) {
    // parses a string of algebraic notation (a1d4) into an integer
    int fil  = c[0] - 'a';
    int rank = int(c[1] - '0') - 1;

    return A1 + fil - 10 * rank;
}

std::string render(int i) {
    // renders a integer into algebraic notation
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

int uciMainLoop() {
    // TODO: commands to add

    // RECIEVING
    // setoption name <id> [value <x>]
    // position fen <fenstring>

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
            hist.clear();
            hist = {Position(INITIAL, 0, {true, true}, {true, true}, 0, 0)};

            if (args.size() > 2) {
                for (int ply = 3; ply < (int)args.size(); ply++) {
                    std::string move = args[ply];

                    int i = parse(move.substr(0, 2));
                    int j = parse(move.substr(2, 2));

                    std::string prom = move.substr(4);
                    std::transform(
                        prom.begin(), prom.end(), prom.begin(), ::toupper);

                    if (hist.size() % 2 == 0) {
                        i = 119 - i, j = 119 - j;
                    }
                    Position to_add = hist.back().move(Move(i, j, prom[0]));
                    hist.push_back(to_add);
                }
            }
        } else if (args[0] == "go") {
            searcher.nodes_searched = 0;
            bool infinite =
                std::find(args.begin(), args.end(), "infinite") != args.end();

            int ms_time = getSearchTime(args, hist);

            if (!infinite) {
                 std::thread infinite_thread(
                    std::mem_fn(&Searcher::searchTimed),
                    &searcher,
                    std::ref(hist),
                    ms_time);

                while (std::getline(std::cin, line) && line != "stop" && !searcher.stop_search) {
                    // Keep reading input until the "stop" command is received. Or searcher stops the search
                }
                searcher.stopSearch();
                infinite_thread.join();
            } else {
                std::thread infinite_thread(
                    std::mem_fn(&Searcher::searchInfinite),
                    &searcher,
                    std::ref(hist));

                while (std::getline(std::cin, line) && line != "stop" && !searcher.stop_search) {
                    // Keep reading input until the "stop" command is received.Or searcher stops the search
                }
                searcher.stopSearch();
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
