#include "Enemy.h"
#include "GlobalConstants.h"
#include <vector>

using std::vector;
using std::shared_ptr;

Enemy::Enemy(const glm::vec2 &positionWorldSpace, const Direction &initialDirection, const int initialHealth) :
    EntityBase(glm::vec2(0, GRAVITY), glm::vec2(TERMINAL_VELOCITY / 5, 0), glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY), initialHealth)
    {
        entity.setSize(sf::Vector2f(64, 64));
        hitbox.setOrigin(positionWorldSpace);
        hitbox.insertHitbox(sf::FloatRect(0, 0, entity.getSize().x, entity.getSize().y));
        hitbox.setActiveHitbox(0);

        if(initialDirection.horizontal == HorizontalDirection::RIGHT) {

            setInitialVelocity(glm::vec2(MOVEMENT_VELOCITY.x, 0));

        } else {

            setInitialVelocity(glm::vec2(-MOVEMENT_VELOCITY.x, 0));
        }
    }

void Enemy::updatePhysics(const float &deltaTime, const sf::FloatRect &worldBounds, TileMap &map) {

    hitboxMovementController.updateVelocities(deltaTime);

    if(hitboxMovementController.moveAlongXAxis(deltaTime, worldBounds)) {

        //change directions
        glm::vec2 currentVelocity = hitboxMovementController.getVelocities();
        currentVelocity.x *= -1;
        hitboxMovementController.setVelocities(currentVelocity);
    }

    handleTileCollisionHorizontally(map);

    if(hitboxMovementController.moveAlongYAxis(deltaTime, worldBounds)) {

        hitboxMovementController.setVelocities(hitboxMovementController.getVelocities().x, 0);
    }

    handleTileCollisionVertically(map);
}

void Enemy::updateRendering() {

    entity.setPosition(hitbox.getOrigin().x, hitbox.getOrigin().y);
}

void Enemy::setInitialVelocity(const glm::vec2 &velocity) {

    hitboxMovementController.setVelocities(velocity);
}

CollisionResponse Enemy::handleCollision(shared_ptr<EntityBase> collidingEntity) {

    collidingEntity->getHit(1);

    return CollisionResponse();
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
