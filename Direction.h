#ifndef DIRECTION_H_INCLUDED
#define DIRECTION_H_INCLUDED

#include "glm/glm.hpp"

enum HorizontalDirection {

    LEFT,
    RIGHT
};

enum VerticalDirection {

    STRAIGHT,
    UP,
    DOWN,
};

struct Direction {

    HorizontalDirection horizontal = RIGHT;
    VerticalDirection vertical = STRAIGHT;

    bool isFacingCompletelyVertical = false;
};

//calculate the normalized vector that points in the given direction
//the vector is in a general object space,
//it must be transformed into world space using the transfomration of whatever object uses them
glm::vec2 getDirectionVector(const Direction& direction);

#endif // DIRECTION_H_INCLUDED
