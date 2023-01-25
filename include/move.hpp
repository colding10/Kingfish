#ifndef MOVE_HPP_INCLUDED
#define MOVE_HPP_INCLUDED

struct Move {
    int startX, startY, endX, endY;
    int captured;
    int value;
};

#endif  // !MOVE_HPP_INCLUDED