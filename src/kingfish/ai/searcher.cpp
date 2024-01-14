#include "searcher.h"

#include <algorithm>
#include <chrono>
#include <climits>
#include <coroutine>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "../clock.h"
#include "../consts.h"
#include "../move.h"
#include "../pieces.h"
#include "../position.h"
#include "../uci.h"
#include "../utils/generator.h"

int Searcher::bound(Position &pos, int gamma, int depth, bool can_null = true) {
    this->nodes_searched += 1;

    depth = std::max(depth, 0);

    if (pos.score <= -MATE_LOWER) {
        return -MATE_UPPER;
    }

    auto entry = this->tp_score.get(Key(pos.hash(), depth, can_null));
    if (entry.lower >= gamma) {
        return entry.lower;
    }
    if (entry.upper < gamma) {
        return entry.upper;
    }

    if (can_null && depth > 0 &&
        std::find(this->history.begin(), this->history.end(), pos) !=
            this->history.end()) {
        return 0;
    }

    auto moves = [&]() -> Generator<std::pair<Move, int>> {
        bool any_of_RBNQ = std::find(pos.board.begin(), pos.board.end(), 'R') !=
                               pos.board.end() ||
                           std::find(pos.board.begin(), pos.board.end(), 'B') !=
                               pos.board.end() ||
                           std::find(pos.board.begin(), pos.board.end(), 'N') !=
                               pos.board.end() ||
                           std::find(pos.board.begin(), pos.board.end(), 'Q') !=
                               pos.board.end();

        if (can_null && depth > NULLMOVE_DEPTH && any_of_RBNQ) {
            Position rot_board = pos.rotate(true);
            co_yield {NULLMOVE, -this->bound(rot_board, 1 - gamma, depth - 3)};
        }

        if (depth == 0) {
            co_yield {NULLMOVE, pos.score};
        }

        int val_lower = (depth == 0 ? QS : -MATE_LOWER);

        Move killer;
        if (this->tp_move.find(pos.hash()) != this->tp_move.end()) {
            if (depth > 2) {
                this->bound(pos, gamma, depth - 3, false);
                killer = tp_move.at(pos.hash());

                if (pos.value(killer) >= val_lower) {
                    Position moved = pos.move(killer);
                    co_yield {killer,
                              -this->bound(moved, 1 - gamma, depth - 1)};
                }
            }
        }

        std::vector<std::pair<int, Move>> rest_moves;

        for (auto m : pos.genMoves()) {
            rest_moves.push_back({pos.value(m), m});
        }

        std::sort(rest_moves.begin(),
                  rest_moves.end(),
                  std::greater<std::pair<int, Move>>());

        for (std::pair<int, Move> m_pair : rest_moves) {
            int  val  = m_pair.first;
            Move move = m_pair.second;

            if (val < val_lower) {
                break;
            }

            if (depth <= 1 && pos.score + val < gamma) {
                co_yield {move,
                          val < MATE_LOWER ? pos.score + val : MATE_UPPER};
                break;
            }
            Position moved_board = pos.move(move);
            co_yield {move, -this->bound(moved_board, 1 - gamma, depth - 1)};
        }
    };

    int best = -MATE_UPPER;

    auto gen = moves();
    for (; gen.next();) {
        auto p = gen.value();

        Move move  = p.first;
        int  score = p.second;

        best = std::max(best, score);
        if (best >= gamma) {
            if (move != NULLMOVE) {
                this->tp_move[pos.hash()] = move;
            }
            break;
        }
    }

    if (depth > 0 && best == -MATE_UPPER) {
        Position flipped = pos.rotate(true);

        bool in_check = this->bound(flipped, MATE_UPPER, 0, true) == MATE_UPPER;
        best          = in_check ? -MATE_LOWER : 0;
    }

    if (best >= gamma) {
        this->tp_score.set(Key(pos.hash(), depth, can_null),
                           Entry(best, entry.upper));
    }
    if (best < gamma) {
        this->tp_score.set(Key(pos.hash(), depth, can_null),
                           Entry(entry.lower, best));
    }
    return best;
}

Generator<std::tuple<int, int, Move>>
Searcher::search(std::vector<Position> hist, int depth) {
    this->history = hist;

    int gamma = 0;
    int lower, upper;

    lower = -MATE_LOWER, upper = MATE_LOWER;
    while (lower < upper - EVAL_ROUGHNESS) {
        int score = this->bound(hist.back(), gamma, depth, false);
        if (score >= gamma) {
            lower = score;
        }
        if (score < gamma) {
            upper = score;
        }

        co_yield std::make_tuple(
            gamma, score, this->tp_move[hist.back().hash()]);
        gamma = (lower + upper + 1) / 2;
    }
}

void Searcher::searchTimed(std::vector<Position> &hist, int ms_time) {
    this->stop_search      = false;
    auto        start_time = Clock::now();
    std::string move_str;
    bool        flip_side = !hist.size() % 2;
    for (int depth = 1; depth < 1000; depth++) {
        if (stop_search) {
            break;
        }

        auto result_moves_gen = search(hist, depth);
        for (; result_moves_gen.next();) {
            if (stop_search) {
                break;
            }

            int  gamma, score;
            Move move;

            auto result                  = result_moves_gen.value();
            std::tie(gamma, score, move) = result;
            move_str =
                render(move.i) + render(move.j) +
                (move.prom != ' ' ? (const char *)tolower(move.prom) : "");

            printPvInfo(move, depth, score, start_time, flip_side);

            if (deltaMs(std::chrono::high_resolution_clock::now(), start_time) >
                ms_time) {
                stop_search = true;
                break;
            }
        }
    }

    std::cout << "bestmove " << (move_str.length() ? move_str : "(none)")
              << std::endl;
}

void Searcher::searchInfinite(std::vector<Position> &hist) {
    std::string move_str;
    this->stop_search = false;
    auto start_time   = Clock::now();

    bool flip_side = !hist.size() % 2;

    for (int depth = 1; !stop_search; depth++) {
        for (auto result_moves_gen = search(hist, depth);
             result_moves_gen.next();) {
            if (stop_search) {
                break;
            }
            auto result = result_moves_gen.value();
            int  gamma, score;
            Move move;
            std::tie(gamma, score, move) = result;

            printPvInfo(move, depth, score, start_time, flip_side);
        }
    }
    std::cout << "bestmove " << (move_str.length() ? move_str : "(none)")
              << std::endl;
}

void Searcher::printPvInfo(Move                                  move,
                           int                                   depth,
                           int                                   score,
                           std::chrono::steady_clock::time_point start_time,
                           bool                                  flip_side) {
    int i = move.i, j = move.j;
    if (flip_side) {
        i = 119 - i, j = 119 - j;
    }
    std::string move_str =
        render(i) + render(j) +
        (move.prom != ' ' ? (const char *)tolower(move.prom) : "");

    int time = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::high_resolution_clock::now() - start_time)
                   .count();

    std::cout << "info depth " << depth << " score cp " << score << " nodes "
              << this->nodes_searched << " nps "
              << (this->nodes_searched * 1000) / time << " hashfull "
              << this->tp_score.getPermillFull() << " time " << time << " pv "
              << move_str << std::endl;
}

void Searcher::stopSearch() {
    this->stop_search = true;
}