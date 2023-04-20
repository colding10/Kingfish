#ifndef KINGFISH_TIMEMANAGER_H
#define KINGFISH_TIMEMANAGER_H

#include <string>
#include <vector>

#include "../position.h"

int getSearchTime(const std::vector<std::string> &args,
                  const std::vector<Position>    &hist);

#endif