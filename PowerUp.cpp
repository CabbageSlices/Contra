#include "PowerUp.h"
#include "GlobalConstants.h"
#include "EntityAnimationStates.h"
#include <iostream>

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;

void applyPowerUp(shared_ptr<Gun> &gun, const PowerUpType &powerUpType) {

    switch(powerUpType) {

        case(PowerUpType::MACHINE_GUN): {

            gun = make_shared<Gun>(BulletType::BULLET_FAST, FIRE_DELAY_SHORT);
            break;
        }

        default: {

            gun = make_shared<Gun>(BulletType::BULLET_MEDIUM, FIRE_DELAY_MEDIUM);
        }
    }
}

PowerUp::PowerUp(const glm::vec2 &positionWorldSpace, const PowerUpType &powerUp, const PreloadedPowerUpData &powerupData) :
    DynamicObject(glm::vec2(0, GRAVITY), glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY) / 3.f, glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY), 1),
    powerUpType(powerUp)
    {
        load(powerupData);
        setPosition(positionWorldSpace);
    }

void PowerUp::updatePhysics(const float &deltaTime, const sf::FloatRect &worldBounds, TileMap &map) {

    //don't move powerup if its spawning or disappearing
    if(!currentState != PowerUpEnums::STATE_SPAWNED) {

        return;
    }

    hitboxMovementController.updateVelocities(deltaTime);

    hitboxMovementController.moveAlongXAxis(deltaTime, worldBounds);
    handleTileCollisionHorizontally(map);

    hitboxMovementController.moveAlongYAxis(deltaTime, worldBounds);
    handleTileCollisionVertically(map);
}

void PowerUp::updateRendering() {

    if(sprite.animate()) {

        if(currentState == PowerUpEnums::STATE_SPAWNING) {

            setState(PowerUpEnums::STATE_SPAWNED);

        } else if(currentState == PowerUpEnums::STATE_DISAPPEARING) {

            //set health to 0 so object is listed as dead
            setHealth(0);
            setState(PowerUpEnums::STATE_DISAPPEARED);
        }
    }

    if(currentState == PowerUpEnums::STATE_SPAWNED) {

        //make sure the correct image for this powerup is being displayed
        setFrameForPowerUpType();
    }

    sprite.getSprite().setPosition(hurtbox.getOrigin().x, hurtbox.getOrigin().y);
}

void PowerUp::draw(sf::RenderWindow &window) {

    sprite.draw(window);
}

bool PowerUp::checkCanGetHit() {

    return currentState == PowerUpEnums::STATE_SPAWNED;
}

void PowerUp::getHit(int damage) {

    setState(PowerUpEnums::STATE_DISAPPEARING);
}

PowerUpType PowerUp::getPowerUpType() const {

    return powerUpType;
}

void PowerUp::load(const PreloadedPowerUpData &data) {

    loadBase(data);

    setState(PowerUpEnums::STATE_SPAWNING);
}

void PowerUp::setFrameForPowerUpType() {

    if(powerUpType == PowerUpType::MACHINE_GUN) {

        sprite.setFrame(PowerUpEnums::FRAME_MACHINEGUN);
        hurtbox.setActiveHitbox(PowerUpEnums::FRAME_MACHINEGUN, PowerUpEnums::STATE_SPAWNED);
    }
}
