#include "Enemy.h"
#include "GlobalConstants.h"
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;

Enemy::Enemy(const glm::vec2 &positionWorldSpace, const Direction &initialDirection, const int initialHealth) :
    EntityBase(glm::vec2(0, GRAVITY), glm::vec2(TERMINAL_VELOCITY / 5, 0), glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY), initialHealth),
    STATE_WALKING_LEFT(0),
    STATE_WALKING_RIGHT(0),
    STATE_FALLING_LEFT(0),
    STATE_FALLING_RIGHT(0),
    currentState(0),
    sprite(sf::milliseconds(75)),
    direction()
    {
        hitbox.setOrigin(positionWorldSpace);

        if(initialDirection.horizontal == HorizontalDirection::RIGHT) {

            setInitialVelocity(glm::vec2(MOVEMENT_VELOCITY.x, 0));

        } else {

            setInitialVelocity(glm::vec2(-MOVEMENT_VELOCITY.x, 0));
        }
    }

void Enemy::updatePhysics(const float &deltaTime, const sf::FloatRect &worldBounds, TileMap &map) {

    hitboxMovementController.updateVelocities(deltaTime);

    if(hitboxMovementController.moveAlongXAxis(deltaTime, worldBounds)) {

        changeDirectionHorizontally();
    }

    handleTileCollisionHorizontally(map);

    if(hitboxMovementController.moveAlongYAxis(deltaTime, worldBounds)) {

        hitboxMovementController.setVelocities(hitboxMovementController.getVelocities().x, 0);
    }

    handleTileCollisionVertically(map);

    determineHorizontalDirection();
}

void Enemy::updateRendering() {

    determineAnimationState();
    sprite.animate();

    sprite.getSprite().setPosition(hitbox.getOrigin().x, hitbox.getOrigin().y);
}

void Enemy::setInitialVelocity(const glm::vec2 &velocity) {

    hitboxMovementController.setVelocities(velocity);
}

void Enemy::respondToCollision(const CollisionResponse &response) {

    if(response.handledHorizontal) {

        changeDirectionHorizontally();
    }
}

CollisionResponse Enemy::handleTileCollision(TileMap &map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object)) {

    vector<shared_ptr<Tile> > tiles = getSurroundingTiles(map, glm::vec2(TILE_SIZE, TILE_SIZE));

    CollisionResponse response;

    for(unsigned i = 0; i < tiles.size(); ++i) {

        CollisionResponse currentResponse = collisionFunction(tiles[i], hitboxMovementController);

        if(currentResponse.handledHorizontal) {

            response.handledHorizontal = true;
            hitboxMovementController.setVelocities(-hitboxMovementController.getVelocities().x, hitboxMovementController.getVelocities().y);
        }

        response.pushedToTop = currentResponse.pushedToTop || response.pushedToTop;
    }

    return response;
}

void Enemy::draw(sf::RenderWindow &window) {

    sprite.draw(window);
}

void Enemy::load(PreloadedEnemyData &data) {

    STATE_WALKING_LEFT = data.STATE_WALKING_LEFT;
    STATE_WALKING_RIGHT = data.STATE_WALKING_RIGHT;
    STATE_FALLING_LEFT = data.STATE_FALLING_LEFT;
    STATE_FALLING_RIGHT = data.STATE_FALLING_RIGHT;

    setHealth(data.health);

    sprite.loadTexture(data.textureFileName);
    sprite.setNextFrameTime(data.animationNextFrameTime);

    for(auto it = data.animationTextureRects.begin(); it != data.animationTextureRects.end(); ++it) {

        for(auto vt = it->second.begin(); vt != it->second.end(); ++vt) {

            sprite.insertTextureRect(it->first, *vt);
        }
    }

    for(auto it = data.hitboxes.begin(); it != data.hitboxes.end(); ++it) {

        for(auto vt = it->second.begin(); vt != it->second.end(); ++vt) {

            hitbox.insertHitbox(*vt, it->first);
        }
    }
}

void Enemy::setState(const unsigned &newState) {

    //if new state is equal to current state then no need to switch
    if(newState == currentState) {

        return;
    }

    currentState = newState;
    sprite.setAnimationState(newState);
    hitbox.setActiveHitbox(0, currentState);
}

void Enemy::changeDirectionHorizontally() {

    //change directions
    glm::vec2 currentVelocity = hitboxMovementController.getVelocities();
    currentVelocity.x *= -1;
    hitboxMovementController.setVelocities(currentVelocity);

    determineHorizontalDirection();
}

void Enemy::determineHorizontalDirection() {

    if(hitboxMovementController.getVelocities().x < 0) {

        direction.horizontal = HorizontalDirection::LEFT;

    } else if(hitboxMovementController.getVelocities().x > 0) {

        direction.horizontal = HorizontalDirection::RIGHT;
    }
}

void Enemy::determineAnimationState() {

    glm::vec2 velocities = hitboxMovementController.getVelocities();

    if(direction.horizontal == HorizontalDirection::LEFT) {

        if(velocities.y != 0) {

            setState(STATE_FALLING_LEFT);
        } else {

            setState(STATE_WALKING_LEFT);
        }

    } else {

        if(velocities.y != 0) {

            setState(STATE_FALLING_RIGHT);

        } else {

            setState(STATE_WALKING_RIGHT);
        }
    }
}
