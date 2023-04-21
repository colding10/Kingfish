#ifndef KINGFISH_BITCOUNT_H
#define KINGFISH_BITCOUNT_H

#include <cassert>

#include "types.h"

enum BitCountType { CNT_64, CNT_HW_POPCNT };

/// popcount() counts the number of non-zero bits in a bitboard

template <BitCountType>
inline int popcount(uint64_t);

template <>
inline int popcount<CNT_64>(uint64_t b) {
    b -= (b >> 1) & 0x5555555555555555ULL;
    b = ((b >> 2) & 0x3333333333333333ULL) + (b & 0x3333333333333333ULL);
    b = ((b >> 4) + b) & 0x0F0F0F0F0F0F0F0FULL;
    return (b * 0x0101010101010101ULL) >> 56;
}

template <>
inline int popcount<CNT_HW_POPCNT>(uint64_t b) {
#ifndef USE_POPCNT

    assert(false);
    return b != 0; // Avoid 'b not used' warning

#elif defined(_MSC_VER) && defined(__INTEL_COMPILER)

    return _mm_popcnt_u64(b);

#elif defined(_MSC_VER)

    return (int)__popcnt64(b);

#else // Assumed gcc or compatible compiler

    return __builtin_popcountll(b);

#endif
}

inline int popcount(const Bitboard& b) {
    return popcount<CNT_64>(b.bb[0]) + popcount<CNT_64>(b.bb[1]);
}

#endif