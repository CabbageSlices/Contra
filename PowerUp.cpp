#include "PowerUp.h"
#include "GlobalConstants.h"
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
    STATE_SPAWNING(0),
    STATE_SPAWNED(0),
    STATE_DISAPPEARING(0),
    STATE_DISAPPEARED(0),
    FRAME_MACHINEGUN(0),
    powerUpType(powerUp)
    {
        load(powerupData);
        setPosition(positionWorldSpace);
    }

void PowerUp::updatePhysics(const float &deltaTime, const sf::FloatRect &worldBounds, TileMap &map) {

    //don't move powerup if its spawning or disappearing
    if(!currentState != STATE_SPAWNED) {

        return;
    }

    CollisionboxMovementController.updateVelocities(deltaTime);

    CollisionboxMovementController.moveAlongXAxis(deltaTime, worldBounds);
    handleTileCollisionHorizontally(map);

    CollisionboxMovementController.moveAlongYAxis(deltaTime, worldBounds);
    handleTileCollisionVertically(map);
}

void PowerUp::updateRendering() {

    if(sprite.animate()) {

        if(currentState == STATE_SPAWNING) {

            setState(STATE_SPAWNED);

        } else if(currentState == STATE_DISAPPEARING) {

            //set health to 0 so object is listed as dead
            setHealth(0);
            setState(STATE_DISAPPEARED);
        }
    }

    if(currentState == STATE_SPAWNED) {

        //make sure the correct image for this powerup is being displayed
        setFrameForPowerUpType();
    }

    sprite.getSprite().setPosition(Collisionbox.getOrigin().x, Collisionbox.getOrigin().y);
}

void PowerUp::draw(sf::RenderWindow &window) {

    sprite.draw(window);
}

bool PowerUp::checkCanGetHit() {

    return currentState == STATE_SPAWNED;
}

void PowerUp::getHit(int damage) {

    setState(STATE_DISAPPEARING);
}

PowerUpType PowerUp::getPowerUpType() const {

    return powerUpType;
}

void PowerUp::load(const PreloadedPowerUpData &data) {

    loadBase(data);

    STATE_SPAWNING = data.STATE_SPAWNING;
    STATE_SPAWNED = data.STATE_SPAWNED;
    STATE_DISAPPEARING = data.STATE_DISAPPEARING;
    STATE_DISAPPEARED = data.STATE_DISAPPEARED;

    FRAME_MACHINEGUN = data.FRAME_MACHINEGUN;

    setState(STATE_SPAWNING);
}

void PowerUp::setFrameForPowerUpType() {

    if(powerUpType == PowerUpType::MACHINE_GUN) {

        sprite.setFrame(FRAME_MACHINEGUN);
        Collisionbox.setActiveCollisionbox(FRAME_MACHINEGUN, STATE_SPAWNED);
    }
}
