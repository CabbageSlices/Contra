#include "OmniDirectionalTurret.h"
#include "TileMap.h"
#include <memory>
#include <iostream>

using std::cout;
using std::endl;
using std::make_shared;

OmniDirectionalTurret::OmniDirectionalTurret(const glm::vec2 &position, const int initialHealth) :
    ShootingEntity(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), initialHealth),
    hiddenStateTimer(),
    exposedStateTimer(),
    shootingTimer(),
    hiddenStateDuration(),
    exposedStateDuration(),
    shootingDelay(),
    STATE_HIDING(0),
    STATE_COMING_OUT_OF_HIDING(0),
    STATE_GOING_INTO_HIDING(0)
    {
        collisionbox.setOrigin(position);

        //this type of turret enemy's fire delay is determined by the object itself, and not its gun
        gun = make_shared<Gun>();
        gun->setFireDelay(sf::seconds(0));
    }

void OmniDirectionalTurret::updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map) {

    //update previous bullets
    gun->updatePhysics(deltaTime, worldBounds, map);

    unsigned frame = sprite.getFrame();

    collisionbox.setActiveCollisionbox(frame, currentState);

    if(checkCanShoot()) {

        beginShootingAnimation();
    }
}

void OmniDirectionalTurret::updateRendering() {

    gun->updateRendering();

    if(sprite.animate()) {

        if(currentState == STATE_COMING_OUT_OF_HIDING) {

            setState(STATE_EXPOSED);
            exposedStateTimer.restart();
            shootingTimer.restart();

        } else if(currentState == STATE_GOING_INTO_HIDING) {

            setState(STATE_HIDING);
            hiddenStateTimer.restart();

        } else if(currentState == STATE_SHOOTING) {

            shoot();
            shootingTimer.restart();
            setState(STATE_EXPOSED);
        }
    }

    if(currentState == STATE_HIDING && hiddenStateTimer.getElapsedTime() > hiddenStateDuration) {

        setState(STATE_COMING_OUT_OF_HIDING);

    } else if(currentState == STATE_EXPOSED && exposedStateTimer.getElapsedTime() > exposedStateDuration) {

        setState(STATE_GOING_INTO_HIDING);
    }

    collisionbox.setActiveCollisionbox(sprite.getFrame(), currentState);

    glm::vec2 position = collisionbox.getOrigin();
    sprite.getSprite().setPosition(position.x, position.y);
}

bool OmniDirectionalTurret::checkCanGetHit() {

    return checkIsAlive() && currentState != STATE_HIDING;
}

void OmniDirectionalTurret::draw(sf::RenderWindow &window) {

    gun->draw(window);
    sprite.draw(window);
}

void OmniDirectionalTurret::load(PreloadedOmniDirectionalTurretData &data) {

    loadBase(data);
    loadShootingEntityData(data);
    scale(data.scale, data.scale);

    STATE_HIDING = data.STATE_HIDING;
    STATE_COMING_OUT_OF_HIDING = data.STATE_COMING_OUT_OF_HIDING;
    STATE_GOING_INTO_HIDING = data.STATE_GOING_INTO_HIDING;
    STATE_EXPOSED = data.STATE_EXPOSED;
    STATE_SHOOTING = data.STATE_SHOOTING;

    hiddenStateDuration = data.hiddenStateDuration;
    exposedStateDuration = data.exposedStateDuration;
    shootingDelay = data.shootingDelay;

    setState(STATE_HIDING);
    hiddenStateTimer.restart();
    exposedStateTimer.restart();
    shootingTimer.restart();
}

bool OmniDirectionalTurret::checkCanShoot() {

    bool isExposed = (currentState == STATE_EXPOSED);
    bool isDelayOver = (shootingTimer.getElapsedTime() > shootingDelay);

    return isExposed && isDelayOver;
}

void OmniDirectionalTurret::beginShootingAnimation() {

    setState(STATE_SHOOTING);
}

void OmniDirectionalTurret::shoot() {

    //shoot in 8 directions
    glm::vec2 bulletOrigin(140, 61);

    gun->fire(collisionbox.getOrigin(), bulletOriginForState[CombinedAxis::LEFT], leftVector);
    gun->fire(collisionbox.getOrigin(), bulletOriginForState[CombinedAxis::UP_LEFT], upLeftVector);
    gun->fire(collisionbox.getOrigin(), bulletOriginForState[CombinedAxis::UP], upVector);
    gun->fire(collisionbox.getOrigin(), bulletOriginForState[CombinedAxis::UP_RIGHT], upRightVector);
    gun->fire(collisionbox.getOrigin(), bulletOriginForState[CombinedAxis::RIGHT], rightVector);
    gun->fire(collisionbox.getOrigin(), bulletOriginForState[CombinedAxis::DOWN_RIGHT], downRightVector);
    gun->fire(collisionbox.getOrigin(), bulletOriginForState[CombinedAxis::DOWN], downVector);
    gun->fire(collisionbox.getOrigin(), bulletOriginForState[CombinedAxis::DOWN_LEFT], downLeftVector);
}
