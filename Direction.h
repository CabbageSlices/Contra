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

    enum Direction : unsigned {

        UP_LEFT = 2100,
        UP = 2101,
        UP_RIGHT = 2102,
        RIGHT = 2103,
        DOWN_RIGHT = 2104,
        DOWN = 2105,
        DOWN_LEFT = 2107,
        LEFT = 2108,
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
