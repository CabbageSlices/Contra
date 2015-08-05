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

namespace CombinedAxis {

    enum Direction {

        UP_LEFT,
        UP,
        UP_RIGHT,
        RIGHT,
        DOWN_RIGHT,
        DOWN,
        DOWN_LEFT,
        LEFT,
    };
}

struct Direction {

    HorizontalDirection horizontal = RIGHT;
    VerticalDirection vertical = STRAIGHT;

    bool isFacingCompletelyVertical = false;
};

CombinedAxis::Direction convertToCombinedAxis(const Direction &direction);

extern const glm::vec2 downLeftVector;
extern const glm::vec2 leftVector;
extern const glm::vec2 upLeftVector;
extern const glm::vec2 upVector;
extern const glm::vec2 upRightVector;
extern const glm::vec2 rightVector;
extern const glm::vec2 downRightVector;
extern const glm::vec2 downVector;

//calculate the normalized vector that points in the given direction
//the vector is in a general object space,
//it must be transformed into world space using the transfomration of whatever object uses them
glm::vec2 getDirectionVector(const Direction& direction);

#endif // DIRECTION_H_INCLUDED
