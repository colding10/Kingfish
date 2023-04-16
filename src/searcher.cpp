#include "searcher.hpp"

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

#include "consts.hpp"
#include "generator.hpp"
#include "move.hpp"
#include "pieces.hpp"
#include "position.hpp"
#include "uci.hpp"

int Searcher::bound(Position &pos, int gamma, int depth, bool can_null = true) {
    this->nodes_searched += 1;

    depth = std::max(depth, 0);

    if (pos.score <= -MATE_LOWER) {
        return -MATE_UPPER;
    }

    auto entry = this->tp_score.at(pos.hash());
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

    auto moves = [&]() -> generator<std::pair<Move, int>> {
        bool any_of_RBNQ = pos.board.find('R') != std::string::npos ||
                           pos.board.find('B') != std::string::npos ||
                           pos.board.find('N') != std::string::npos ||
                           pos.board.find('Q') != std::string::npos;

        if (can_null && depth > 2 && any_of_RBNQ) {
            Position rot_board = pos.rotate(true);
            co_yield {NULLMOVE, -this->bound(rot_board, 1 - gamma, depth - 3)};
        }

        if (depth == 0) {
            co_yield {NULLMOVE, pos.score};
        }

        int val_lower = (depth == 0 ? QS : -MATE_LOWER);

        // # Look for the strongest move from last time, the hash-move.
        // killer = self.tp_move.get(pos)

        // # If there isn't one, try to find one with a more shallow search.
        // # This is known as Internal Iterative Deepening (IID). We set
        // # can_null=True, since we want to make sure we actually find a move.
        // if not killer and depth > 2:
        //     self.bound(pos, gamma, depth - 3, can_null=False)
        //     killer = self.tp_move.get(pos)

        // # If depth == 0 we only try moves with high intrinsic score (captures and
        // # promotions). Otherwise we do all moves. This is called quiescent search.
        // val_lower = QS if depth == 0 else -MATE_LOWER

        // # Only play the move if it would be included at the current val-limit,
        // # since otherwise we'd get search instability.
        // # We will search it again in the main loop below, but the tp will fix
        // # things for us.
        // if killer and pos.value(killer) >= val_lower:
        //     yield killer, -self.bound(pos.move(killer), 1 - gamma, depth - 1)

        std::vector<std::pair<int, Move>> rest_moves;

        for (auto m : pos.genMoves()) {
            rest_moves.push_back({pos.value(m), m});
        }

        std::sort(rest_moves.begin(),
                  rest_moves.end(),
                  std::greater<std::pair<int, Move>>());

        for (std::pair<int, Move> m_pair : rest_moves) {
            int val       = m_pair.first;
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

        if (best >= gamma) {
            this->tp_score[pos.hash()] = Entry(best, entry.upper);
        }
        if (best < gamma) {
            this->tp_score[pos.hash()] =
                Entry(entry.lower, best); // TODO: check if should be including
                                          // other flags. THINK SHOULD
        }
    }
    return best;
}

generator<std::tuple<int, int, Move>>
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

void Searcher::searchInfinite(std::vector<Position> &hist) {
    std::string move_str;
    this->stop_search = false;
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

            int i = move.i, j = move.j;
            if (hist.size() % 2 == 0) {
                i = 119 - i, j = 119 - j;
            }
            move_str = render(i) + render(j) + (char)tolower(move.prom);
            std::cout << "info depth " << depth << " nodes " << nodes_searched
                      << " score cp " << score << " pv " << move_str
                      << std::endl;
        }
    }
    std::cout << "bestmove " << (move_str.length() ? move_str : "(none)")
              << std::endl;
}

void Searcher::stopInfiniteSearch() {
    this->stop_search = true;
}