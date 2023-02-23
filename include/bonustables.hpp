#pragma once

#ifndef BONUSTABLES_HPP_INCLUDED
#define BONUSTABLES_HPP_INCLUDED

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace Tables {
    static int pawn_table[8][8];
    static int knight_table[8][8];
    static int bishop_table[8][8];
    static int rook_table[8][8];
    static int queen_table[8][8];
    static int king_table[8][8];
};  // namespace Tables

static void readTables(std::string basepath) {
    std::ifstream pawn_in(basepath + "pawn_table.txt");
    std::ifstream knight_in(basepath + "knight_table.txt");
    std::ifstream bishop_in(basepath + "bishop_table.txt");
    std::ifstream rook_in(basepath + "rook_table.txt");
    std::ifstream queen_in(basepath + "queen_table.txt");
    std::ifstream king_in(basepath + "king_table.txt");

    for (int i = 0; i < 8; i++) {
        std::string pawn_line, knight_line, bishop_line, rook_line, queen_line, king_line;

        std::getline(pawn_in, pawn_line);
        std::getline(knight_in, knight_line);
        std::getline(bishop_in, bishop_line);
        std::getline(rook_in, rook_line);
        std::getline(queen_in, queen_line);
        std::getline(king_in, king_line);

        int j = 0;

        for (int pos = 0; pos < (int) pawn_line.length(); pos += 3) {
            Tables::pawn_table[i][j++] = std::stoi(pawn_line.substr(pos, 3));
            Tables::knight_table[i][j++] = std::stoi(knight_line.substr(pos, 3));
            Tables::bishop_table[i][j++] = std::stoi(bishop_line.substr(pos, 3));
            Tables::rook_table[i][j++] = std::stoi(rook_line.substr(pos, 3));
            Tables::queen_table[i][j++] = std::stoi(queen_line.substr(pos, 3));
            Tables::king_table[i][j++] = std::stoi(king_line.substr(pos, 3));
        }
    }

    pawn_in.close();
    knight_in.close();
    bishop_in.close();
    rook_in.close();
    queen_in.close();
    king_in.close();
}

#endif  // !BONUSTABLES_HPP_INCLUDED