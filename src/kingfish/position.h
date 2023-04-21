#ifndef KINGFISH_POSITION_H
#define KINGFISH_POSITION_H

#include <string>
#include <vector>

#include "bitboard.h"
#include "move.h"
#include "types.h"

namespace kingfish {
class Position {
  public:
    Bitboard pieces[2][6];
    // a array of 6 bboards (piece types) for each color (white/black)

    std::pair<bool, bool> wc;
    // the castling rights, [west/queen side, east/king side]
    std::pair<bool, bool> bc;
    // the opponent rights, [west/king side, east/queen side]
    Square en_passant;
    int    score;

    Position(const std::string &fen_str = "a"); // TODO: fix

    inline bool operator==(const Position &other) const;
    inline bool operator<(const Position &other) const;
    inline bool operator>(const Position &other) const;

    Piece getPieceAt(Square square);
    void  setPieceAt(Square square, Piece val);
    void  clearPieceAt(Square square);

    std::vector<Move> genMoves(bool check_king = true) const;
    bool              isCheckmate() const;
    bool              isCheck() const;
    bool              isValidMove(const Move &move) const;

    Position move(const Move &move) const;

    int value(const Move &move) const;
    int value() const;
    int hash() const;
};
} // namespace kingfish
#endif // !KINGFISH_POSITION_H
