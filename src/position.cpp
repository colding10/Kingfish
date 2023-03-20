#include "position.hpp"

#include <string.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "consts.hpp"
#include "move.hpp"

std::vector<Move> Position::genMoves() {
  std::vector<Move> moves;
  for (int i = 0; i < 64; ++i) {
    char p = this->board[i];
    if (!isupper(p)) {
      continue;
    }
    for (int d : directions[p]) {
      for (int j = i + d;; j += d) {
        char q = board[j];
        if (isspace(q) || isupper(q)) {
          break;
        }
        if (p == 'P') {
          if (d == N && q != '.')
            break;
          if (d == N + N) {
            if (i < A1 + N || board[i + N] != '.')
              break;
          }
          if (d == N + W || d == N + E) {
            if (q == '.' && j != ep && j != kp && j != kp - 1 && j != kp + 1)
              break;
          }
          if (A8 <= j && j <= H8) {
            for (char prom : {'N', 'B', 'R', 'Q'}) {
              moves.push_back(Move(i, j, prom));
            }
            break;
          }
        }
        moves.push_back(Move(i, j, ' '));
        if (p == 'P' || p == 'N' || p == 'K' || islower(q)) {
          break;
        }
        if (i == A1 && board[j + E] == 'K' && wc.first) {
          moves.push_back(Move(j + E, j + W, ' '));
        }
        if (i == H1 && board[j + W] == 'K' && wc.second) {
          moves.push_back(Move(j + W, j + E, ' '));
        }
      }
    }
  }
  return moves;
}

Position Position::rotate(bool nullmove) {
  std::string rotated_board(board.rbegin(), board.rend());
  std::transform(rotated_board.begin(), rotated_board.end(),
                 rotated_board.begin(), [](char c) {
                   return std::islower(c) ? std::toupper(c) : std::tolower(c);
                 });
  return Position(rotated_board, -score, wc, bc,
                  (ep && !nullmove) ? 119 - ep : 0,
                  (kp && !nullmove) ? 119 - kp : 0);
}

Position Position::move(Move move) {
  int i = move.i, j = move.j;
  char prom = move.prom;
  char p = board[i], q = board[j];

  auto put = [](std::string board, int i, char p) -> std::string {
    board[i] = p;
    return board;
  };

  std::string new_board = put(board, j, board[i]);
  new_board = put(new_board, i, '.');
  int new_score = score + value(move);

  std::pair<bool, bool> new_wc = {false, false};
  std::pair<bool, bool> new_bc = {false, false};

  int new_kp = 0, new_ep = 0;

  if (i == A1)
    new_wc.second = wc.second;
  if (i == H1)
    wc.second = wc.first;
  if (j == A8)
    bc.second = bc.first;
  if (j == H8)
    bc.first = bc.second;

  if (p == 'K') {
    new_wc = {false, false};

    if (std::abs(j - i) == 2) {
      new_kp = (i + j) / 2;
      new_board = put(new_board, j < i ? A1 : H1, '.');
      new_board = put(new_board, new_kp, 'R');
    }
  }

  if (p == 'P') {
    if (A8 <= j && j <= H8) {
      new_board = put(new_board, j, prom);
    }
    if (j - i == 2 * N) {
      new_ep = i + N;
    }
    if (j == ep) {
      new_board = put(new_board, j - S, '.');
    }
  }

  Position new_pos(new_board, new_score, wc, bc, new_ep, new_kp);
  return new_pos.rotate();
}

int Position::value(Move move) {
  int i = move.i;
  int j = move.j;
  char p = board[i];
  char q = board[j];
  int score = piece_square_tables[p][j] - piece_square_tables[p][i];

  // Capture
  if (islower(q)) {
    score += piece_square_tables[toupper(q)][119 - j];
  }

  // Castling check detection
  if (abs(j - kp) < 2) {
    score += piece_square_tables['K'][119 - j];
  }

  // Castling
  if (p == 'K' && abs(i - j) == 2) {
    score += piece_square_tables['R'][(i + j) / 2];
    score -= piece_square_tables['R'][j < i ? A1 : H1];
  }

  // Special pawn stuff
  if (p == 'P') {
    if (A8 <= j && j <= H8) {
      score += piece_square_tables[move.prom][j] - piece_square_tables['P'][j];
    }
    if (j == ep) {
      score += piece_square_tables['P'][119 - (j + S)];
    }
  }

  return score;
}

int Position::hash() {
  auto str_hasher = std::hash<std::string>();
  auto int_hasher = std::hash<int>();

  return (str_hasher(board) ^ int_hasher(score) ^ int_hasher(wc.first) ^
          int_hasher(wc.second) ^ int_hasher(bc.first) ^ int_hasher(bc.second) ^
          int_hasher(ep) ^ int_hasher(kp));
}