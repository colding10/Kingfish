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
            return all_moves;
        }

        int val_lower = (depth == 0 ? QS : -MATE_LOWER);
        std::vector<std::pair<int, Move>> rest_moves;

        for (auto m : pos.genMoves()) {
            rest_moves.push_back({pos.value(m), m});
        }

        std::sort(rest_moves.begin(),
                  rest_moves.end(),
                  std::greater<std::pair<int, Move>>());

        int val;
        for (std::pair<int, Move> m_pair : rest_moves) {
            val       = m_pair.first;
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
    this->nodes_searched = 0;
    this->history        = hist;
    this->tp_score.clear();

    int gamma = 0;
    int lower, upper, score;

    lower = -MATE_LOWER, upper = MATE_LOWER;
    while (lower < upper - EVAL_ROUGHNESS) {
        score = this->bound(hist.back(), gamma, depth, false);
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
