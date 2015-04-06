#include "PositionController.h"
#include "GlobalConstants.h"
#include <glm/glm.hpp>
#include <iostream>

using std::cout;
using std::endl;

PositionController::PositionController(const glm::vec2& sizeObjectSpace, const glm::vec2& gravityInObjectSpace, const glm::vec2& terminalVelocityObjectSpace, const glm::vec2& xAxis, const glm::vec2& yAxis):
    object(xAxis, yAxis, sizeObjectSpace),
    gravityObjectSpace(gravityInObjectSpace),
    terminalVelocitiesObjectSpace(terminalVelocityObjectSpace),
    velocitiesObjectSpace(0, 0),
    previousDeltaTime(0)
    {

    }

void PositionController::setVelocities(const glm::vec2& velocityObjectSpace) {

    velocitiesObjectSpace = velocityObjectSpace;

    capVelocities();
}

void PositionController::setVelocities(const float& xVelocityObjectSpace, const float& yVelocityObjectSpace) {

    setVelocities(glm::vec2(xVelocityObjectSpace, yVelocityObjectSpace));
}

void PositionController::setPositionWorldSpace(const glm::vec2& position) {

    object.setPositionWorldSpace(position);
}

void PositionController::setPositionObjectSpace(const glm::vec2& position) {

    object.setPositionObjectSpace(position);
}

const glm::vec2 PositionController::getVelocitiesObjectSpace() const {

    return velocitiesObjectSpace;
}

const glm::vec2 PositionController::getVelocitiesWorldSpace() const {

    return object.convertToWorldSpace(velocitiesObjectSpace);
}

const sf::FloatRect PositionController::getBoundingBoxWorldSpace() const {

    return object.getBoundingBoxWorldSpace();
}

const sf::FloatRect PositionController::getBoundingBoxObjectSpace() const {

    return object.getBoundingBoxObjectSpace();
}

const float PositionController::getLastDelta() const {

    return previousDeltaTime;
}

const ObjectSpaceManager& PositionController::getObjectSpace() const {

    return object;
}

void PositionController::updateVelocities(const float& delta) {

    velocitiesObjectSpace += gravityObjectSpace * delta;

    capVelocities();

    previousDeltaTime = delta;
}

bool PositionController::moveAlongXAxis(const float& delta, const sf::FloatRect& worldBounds) {

    float xOffset = velocitiesObjectSpace.x * METERS_TO_PIXEL_RATIO* delta;
    object.move(glm::vec2(xOffset, 0));

    //calculate the bounds of the world in object space and do bounds checking in object space
    glm::vec2 boundsPosition(worldBounds.left, worldBounds.top);
    glm::vec2 boundsSize(worldBounds.width, worldBounds.height);

    glm::vec2 boundsPositionObjectSpace = object.convertToObjectSpace(boundsPosition);
    glm::vec2 boundsSizeObjectSpace = object.convertToObjectSpace(boundsSize);

    //make sure the x position referes to left edge and left + width revers to right edge
    //after doing a space transformation this may change due to multiplication by a negative
    float leftEdge = glm::min(boundsPositionObjectSpace.x, boundsPositionObjectSpace.x + boundsSizeObjectSpace.x);
    float rightEdge = glm::max(boundsPositionObjectSpace.x, boundsPositionObjectSpace.x + boundsSizeObjectSpace.x);

    bool snappedToEdge = false;

    if(object.getPositionObjectSpace().x < leftEdge) {

        snappedToEdge = true;
        object.setPositionObjectSpace(glm::vec2(leftEdge, object.getPositionObjectSpace().y));
    }

    if(object.getPositionObjectSpace().x + object.getSizeObjectSpace().x > rightEdge) {

        snappedToEdge = true;
        object.setPositionObjectSpace(glm::vec2(rightEdge - object.getSizeObjectSpace().x, object.getPositionObjectSpace().y));
    }

    previousDeltaTime = delta;

    return snappedToEdge;
}

bool PositionController::moveAlongYAxis(const float& delta, const sf::FloatRect& worldBounds) {

    float yOffset = velocitiesObjectSpace.y * METERS_TO_PIXEL_RATIO * delta;
    object.move(glm::vec2(0, yOffset));

    //calculate the bounds of the world in object space and do bounds checking in object space
    glm::vec2 boundsPosition(worldBounds.left, worldBounds.top);
    glm::vec2 boundsSize(worldBounds.width, worldBounds.height);

    glm::vec2 boundsPositionObjectSpace = object.convertToObjectSpace(boundsPosition);
    glm::vec2 boundsSizeObjectSpace = object.convertToObjectSpace(boundsSize);

    //make sure the top position referes to top edge and top + height revers to right edge
    //after doing a space transformation this may change due to multiplication by a negative
    float topEdge = glm::min(boundsPositionObjectSpace.y, boundsPositionObjectSpace.y + boundsSizeObjectSpace.y);
    float bottomEdge = glm::max(boundsPositionObjectSpace.y, boundsPositionObjectSpace.y + boundsSizeObjectSpace.y);

    bool snappedToEdge = false;

    if(object.getPositionObjectSpace().y < topEdge) {

        snappedToEdge = true;
        object.setPositionObjectSpace(glm::vec2(object.getPositionObjectSpace().x, topEdge));
    }

    if(object.getPositionObjectSpace().y + object.getSizeObjectSpace().y > bottomEdge) {

        snappedToEdge = true;
        object.setPositionObjectSpace(glm::vec2(object.getPositionObjectSpace().x, bottomEdge - object.getSizeObjectSpace().y));
    }

    previousDeltaTime = delta;

    return snappedToEdge;
}

void PositionController::capVelocities() {

    if(velocitiesObjectSpace.x > terminalVelocitiesObjectSpace.x) {

        velocitiesObjectSpace.x = terminalVelocitiesObjectSpace.x;
    }

    if(velocitiesObjectSpace.x < -terminalVelocitiesObjectSpace.x) {

        velocitiesObjectSpace.x = -terminalVelocitiesObjectSpace.x;
    }

    if(velocitiesObjectSpace.y > terminalVelocitiesObjectSpace.y) {

        velocitiesObjectSpace.y = terminalVelocitiesObjectSpace.y;
    }

    if(velocitiesObjectSpace.y < -terminalVelocitiesObjectSpace.y) {

        velocitiesObjectSpace.y = -terminalVelocitiesObjectSpace.y;
    }
}
