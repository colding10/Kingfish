#ifndef CONSTS_HPP_INCLUDED
#define CONSTS_HPP_INCLUDED

#include <string>
#include <unordered_map>
#include <vector>

#include "pieces.hpp"

typedef int Direction;

const int A1 = 91, H1 = 98, A8 = 21, H8 = 28;
const int N = -10, E = 1, S = 10, W = -1;

static std::unordered_map<char, std::vector<Direction>> directions = {
    {'P', {N, N + N, N + W, N + E}},
    {'N',
     {N + N + E, E + N + E, E + S + E, S + S + E, S + S + W, W + S + W,
      W + N + W, N + N + W}},
    {'B', {N + E, S + E, S + W, N + W}},
    {'R', {N, E, S, W}},
    {'Q', {N, E, S, W, N + E, S + E, S + W, N + W}},
    {'K', {N, E, S, W, N + E, S + E, S + W, N + W}}};

const int MATE_LOWER = piece_values['K'] - 10 * piece_values['Q'];
const int MATE_UPPER = piece_values['K'] + 10 * piece_values['Q'];

const int QS = 35;
const int EVAL_ROUGHNESS = 15;

const std::string VERSION = "Kingfish 0.1";
const std::string INITIAL = (
    "         \n"  //   0 -  9
    "         \n"  //  10 - 19
    " rnbqkbnr\n"  //  20 - 29
    " pppppppp\n"  //  30 - 39
    " ........\n"  //  40 - 49
    " ........\n"  //  50 - 59
    " ........\n"  //  60 - 69
    " ........\n"  //  70 - 79
    " PPPPPPPP\n"  //  80 - 89
    " RNBQKBNR\n"  //  90 - 99
    "         \n"  // 100 -109
    "         \n"  // 110 -119
);

#endif  // !CONSTS_HPP_INCLUDED
