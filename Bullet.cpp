#include "Bullet.h"
#include "TileCollisionHandling.h"
#include "Enemy.h"
#include "EntityAnimationStates.h"

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
        hurtbox.setOrigin(positionWorldSpace - glm::vec2(10, 10));
        hurtbox.insertHitbox(sf::FloatRect(0, 0, entity.getSize().x, entity.getSize().y));
        hurtbox.setActiveHitbox(0);
        hitboxMovementController.setVelocities(MOVEMENT_VELOCITY);
    }

Bullet::Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, const PreloadedBulletData &data) :
    EntityBase(glm::vec2(0, 0), directionWorldSpace, glm::vec2(20.f, 20.f), 1),
    lifeTime(sf::seconds(1.5)),
    timeElapsed(0),
    direction(directionWorldSpace)
    {
        /**
            CONSTRUCTOR CALLS THE VIRTUAL FUNCTION, SCALECOMPONENTS
            IF ANY DERIVATIVE OF THIS CLASS OVERRIDES THE SCALECOMPONENTS FUNCTION THEN THAT DERIVED CLASS WON'T WORK PROPERLY
        **/

        load(data);

        setPosition(positionWorldSpace);
        hitboxMovementController.setVelocities(MOVEMENT_VELOCITY);
    }

void Bullet::updatePhysics(const float &delta, const sf::FloatRect &worldBounds, TileMap& map) {

    timeElapsed += delta;

    hitboxMovementController.move(delta);
    handleTileCollision(map);

    matchHitboxPosition();
}

void Bullet::updateRendering() {

    sprite.animate();

    matchHitboxPosition();
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

void Bullet::load(const PreloadedBulletData &data) {

    loadBase(data);
    scale(data.scale, data.scale);

    MOVEMENT_VELOCITY = direction * data.velocity;
    lifeTime = data.lifetime;

    determineState();
}

void Bullet::determineState() {

    //determine state based on direction bullet is travelling
    if(MOVEMENT_VELOCITY.x == 0) {

        if(MOVEMENT_VELOCITY.y < 0) {

            setState(BulletEnums::STATE_UP);

        } else if(MOVEMENT_VELOCITY.y > 0) {

            setState(BulletEnums::STATE_DOWN);
        }

    } else if(MOVEMENT_VELOCITY.y == 0) {

        if(MOVEMENT_VELOCITY.x < 0) {

            setState(BulletEnums::STATE_LEFT);

        } else if(MOVEMENT_VELOCITY.x > 0) {

            setState(BulletEnums::STATE_RIGHT);
        }

    } else {

        if(MOVEMENT_VELOCITY.y < 0 && MOVEMENT_VELOCITY.x < 0) {

            setState(BulletEnums::STATE_UP_LEFT);

        } else if(MOVEMENT_VELOCITY.y < 0 && MOVEMENT_VELOCITY.x > 0) {

            setState(BulletEnums::STATE_UP_RIGHT);

        } else if(MOVEMENT_VELOCITY.y > 0 && MOVEMENT_VELOCITY.x < 0) {

            setState(BulletEnums::STATE_DOWN_LEFT);

        } else if(MOVEMENT_VELOCITY.y > 0 && MOVEMENT_VELOCITY.x > 0) {

            setState(BulletEnums::STATE_DOWN_RIGHT);
        }
    }
}

void Bullet::setState(const unsigned &state) {

    if(currentState == state) {

        return;
    }

    currentState = state;
    sprite.setAnimationState(state);
    hurtbox.setActiveHitbox(0, state);
}
