#ifndef PIECES_h_INCLUDED
#define PIECES_h_INCLUDED

#include <unordered_map>
#include <vector>

// clang-format off
static std::unordered_map<char, std::vector<int>> PIECE_SQUARE_TABLES = {
    {'P',
     {0,    0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,    0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   100, 100, 100, 100, 100, 100, 100, 100,  0,
      0,   178, 183, 186, 173, 202, 182, 185, 190,  0, 
      0,   107, 129, 121, 144, 140, 131, 144, 107,  0,
      0,   83,  116, 98,  115, 114, 100, 115, 87,   0,
      0,   74,  103, 110, 109, 106, 101, 100, 77,   0,
      0,   78,  109, 105, 89,   90,  98, 103, 81,   0,
      0,   69,  108, 93,  63,  64,  86,  103, 69,   0,
      0,   100, 100, 100, 100, 100, 100, 100, 100,  0,
      0,     0,   0,   0,   0,   0,   0,   0,   0,  0,   
      0,     0,   0,   0,   0,   0,   0,   0,   0,  0}},
    {'N',
     {0,     0,   0,   0,   0,   0,   0,   0,   0,  0,
      0,     0,   0,   0,   0,   0,   0,   0,   0,  0,
      0,   214, 227, 205, 205, 270, 225, 222, 210,  0,
      0,   277, 274, 380, 244, 284, 342, 276, 266,  0,   
      0,   290, 347, 281, 354, 353, 307, 342, 278,  0,   
      0,   304, 304, 325, 317, 313, 321, 305, 297,  0,
      0,   279, 285, 311, 301, 302, 315, 282, 280,  0,   
      0,   262, 290, 293, 302, 298, 295, 291, 266,  0,   
      0,   257, 265, 282, 280, 282, 280, 257, 260,  0,
      0,   206, 257, 254, 256, 261, 245, 258, 211,  0,   
      0,     0,   0,   0,   0,   0,   0,   0,   0,  0,   
      0,     0,   0,   0,   0,   0,   0,   0,   0,  0}},
    {'B',
     {0,     0,   0,   0,   0,   0,   0,   0,   0,   0,   
      0,     0,   0,   0,   0,   0,   0,   0,   0,   0,   
      0,   261, 242, 238, 244, 297, 213, 283, 270,   0,
      0,   309, 340, 355, 278, 281, 351, 322, 298,   0,   
      0,   311, 359, 288, 361, 372, 310, 348, 306,   0,   
      0,   345, 337, 340, 354, 346, 345, 335, 330,   0,
      0,   333, 330, 337, 343, 337, 336, 320, 327,   0,   
      0,   334, 345, 344, 335, 328, 345, 340, 335,   0,   
      0,   339, 340, 331, 326, 327, 326, 340, 336,   0,
      0,   313, 322, 305, 308, 306, 305, 310, 310,   0,   
      0,     0,   0,   0,   0,   0,   0,   0,   0,   0,   
      0,     0,   0,   0,   0,   0,   0,   0,   0,   0}},
    {'R',
     {0,     0,   0,   0,   0,   0,   0,   0,   0,   0,   
      0,     0,   0,   0,   0,   0,   0,   0,   0,   0,   
      0,   514, 508, 512, 483, 516, 512, 535, 529,   0,
      0,   534, 508, 535, 546, 534, 541, 513, 539,   0,   
      0,   498, 514, 507, 512, 524, 506, 504, 494,   0,   
      0,   479, 484, 495, 492, 497, 475, 470, 473,   0,
      0,   451, 444, 463, 458, 466, 450, 433, 449,   0,   
      0,   437, 451, 437, 454, 454, 444, 453, 433,   0,   
      0,   426, 441, 448, 453, 450, 436, 435, 426,   0,
      0,   449, 455, 461, 484, 477, 461, 448, 447,   0,   
      0,     0,   0,   0,   0,   0,   0,   0,   0,   0,   
      0,     0,   0,   0,   0,   0,   0,   0,   0,   0}},
    {'Q',
     {0,     0,   0,   0,   0,   0,   0,   0,   0,   0,   
      0,     0,   0,   0,   0,   0,   0,   0,   0,   0,   
      0,   935, 930, 921, 825, 998, 953,1017, 955,   0,  
      0,   943, 961, 989, 919, 949,1005, 986, 953,   0,   
      0,   927, 972, 961, 989, 1001,992, 972, 931,   0,   
      0,   930, 913, 951, 946, 954, 949, 916, 923,   0,    
      0,   915, 914, 927, 924, 928, 919, 909, 907,   0,
      0,   899, 923, 916, 918, 913, 918, 913, 902,   0,   
      0,   893, 911, 929, 910, 914, 914, 908, 891,   0,   
      0,   890, 899, 898, 916, 898, 893, 895, 887,   0,   
      0,     0,   0,   0,   0,   0,   0,    0,   0,  0,  
      0,     0,   0,   0,   0,   0,   0,    0,   0,  0}},
    {'K', 
     {0, 0,     0,     0,     0,     0,     0,     0,     0,     0,
      0, 0,     0,     0,     0,     0,     0,     0,     0,     0,
      0, 60004, 60054, 60047, 59901, 59901, 60060, 60083, 59938, 0,
      0, 59968, 60010, 60055, 60056, 60056, 60055, 60010, 60003, 0,
      0, 59938, 60012, 59943, 60044, 59933, 60028, 60037, 59969, 0,
      0, 59945, 60050, 60011, 59996, 59981, 60013, 60000, 59951, 0,
      0, 59945, 59957, 59948, 59972, 59949, 59953, 59992, 59950, 0,
      0, 59953, 59958, 59957, 59921, 59936, 59968, 59971, 59968, 0,
      0, 59996, 60003, 59986, 59950, 59943, 59982, 60013, 60004, 0,
      0, 60017, 60030, 59997, 59986, 60006, 59999, 60040, 60018, 0,
      0, 0,     0,     0,     0,     0,     0,     0,     0,     0,
      0, 0,     0,     0,     0,     0,     0,     0,     0,     0}}};
// clang-format on
static std::unordered_map<char, int> PIECE_VALUES = {
    {'P', 100}, {'N', 280}, {'B', 320}, {'R', 429}, {'Q', 929}, {'K', 60000}};

#endif // !PIECES_h_INCLUDED