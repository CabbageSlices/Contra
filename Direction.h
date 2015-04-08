#ifndef DIRECTION_H_INCLUDED
#define DIRECTION_H_INCLUDED

#include "glm/glm.hpp"

enum Direction {

    UP,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT,
};

//calculate the normalized vector that points in the given direction
//the vector is in a general object space,
//it must be transformed into world space using the transfomration of whatever object uses them
glm::vec2 getDirectionVector(const Direction& direction);

#endif // DIRECTION_H_INCLUDED
