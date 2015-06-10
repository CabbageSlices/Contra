#include "TurretEnemy.h"
#include "GlobalConstants.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;

TurretEnemy::TurretEnemy(const glm::vec2 &position, const int initialHealth) :
    ShootingEntity(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), initialHealth)
    {
        entity.setSize(sf::Vector2f(128, 128));
        hitbox.setOrigin(position);
        hitbox.insertHitbox(sf::FloatRect(0, 0, entity.getSize().x, entity.getSize().y));
        hitbox.setActiveHitbox(0);
        entity.setPosition(position.x, position.y);

        gun = make_shared<Gun>();
    }

void TurretEnemy::updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map, const vector<glm::vec2> &targetPositions) {

    //update previous bullets
    gun->updatePhysics(deltaTime, worldBounds, map);

    //start shooting at the closest player
    unsigned idClosestTarget = getIdOfClosestTarget(targetPositions);

    if(idClosestTarget < 0) {

        return;
    }

    glm::vec2 gunfireOrigin = calculateGunfireOrigin(targetPositions[idClosestTarget]);
    determineDirection(targetPositions[idClosestTarget]);
    gun->fire(hitbox.getOrigin(), gunfireOrigin, direction);
}

void TurretEnemy::updateRendering() {

    gun->updateRendering();
}

void TurretEnemy::createHitboxes(const vector<sf::FloatRect> &hitboxes) {

    hitbox.clearHitboxes();

    for(unsigned i = 0; i < hitboxes.size(); ++i) {

        hitbox.insertHitbox(hitboxes[i]);
    }
}

unsigned TurretEnemy::getIdOfClosestTarget(const vector<glm::vec2> &targetPositions) const {

    if(targetPositions.size() == 0) {

        return -1;
    }

    unsigned closestId = 0;
    float distanceToClosest = glm::dot(targetPositions[0] - hitbox.getOrigin(), targetPositions[0] - hitbox.getOrigin());

    for(unsigned i = 1; i < targetPositions.size(); ++i) {

        float distanceToTarget = glm::dot(targetPositions[i] - hitbox.getOrigin(), targetPositions[i] - hitbox.getOrigin());

        if(distanceToTarget < distanceToClosest) {

            distanceToClosest = distanceToTarget;
            closestId = i;
        }
    }

    return closestId;
}

glm::vec2 TurretEnemy::calculateGunfireOrigin(const glm::vec2 &targetPosition) const {

    //for now just shoot from the center
    sf::FloatRect bounds = hitbox.getActiveHitboxObjectSpace();
    return glm::vec2(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
}

void TurretEnemy::determineDirection(const glm::vec2 &targetPosition) {

    //determine angle to target
    float x = targetPosition.x - hitbox.getOrigin().x;
    float y = targetPosition.y - hitbox.getOrigin().y;

    float angleRadians = atan2(y, x);

    //angle goes positive in clockwise direction when tis hould be counter clockwise so negate it
    float angleDegrees = angleRadians * RAD_TO_DEG_RATIO * -1;

    direction.isFacingCompletelyVertical = false;

    if(angleDegrees <= 60 && angleDegrees >= -60) {

        direction.horizontal = RIGHT;

    } else if(angleDegrees >= 120 || angleDegrees <= -120) {

        direction.horizontal = LEFT;

    } else {

        direction.isFacingCompletelyVertical = true;
    }

    if(angleDegrees <= 150 && angleDegrees >= 30) {

        direction.vertical = VerticalDirection::UP;

    } else if(angleDegrees <= -30 && angleDegrees >= -150) {

        direction.vertical = VerticalDirection::DOWN;

    } else {

        direction.vertical = VerticalDirection::STRAIGHT;
    }
}