#include "Direction.h"

glm::vec2 getDirectionVector(const Direction& direction) {

    switch(direction) {

        case Direction::UP: {

            return glm::vec2(0, -1);
        }

        case Direction::UP_RIGHT: {

            return glm::vec2(0.7071, -0.7071);
        }

        case Direction::RIGHT: {

            return glm::vec2(1, 0);
        }

        case Direction::DOWN_RIGHT: {

            return glm::vec2(0.7071, 0.7071);
        }

        case Direction::DOWN: {

            return glm::vec2(0, 1);
        }

        case Direction::DOWN_LEFT: {

            return glm::vec2(-0.7071, 0.7071);
        }

        case Direction::LEFT: {

            return glm::vec2(-1, 0);
        }

        case Direction::UP_LEFT: {

            return glm::vec2(-0.7071, -0.7071);
        }

        default:
        return glm::vec2(0, 0);
    }

    return glm::vec2(0, 0);
}
