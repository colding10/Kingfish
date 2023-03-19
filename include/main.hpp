#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <atomic>
#include <map>
#include <string>
#include <tuple>

#include "board.hpp"
#include "transtable.hpp"

static std::atomic<bool> should_stop(false);

#endif  // !MAIN_HPP_INCLUDED