#include "Direction.h"

const glm::vec2 downLeftVector(-0.707, 0.707);
const glm::vec2 leftVector(-1, 0);
const glm::vec2 upLeftVector(-0.707, -0.707);
const glm::vec2 upVector(0, -1);
const glm::vec2 upRightVector(0.707, -0.707);
const glm::vec2 rightVector(1, 0);
const glm::vec2 downRightVector(0.707, 0.707);
const glm::vec2 downVector(0, 1);

glm::vec2 getDirectionVector(const Direction& direction) {

    if(direction.isFacingCompletelyVertical && direction.vertical != VerticalDirection::STRAIGHT) {

        if(direction.vertical == VerticalDirection::UP) {

            return upVector;

        } else if(direction.vertical == VerticalDirection::DOWN) {

            return downVector;
        }
    }

    if(direction.horizontal == HorizontalDirection::LEFT) {

        if(direction.vertical == VerticalDirection::STRAIGHT) {

            return leftVector;

        } else if(direction.vertical == VerticalDirection::UP) {

            return upLeftVector;

        } else if(direction.vertical == VerticalDirection::DOWN) {

            return downLeftVector;
        }

    } else if(direction.horizontal == HorizontalDirection::RIGHT) {

        if(direction.vertical == VerticalDirection::STRAIGHT) {

            return rightVector;

        } else if(direction.vertical == VerticalDirection::UP) {

            return upRightVector;

        } else if(direction.vertical == VerticalDirection::DOWN) {

            return downRightVector;
        }
    }

    return glm::vec2(0, 0);
}

CombinedAxis::Direction convertToCombinedAxis(const Direction &direction) {

    if(direction.isFacingCompletelyVertical && direction.vertical != VerticalDirection::STRAIGHT) {

        if(direction.vertical == VerticalDirection::UP) {

            return CombinedAxis::UP;

        } else if(direction.vertical == VerticalDirection::DOWN) {

            return CombinedAxis::DOWN;
        }
    }

    if(direction.horizontal == HorizontalDirection::LEFT) {

        if(direction.vertical == VerticalDirection::STRAIGHT) {

            return CombinedAxis::LEFT;

        } else if(direction.vertical == VerticalDirection::UP) {

            return CombinedAxis::UP_LEFT;

        } else if(direction.vertical == VerticalDirection::DOWN) {

            return CombinedAxis::DOWN_LEFT;
        }

    } else if(direction.horizontal == HorizontalDirection::RIGHT) {

        if(direction.vertical == VerticalDirection::STRAIGHT) {

            return CombinedAxis::RIGHT;

        } else if(direction.vertical == VerticalDirection::UP) {

            return CombinedAxis::UP_RIGHT;

        } else if(direction.vertical == VerticalDirection::DOWN) {

            return CombinedAxis::DOWN_RIGHT;
        }
    }

    return CombinedAxis::RIGHT;
}
