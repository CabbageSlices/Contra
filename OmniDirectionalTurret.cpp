#include "OmniDirectionalTurret.h"
#include "EntityAnimationStates.h"
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
    shootingDelay()
    {
        setPosition(position);

        //this type of turret enemy's fire delay is determined by the object itself, and not its gun
        gun = make_shared<Gun>();
        gun->setFireDelay(sf::seconds(0));
    }

void OmniDirectionalTurret::updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map) {

    //update previous bullets
    gun->updatePhysics(deltaTime, worldBounds, map);

    unsigned frame = sprite.getFrame();

    hurtbox.setActiveHitbox(frame, currentState);

    if(checkCanShoot()) {

        beginShootingAnimation();
    }

    matchHitboxPosition();
}

void OmniDirectionalTurret::updateRendering() {

    gun->updateRendering();

    if(sprite.animate()) {

        if(currentState == OmniDirectionalTurretEnums::STATE_COMING_OUT_OF_HIDING) {

            setState(OmniDirectionalTurretEnums::STATE_EXPOSED);
            exposedStateTimer.restart();
            shootingTimer.restart();

        } else if(currentState == OmniDirectionalTurretEnums::STATE_GOING_INTO_HIDING) {

            setState(OmniDirectionalTurretEnums::STATE_HIDING);
            hiddenStateTimer.restart();

        } else if(currentState == OmniDirectionalTurretEnums::STATE_SHOOTING) {

            shoot();
            shootingTimer.restart();
            setState(OmniDirectionalTurretEnums::STATE_EXPOSED);
        }
    }

    if(currentState == OmniDirectionalTurretEnums::STATE_HIDING && hiddenStateTimer.getElapsedTime() > hiddenStateDuration) {

        setState(OmniDirectionalTurretEnums::STATE_COMING_OUT_OF_HIDING);

    } else if(currentState == OmniDirectionalTurretEnums::STATE_EXPOSED && exposedStateTimer.getElapsedTime() > exposedStateDuration) {

        setState(OmniDirectionalTurretEnums::STATE_GOING_INTO_HIDING);
    }

    hurtbox.setActiveHitbox(sprite.getFrame(), currentState);

    matchHitboxPosition();
}

bool OmniDirectionalTurret::checkCanGetHit() {

    return checkIsAlive() && currentState != OmniDirectionalTurretEnums::STATE_HIDING;
}

void OmniDirectionalTurret::draw(sf::RenderWindow &window) {

    gun->draw(window);
    sprite.draw(window);
}

void OmniDirectionalTurret::load(const PreloadedOmniDirectionalTurretData &data) {

    loadBase(data);
    loadShootingEntityData(data);
    scale(data.scale, data.scale);

    hiddenStateDuration = data.hiddenStateDuration;
    exposedStateDuration = data.exposedStateDuration;
    shootingDelay = data.shootingDelay;

    setState(OmniDirectionalTurretEnums::STATE_COMING_OUT_OF_HIDING);
    hiddenStateTimer.restart();
    exposedStateTimer.restart();
    shootingTimer.restart();
}

bool OmniDirectionalTurret::checkCanShoot() {

    bool isExposed = (currentState == OmniDirectionalTurretEnums::STATE_EXPOSED);
    bool isDelayOver = (shootingTimer.getElapsedTime() > shootingDelay);

    return isExposed && isDelayOver;
}

void OmniDirectionalTurret::beginShootingAnimation() {

    setState(OmniDirectionalTurretEnums::STATE_SHOOTING);
}

void OmniDirectionalTurret::shoot() {

    //shoot in 8 directions
    glm::vec2 bulletOrigin(140, 61);

    gun->fire(hurtbox.getOrigin(), bulletOriginForState[CombinedAxis::LEFT], leftVector);
    gun->fire(hurtbox.getOrigin(), bulletOriginForState[CombinedAxis::UP_LEFT], upLeftVector);
    gun->fire(hurtbox.getOrigin(), bulletOriginForState[CombinedAxis::UP], upVector);
    gun->fire(hurtbox.getOrigin(), bulletOriginForState[CombinedAxis::UP_RIGHT], upRightVector);
    gun->fire(hurtbox.getOrigin(), bulletOriginForState[CombinedAxis::RIGHT], rightVector);
    gun->fire(hurtbox.getOrigin(), bulletOriginForState[CombinedAxis::DOWN_RIGHT], downRightVector);
    gun->fire(hurtbox.getOrigin(), bulletOriginForState[CombinedAxis::DOWN], downVector);
    gun->fire(hurtbox.getOrigin(), bulletOriginForState[CombinedAxis::DOWN_LEFT], downLeftVector);
}
