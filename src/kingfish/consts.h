#ifndef KINGFISH_CONSTS_H
#define KINGFISH_CONSTS_H

#include <string>
#include <unordered_map>
#include <vector>

#include "pieces.h"
#include "types.h"

const int A1 = 91, H1 = 98, A8 = 21, H8 = 28;

static std::unordered_map<char, std::vector<Direction>> DIRECTIONS = {
    {'P', {DIR_NORTH, DIR_NORTH * 2, DIR_NORTHWEST, DIR_NORTHEAST}},
    {'N',
     {DIR_NORTH + DIR_NORTHEAST,
      DIR_EAST + DIR_NORTHEAST,
      DIR_EAST + DIR_SOUTHEAST,
      DIR_SOUTH + DIR_SOUTHEAST,
      DIR_SOUTH + DIR_SOUTHWEST,
      DIR_WEST + DIR_SOUTHWEST,
      DIR_WEST + DIR_NORTHWEST,
      DIR_NORTH + DIR_NORTHWEST}},
    {'B', {DIR_NORTHEAST, DIR_SOUTHEAST, DIR_SOUTHWEST, DIR_NORTHWEST}},
    {'R', {DIR_NORTH, DIR_NORTH, DIR_SOUTH, DIR_WEST}},
    {'Q',
     {DIR_NORTH,
      DIR_EAST,
      DIR_SOUTH,
      DIR_WEST,
      DIR_NORTHEAST,
      DIR_SOUTHEAST,
      DIR_SOUTHWEST,
      DIR_NORTHWEST}},
    {'K',
     {DIR_NORTH,
      DIR_EAST,
      DIR_SOUTH,
      DIR_WEST,
      DIR_NORTHEAST,
      DIR_SOUTHEAST,
      DIR_SOUTHWEST,
      DIR_NORTHWEST}}};

const int MATE_LOWER = PIECE_VALUES['K'] - 10 * PIECE_VALUES['Q'];
const int MATE_UPPER = PIECE_VALUES['K'] + 10 * PIECE_VALUES['Q'];

const int QS             = 35;
const int EVAL_ROUGHNESS = 15;
const int NULLMOVE_DEPTH = 2;

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

#endif // !KINGFISH_CONSTS_H
