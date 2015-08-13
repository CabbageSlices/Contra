#include "HitboxMovementController.h"
#include "GlobalConstants.h"
#include <glm/glm.hpp>
#include <iostream>

using std::cout;
using std::endl;

HitboxMovementController::HitboxMovementController(const glm::vec2& objectGravity, const glm::vec2& terminalVelocity, ObjectHitbox *hurtbox):
    objectHitbox(hurtbox),
    gravity(objectGravity),
    terminalVelocities(terminalVelocity),
    velocities(0, 0),
    previousDeltaTime(0)
    {

    }

void HitboxMovementController::setHitbox(ObjectHitbox *hurtbox) {

    objectHitbox = hurtbox;
}

void HitboxMovementController::setVelocities(const glm::vec2& velocity) {

    velocities = velocity;

    capVelocities();
}

void HitboxMovementController::setVelocities(const float& xVelocity, const float& yVelocity) {

    setVelocities(glm::vec2(xVelocity, yVelocity));
}

const glm::vec2 HitboxMovementController::getVelocities() const {

    return velocities;
}

const float HitboxMovementController::getLastDelta() const {

    return previousDeltaTime;
}

ObjectHitbox* HitboxMovementController::getHitbox() {

    return objectHitbox;
}

void HitboxMovementController::updateVelocities(const float& delta) {

    velocities += gravity * delta;

    capVelocities();

    previousDeltaTime = delta;
}

void HitboxMovementController::move(const float &delta) {

    if(!objectHitbox) {

        return;
    }

    previousDeltaTime = delta;

    glm::vec2 offset = velocities * METERS_TO_PIXEL_RATIO * delta;
    objectHitbox->move(offset);
}

bool HitboxMovementController::moveAlongXAxis(const float& delta, const sf::FloatRect& worldBounds) {

    if(!objectHitbox) {

        return false;
    }

    float xOffset = velocities.x * METERS_TO_PIXEL_RATIO * delta;
    objectHitbox->move(glm::vec2(xOffset, 0));

    bool snappedToEdge = false;

    float leftEdge = worldBounds.left;
    float rightEdge = worldBounds.left + worldBounds.width;

    sf::FloatRect activeHitbox = objectHitbox->getActiveHitboxWorldSpace();

    float offset = 0;
    if(activeHitbox.left < leftEdge) {

        offset = leftEdge - activeHitbox.left;
        snappedToEdge = true;

    } else if(activeHitbox.left + activeHitbox.width > rightEdge) {

        offset = rightEdge - (activeHitbox.left + activeHitbox.width);
        snappedToEdge = true;
    }

    objectHitbox->move(glm::vec2(offset, 0));

    previousDeltaTime = delta;

    return snappedToEdge;
}

bool HitboxMovementController::moveAlongYAxis(const float& delta, const sf::FloatRect& worldBounds) {

    if(!objectHitbox) {

        return false;
    }

    float yOffset = velocities.y * METERS_TO_PIXEL_RATIO * delta;
    objectHitbox->move(glm::vec2(0, yOffset));

    bool snappedToEdge = false;

    float topEdge = worldBounds.top;
    float bottomEdge = worldBounds.top + worldBounds.height;

    sf::FloatRect activeHitbox = objectHitbox->getActiveHitboxWorldSpace();

    float offset = 0;
    if(activeHitbox.top < topEdge) {

        offset = topEdge - activeHitbox.top;
        snappedToEdge = true;

    } else if(activeHitbox.top + activeHitbox.height > bottomEdge) {

        offset = bottomEdge - (activeHitbox.top + activeHitbox.height);
        snappedToEdge = true;
    }

    objectHitbox->move(glm::vec2(0, offset));

    previousDeltaTime = delta;

    return snappedToEdge;
}

void HitboxMovementController::capVelocities() {

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
