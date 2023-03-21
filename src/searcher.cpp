#include "searcher.hpp"

#include <algorithm>
#include <boost/coroutine2/all.hpp>
#include <chrono>
#include <climits>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "consts.hpp"
#include "move.hpp"
#include "pieces.hpp"
#include "position.hpp"
#include "transtable.hpp"

typedef boost::coroutines2::coroutine<const std::string&> coro_t;

int Searcher::bound(Position& pos, int gamma, int depth, bool can_null = true) {
    this->nodes_searched += 1;

    depth = std::max(depth, 0);

    if (pos.score <= -MATE_LOWER) {
        return -MATE_UPPER;
    }

    auto entry = this->tp_score[pos.hash()];
    if (entry.lower >= gamma)
        return entry.lower;
    if (entry.upper < gamma)
        return entry.upper;

    if (can_null && depth > 0 && this->history.count(pos)) {
        return 0;
    }

    auto moves = [&]() {
        std::vector<std::pair<Move, int>> all_moves;

        bool any_of_RBNQ = pos.board.find('R') != std::string::npos ||
                           pos.board.find('B') != std::string::npos ||
                           pos.board.find('N') != std::string::npos ||
                           pos.board.find('Q') != std::string::npos;

        if (can_null && depth > 2 && any_of_RBNQ) {
            Position rot_board = pos.rotate(true);
            all_moves.push_back(
                {NULLMOVE, -this->bound(rot_board, 1 - gamma, depth - 3)});
        }

        if (depth == 0) {
            all_moves.push_back({NULLMOVE, pos.score});
        }
        int val_lower = depth == 0 ? QS : -MATE_LOWER;
        if (tp_move.count(pos.hash())) {
            Move killer = this->tp_move[pos.hash()];
            if (depth > 2) {
                this->bound(pos, gamma, depth - 3, false);
                killer = this->tp_move[pos.hash()];
            }
            if (pos.value(killer) >= val_lower) {
                Position moved_board = pos.move(killer);
                all_moves.push_back(
                    {killer, -this->bound(moved_board, 1 - gamma, depth - 1)});
            }
        }
        std::vector<std::pair<int, Move>> rest_moves;
        for (auto m : pos.genMoves()) {
            rest_moves.push_back({pos.value(m), m});
        }

        std::sort(rest_moves.rend(), rest_moves.rbegin());

        int val;
        for (std::pair<int, Move> m_pair : rest_moves) {
            val = m_pair.first;
            Move move = m_pair.second;

            if (val < val_lower) {
                break;
            }

            if (depth <= 1 && pos.score + val < gamma) {
                all_moves.push_back(
                    {move, val < MATE_LOWER ? pos.score + val : MATE_UPPER});
                break;
            }
            Position moved_board = pos.move(move);
            all_moves.push_back(
                {move, -this->bound(moved_board, 1 - gamma, depth - 1)});
        }

        return all_moves;
    };
    int best = -MATE_UPPER;

    for (auto p : moves()) {
        Move move = p.first;
        int score = p.second;

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
        best = in_check ? -MATE_LOWER : 0;

        if (best >= gamma) {
            this->tp_score[pos.hash()] = Entry(best, entry.upper);
        }
        if (best < gamma) {
            this->tp_score[pos.hash()] =
                Entry(entry.lower, best);  // TODO: check if should be including
                                           // other flags. THINK SHOULD
        }
    }
    return best;
}

// std::tuple<int, int, int, Move> Searcher::search_(std::set<Position> history) {
//     this->nodes_searched = 0;
//     this->history = history;
//     this->tp_score.clear();

//     int gamma = 0;
//     int lower, upper, score;
//     for (int depth = 1; depth < 1000; depth++) {
//         lower = -MATE_LOWER, upper = MATE_LOWER;
//         while (lower < upper - EVAL_ROUGHNESS) {
//             score = this->bound(*(history.rbegin()), gamma, depth, false);
//             if (score >= gamma) {
//                 lower = score;
//             }
//             if (score < gamma) {
//                 upper = score;
//             }

//             // GENERATOR SCREWING START
//             // yield depth, gamma, score, self.tp_move.get(history[-1])
//             // GENERATOR SCREWING END
//             gamma = (lower + upper + 1) / 2;
//         }
//     }
// }

void Searcher::search(coro_t::push_type& yield, std::set<Position> history) {
    this->nodes_searched = 0;
    this->history = history;
    this->tp_score.clear();

    int gamma = 0;
    int lower, upper, score;
    for (int depth = 1; depth < 1000; depth++) {
        lower = -MATE_LOWER, upper = MATE_LOWER;
        while (lower < upper - EVAL_ROUGHNESS) {
            score = this->bound(*(history.rbegin()), gamma, depth, false);
            if (score >= gamma) {
                lower = score;
            }
            if (score < gamma) {
                upper = score;
            }

            yield(std::make_tuple(depth, gamma, score, this->tp_move[(*(history.rbegin())).hash()]));
            gamma = (lower + upper + 1) / 2;
        }
    }
}