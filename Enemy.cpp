#include "Enemy.h"
#include "GlobalConstants.h"
#include <vector>

using std::vector;
using std::shared_ptr;

Enemy::Enemy(const glm::vec2 &positionWorldSpace, const glm::vec2 &sizeWorldSpace) :
    hitbox(),
    hitboxMovementController(glm::vec2(0, GRAVITY), glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY), &hitbox),
    enemy(sf::Vector2f(64, 64))
    {
        hitbox.insertHitbox(sf::FloatRect(0, 0, enemy.getSize().x, enemy.getSize().y));
        hitbox.setActiveHitbox(0);
    }

void Enemy::setInitialVelocity(const glm::vec2 &velocity) {

    hitboxMovementController.setVelocities(velocity);
}

void Enemy::update(const float &deltaTime, const sf::FloatRect &worldBounds, TileMap &map) {

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

    enemy.setPosition(hitbox.getOrigin().x, hitbox.getOrigin().y);
}

void Enemy::draw(sf::RenderWindow &window) {

    window.draw(enemy);
}

void Enemy::handleTileCollision(TileMap &map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object)) {

     sf::FloatRect bounding = hitbox.getActiveHitboxWorldSpace();

    //calculate region encompassed by object
    //extedn the region slightly because slope tiles need extra information about object previous position if he leaves a tile
    glm::vec2 regionTopLeft = glm::vec2(bounding.left, bounding.top) - glm::vec2(TILE_SIZE, TILE_SIZE);
    glm::vec2 regionBottomRight = glm::vec2(bounding.left + bounding.width, bounding.top + bounding.height) + glm::vec2(TILE_SIZE, TILE_SIZE);

    vector<shared_ptr<Tile> > tiles = map.getTilesInRegion(regionTopLeft, regionBottomRight);

    for(unsigned i = 0; i < tiles.size(); ++i) {

        CollisionResponse response = collisionFunction(tiles[i], hitboxMovementController);

        if(response.handledHorizontal) {

            hitboxMovementController.setVelocities(-hitboxMovementController.getVelocities().x, hitboxMovementController.getVelocities().y);
        }
    }
}

void Enemy::handleTileCollisionHorizontally(TileMap& map) {

    handleTileCollision(map, &handleCollisionHorizontal);
}

void Enemy::handleTileCollisionVertically(TileMap& map) {

    handleTileCollision(map, &handleCollisionVertical);
}
