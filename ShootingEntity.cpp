#include "ShootingEntity.h"

using std::shared_ptr;

ShootingEntity::ShootingEntity(const glm::vec2 &gravity, const glm::vec2 &movementVelocity, const glm::vec2 &terminalVelocity, const unsigned &initialHealth) :
    EntityBase(gravity, movementVelocity, terminalVelocity, initialHealth),
    gun(),
    direction(),
    bulletOriginForState()
    {

    }

void ShootingEntity::draw(sf::RenderWindow &window) {

    if(gun) {

        gun->draw(window);
    }

    EntityBase::draw(window);
}

shared_ptr<Gun>& ShootingEntity::getGun() {

    return gun;
}

void ShootingEntity::scaleComponents(const float &xFactor, const float &yFactor) {

    for(auto &pair : bulletOriginForState) {

        pair.second.x *= xFactor;
        pair.second.y *= yFactor;
    }
}
