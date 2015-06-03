#include "Bullet.h"
#include "TileCollisionHandling.h"
#include "Enemy.h"

#include <vector>
#include <iostream>

using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;

Bullet::Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, const float &bulletVel) :
    EntityBase(glm::vec2(0, 0), directionWorldSpace * bulletVel, glm::vec2(bulletVel, bulletVel), 1),
    lifeTime(sf::seconds(1.5)),
    timeElapsed(0),
    direction(directionWorldSpace)
    {
        entity.setSize(sf::Vector2f(20, 20));
        hitbox.setOrigin(positionWorldSpace - glm::vec2(10, 10));
        hitbox.insertHitbox(sf::FloatRect(0, 0, entity.getSize().x, entity.getSize().y));
        hitbox.setActiveHitbox(0);
        hitboxMovementController.setVelocities(MOVEMENT_VELOCITY);
    }

void Bullet::updatePhysics(const float &delta, const sf::FloatRect &worldBounds, TileMap& map) {

    timeElapsed += delta;

    hitboxMovementController.move(delta);
    handleTileCollision(map);
}

void Bullet::updateRendering() {

    glm::vec2 position = hitbox.getOrigin();
    entity.setPosition(position.x, position.y);
}

bool Bullet::checkIsAlive() {

    return timeElapsed < lifeTime.asSeconds();
}

void Bullet::handleEntityCollision(shared_ptr<EntityBase> collidingEntity) {

    collidingEntity->getHit(1);
    killBullet();
}

void Bullet::killBullet() {

    lifeTime = sf::seconds(0);
}

void Bullet::handleTileCollision(TileMap& map) {

    vector<shared_ptr<Tile> > tiles = getSurroundingTiles(map, glm::vec2(TILE_SIZE, TILE_SIZE));

    for(unsigned i = 0; i < tiles.size(); ++i) {

        if(checkSolidTileIntersection(tiles[i], hitboxMovementController)) {

            killBullet();
        }
    }
}
