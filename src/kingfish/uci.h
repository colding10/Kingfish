#ifndef KINGFISH_UCI_H
#define KINGFISH_UCI_H

#include <string>

int         parse(const std::string &c);
std::string render(int i);
int         uciMainLoop();

#endif //! KINGFISH_UCI_H