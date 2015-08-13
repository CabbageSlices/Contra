#include "CollisionboxMovementController.h"
#include "GlobalConstants.h"
#include <glm/glm.hpp>
#include <iostream>

using std::cout;
using std::endl;

CollisionboxMovementController::CollisionboxMovementController(const glm::vec2& objectGravity, const glm::vec2& terminalVelocity, ObjectCollisionbox *collisionbox):
    objectCollisionbox(collisionbox),
    gravity(objectGravity),
    terminalVelocities(terminalVelocity),
    velocities(0, 0),
    previousDeltaTime(0)
    {

    }

void CollisionboxMovementController::setCollisionbox(ObjectCollisionbox *collisionbox) {

    objectCollisionbox = collisionbox;
}

void CollisionboxMovementController::setVelocities(const glm::vec2& velocity) {

    velocities = velocity;

    capVelocities();
}

void CollisionboxMovementController::setVelocities(const float& xVelocity, const float& yVelocity) {

    setVelocities(glm::vec2(xVelocity, yVelocity));
}

const glm::vec2 CollisionboxMovementController::getVelocities() const {

    return velocities;
}

const float CollisionboxMovementController::getLastDelta() const {

    return previousDeltaTime;
}

ObjectCollisionbox* CollisionboxMovementController::getCollisionbox() {

    return objectCollisionbox;
}

void CollisionboxMovementController::updateVelocities(const float& delta) {

    velocities += gravity * delta;

    capVelocities();

    previousDeltaTime = delta;
}

void CollisionboxMovementController::move(const float &delta) {

    if(!objectCollisionbox) {

        return;
    }

    previousDeltaTime = delta;

    glm::vec2 offset = velocities * METERS_TO_PIXEL_RATIO * delta;
    objectCollisionbox->move(offset);
}

bool CollisionboxMovementController::moveAlongXAxis(const float& delta, const sf::FloatRect& worldBounds) {

    if(!objectCollisionbox) {

        return false;
    }

    float xOffset = velocities.x * METERS_TO_PIXEL_RATIO * delta;
    objectCollisionbox->move(glm::vec2(xOffset, 0));

    bool snappedToEdge = false;

    float leftEdge = worldBounds.left;
    float rightEdge = worldBounds.left + worldBounds.width;

    sf::FloatRect activeCollisionbox = objectCollisionbox->getActiveCollisionboxWorldSpace();

    float offset = 0;
    if(activeCollisionbox.left < leftEdge) {

        offset = leftEdge - activeCollisionbox.left;
        snappedToEdge = true;

    } else if(activeCollisionbox.left + activeCollisionbox.width > rightEdge) {

        offset = rightEdge - (activeCollisionbox.left + activeCollisionbox.width);
        snappedToEdge = true;
    }

    objectCollisionbox->move(glm::vec2(offset, 0));

    previousDeltaTime = delta;

    return snappedToEdge;
}

bool CollisionboxMovementController::moveAlongYAxis(const float& delta, const sf::FloatRect& worldBounds) {

    if(!objectCollisionbox) {

        return false;
    }

    float yOffset = velocities.y * METERS_TO_PIXEL_RATIO * delta;
    objectCollisionbox->move(glm::vec2(0, yOffset));

    bool snappedToEdge = false;

    float topEdge = worldBounds.top;
    float bottomEdge = worldBounds.top + worldBounds.height;

    sf::FloatRect activeCollisionbox = objectCollisionbox->getActiveCollisionboxWorldSpace();

    float offset = 0;
    if(activeCollisionbox.top < topEdge) {

        offset = topEdge - activeCollisionbox.top;
        snappedToEdge = true;

    } else if(activeCollisionbox.top + activeCollisionbox.height > bottomEdge) {

        offset = bottomEdge - (activeCollisionbox.top + activeCollisionbox.height);
        snappedToEdge = true;
    }

    objectCollisionbox->move(glm::vec2(0, offset));

    previousDeltaTime = delta;

    return snappedToEdge;
}

void CollisionboxMovementController::capVelocities() {

    if(velocities.x > terminalVelocities.x) {

        velocities.x = terminalVelocities.x;
    }

    if(velocities.x < -terminalVelocities.x) {

        velocities.x = -terminalVelocities.x;
    }

    if(velocities.y > terminalVelocities.y) {

        velocities.y = terminalVelocities.y;
    }

    if(velocities.y < -terminalVelocities.y) {

        velocities.y = -terminalVelocities.y;
    }
}
