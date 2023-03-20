#include "transtable.hpp"

void TranspositionTable::put(long long hash, Entry entry) {
  table[hash] = entry;
}

bool TranspositionTable::get(long long hash, Entry &entry) {
  auto it = table.find(hash);
  if (it != table.end()) {
    entry = it->second;
    return true;
  } else {
    return false;
  }
}
