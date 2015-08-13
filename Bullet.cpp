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
    direction(directionWorldSpace),
    STATE_RIGHT(0),
    STATE_UP_RIGHT(0),
    STATE_UP(0),
    STATE_UP_LEFT(0),
    STATE_LEFT(0),
    STATE_DOWN_LEFT(0),
    STATE_DOWN(0),
    STATE_DOWN_RIGHT(0)
    {
        entity.setSize(sf::Vector2f(20, 20));
        hitbox.setOrigin(positionWorldSpace - glm::vec2(10, 10));
        hitbox.insertHitbox(sf::FloatRect(0, 0, entity.getSize().x, entity.getSize().y));
        hitbox.setActiveHitbox(0);
        hitboxMovementController.setVelocities(MOVEMENT_VELOCITY);
    }

Bullet::Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, PreloadedBulletData &data) :
    EntityBase(glm::vec2(0, 0), directionWorldSpace, glm::vec2(20.f, 20.f), 1),
    lifeTime(sf::seconds(1.5)),
    timeElapsed(0),
    direction(directionWorldSpace),
    STATE_RIGHT(0),
    STATE_UP_RIGHT(0),
    STATE_UP(0),
    STATE_UP_LEFT(0),
    STATE_LEFT(0),
    STATE_DOWN_LEFT(0),
    STATE_DOWN(0),
    STATE_DOWN_RIGHT(0)
    {
        load(data);

        //center the bullet at the given position
        sf::FloatRect boundingBox = hitbox.getActiveHitboxWorldSpace();

        hitbox.setOrigin(positionWorldSpace - glm::vec2(boundingBox.width / 2, boundingBox.height / 2));
        hitboxMovementController.setVelocities(MOVEMENT_VELOCITY);
    }

void Bullet::updatePhysics(const float &delta, const sf::FloatRect &worldBounds, TileMap& map) {

    timeElapsed += delta;

    hitboxMovementController.move(delta);
    handleTileCollision(map);
}

void Bullet::updateRendering() {

    sprite.animate();

    glm::vec2 position = hitbox.getOrigin();
    sprite.getSprite().setPosition(position.x, position.y);
}

bool Bullet::checkIsAlive() {

    return timeElapsed < lifeTime.asSeconds();
}

void Bullet::draw(sf::RenderWindow &window) {

    sprite.draw(window);
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

void Bullet::load(PreloadedBulletData &data) {

    loadBase(data);
    scale(data.scale, data.scale);

    STATE_RIGHT = data.STATE_RIGHT;
    STATE_UP_RIGHT = data.STATE_UP_RIGHT;
    STATE_UP = data.STATE_UP;
    STATE_UP_LEFT = data.STATE_UP_LEFT;
    STATE_LEFT = data.STATE_LEFT;
    STATE_DOWN_LEFT = data.STATE_DOWN_LEFT;
    STATE_DOWN = data.STATE_DOWN;
    STATE_DOWN_RIGHT = data.STATE_DOWN_RIGHT;

    MOVEMENT_VELOCITY = direction * data.velocity;
    lifeTime = data.lifetime;

    determineState();
}

void Bullet::determineState() {

    //determine state based on direction bullet is travelling
    if(MOVEMENT_VELOCITY.x == 0) {

        if(MOVEMENT_VELOCITY.y < 0) {

            setState(STATE_UP);

        } else if(MOVEMENT_VELOCITY.y > 0) {

            setState(STATE_DOWN);
        }

    } else if(MOVEMENT_VELOCITY.y == 0) {

        if(MOVEMENT_VELOCITY.x < 0) {

            setState(STATE_LEFT);

        } else if(MOVEMENT_VELOCITY.x > 0) {

            setState(STATE_RIGHT);
        }

    } else {

        if(MOVEMENT_VELOCITY.y < 0 && MOVEMENT_VELOCITY.x < 0) {

            setState(STATE_UP_LEFT);

        } else if(MOVEMENT_VELOCITY.y < 0 && MOVEMENT_VELOCITY.x > 0) {

            setState(STATE_UP_RIGHT);

        } else if(MOVEMENT_VELOCITY.y > 0 && MOVEMENT_VELOCITY.x < 0) {

            setState(STATE_DOWN_LEFT);

        } else if(MOVEMENT_VELOCITY.y > 0 && MOVEMENT_VELOCITY.x > 0) {

            setState(STATE_DOWN_RIGHT);
        }
    }
}

void Bullet::setState(const unsigned &state) {

    if(currentState == state) {

        return;
    }

    currentState = state;
    sprite.setAnimationState(state);
    hitbox.setActiveHitbox(0, state);
}
