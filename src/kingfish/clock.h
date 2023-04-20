#ifndef KINGFISH_CLOCK_H
#define KINGFISH_CLOCK_H

#include <chrono>

#include "types.h"

using Clock     = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock, Clock::duration>;

inline i64 deltaMs(TimePoint later, TimePoint earlier) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(later -
                                                                 earlier)
        .count();
}

#endif // KINGFISH_CLOCK_H