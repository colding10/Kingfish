#ifndef KINGFISH_CONSTS_H
#define KINGFISH_CONSTS_H

#include <string>
#include <unordered_map>
#include <vector>

#include "move.h"
#include "pieces.h"
#include "types.h"

namespace kingfish {
const int A1 = 91, H1 = 98, A8 = 21, H8 = 28;
const int N = -10, E = 1, S = 10, W = -1;

static std::unordered_map<char, std::vector<Direction>> DIRECTIONS = {
    {'P', {N, N + N, N + W, N + E}},
    {'N',
     {N + N + E,
      E + N + E,
      E + S + E,
      S + S + E,
      S + S + W,
      W + S + W,
      W + N + W,
      N + N + W}},
    {'B', {N + E, S + E, S + W, N + W}},
    {'R', {N, E, S, W}},
    {'Q', {N, E, S, W, N + E, S + E, S + W, N + W}},
    {'K', {N, E, S, W, N + E, S + E, S + W, N + W}}};

const int MATE_LOWER = PIECE_VALUES['K'] - 10 * PIECE_VALUES['Q'];
const int MATE_UPPER = PIECE_VALUES['K'] + 10 * PIECE_VALUES['Q'];

const int  QS             = 35;
const int  EVAL_ROUGHNESS = 15;
const int  NULLMOVE_DEPTH = 2;
const Move NULLMOVE       = Move();

const std::string VERSION = "Kingfish 1.2.0";
const std::string INITIAL = ("         \n" //   0 -  9
                             "         \n" //  10 - 19
                             " rnbqkbnr\n" //  20 - 29
                             " pppppppp\n" //  30 - 39
                             " ........\n" //  40 - 49
                             " ........\n" //  50 - 59
                             " ........\n" //  60 - 69
                             " ........\n" //  70 - 79
                             " PPPPPPPP\n" //  80 - 89
                             " RNBQKBNR\n" //  90 - 99
                             "         \n" // 100 -109
                             "         \n" // 110 -119
);

} // namespace kingfish

#endif // !KINGFISH_CONSTS_H
