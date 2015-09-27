#include "Enemy.h"
#include "GlobalConstants.h"
#include "EntityAnimationStates.h"
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;

Enemy::Enemy(const glm::vec2 &positionWorldSpace, const Direction &initialDirection, const int initialHealth) :
    EntityBase(glm::vec2(0, GRAVITY), glm::vec2(TERMINAL_VELOCITY / 5, 0), glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY), initialHealth),
    direction()
    {
        setPosition(positionWorldSpace);

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

    matchHitboxPosition();
}

void Enemy::updateRendering() {

    determineAnimationState();
    sprite.animate();

    sprite.getSprite().setPosition(hurtbox.getOrigin().x, hurtbox.getOrigin().y);

    matchHitboxPosition();
}

void Enemy::setInitialVelocity(const glm::vec2 &velocity) {

    hitboxMovementController.setVelocities(velocity);
}

void Enemy::respondToCollision(const CollisionResponse &response) {

    if(response.handledHorizontal) {

        changeDirectionHorizontally();
    }

    matchHitboxPosition();
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
    sf::FloatRect box = hurtbox.getActiveHitboxWorldSpace();
    entity.setPosition(box.left, box.top);
    entity.setSize(sf::Vector2f(box.width, box.height));
}

void Enemy::load(const PreloadedEnemyData &data) {

    loadBase(data);
    scale(data.scale, data.scale);

    determineAnimationState();
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
    determineHorizontalDirection();

    if(direction.horizontal == HorizontalDirection::LEFT) {

        if(velocities.y != 0) {

            setState(EnemyEnums::STATE_FALLING_LEFT);

        } else {

            setState(EnemyEnums::STATE_WALKING_LEFT);
        }

    } else {

        if(velocities.y != 0) {

            setState(EnemyEnums::STATE_FALLING_RIGHT);

        } else {

            setState(EnemyEnums::STATE_WALKING_RIGHT);
        }
    }
}
