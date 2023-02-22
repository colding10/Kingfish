#include "transtable.hpp"

void TranspositionTable::put(int key, int depth, int value, int alpha,
                             int beta) {
  Entry entry = {depth, value, alpha, beta};
  table[key] = entry;
}

bool TranspositionTable::get(int key, int depth, int alpha, int beta,
                             int &value) {
  auto it = table.find(key);
  if (it != table.end() && it->second.depth >= depth) {
    Entry &entry = it->second;
    if (entry.alpha <= alpha && entry.beta >= beta) {
      value = entry.value;
      return true;
    }
  }
  return false;
}