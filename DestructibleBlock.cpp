#include "DestructibleBlock.h"

#include <iostream>

using std::cout;
using std::endl;
using std::shared_ptr;

DestructibleBlock::DestructibleBlock(const glm::vec2 &position) :
    DynamicObject(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 1),
    sprite(sf::milliseconds(30))
    {
        setup();
        hitbox.setOrigin(position);
        hitbox.insertHitbox(sf::FloatRect(0, 0, 64, 64));
        hitbox.setActiveHitbox(0);
        sprite.getSprite().setPosition(sf::Vector2f(position.x, position.y));
        updateRendering();
    }

void DestructibleBlock::updatePhysics(const float &deltaTime, const sf::FloatRect &worldBounds, TileMap &map) {

    if(sprite.animate() && sprite.getAnimationState() == DESTRYOING) {

        sprite.setAnimationState(DESTROYED);
        health = 0;
    }
}

void DestructibleBlock::updateRendering() {

    sprite.getSprite().setPosition(hitbox.getOrigin().x, hitbox.getOrigin().y);
}

bool DestructibleBlock::checkCanGetHit() {

    return sprite.getAnimationState() == SOLID;
}

void DestructibleBlock::getHit(int damage) {

    sprite.setAnimationState(DESTRYOING);
}

void DestructibleBlock::draw(sf::RenderWindow &window) {

    sprite.draw(window);
}

CollisionResponse DestructibleBlock::handleCollision(shared_ptr<EntityBase> collidingEntity) {

    CollisionResponse response;

    sf::FloatRect collidingRect = collidingEntity->getHitbox().getActiveHitboxWorldSpace();
    sf::FloatRect rect = hitbox.getActiveHitboxWorldSpace();

    if(!rect.intersects(collidingRect)) {

        return response;
    }

    //intersection occured so first resolve intersection before other effects
    glm::vec2 minimumTranslation = calculateCollisionResolutionTranslation(collidingRect, rect);

    collidingEntity->getHitbox().move(minimumTranslation);

    //check if object is now standing on top of the block
    if(minimumTranslation.y < 0) {

        response.pushedToTop = true;

        //if object is falling ontop of the block, negate velocity
        if(collidingEntity->getMovementController().getVelocities().y > 0) {

          collidingEntity->getMovementController().setVelocities(collidingEntity->getMovementController().getVelocities().x, 0);
        }
    }

    collidingRect = collidingEntity->getHitbox().getActiveHitboxWorldSpace();

    if(minimumTranslation.x != 0) {

        response.handledHorizontal = true;
    }

    //if this was hit from the bottom then destory itself, only do it if object is jumping
    if(minimumTranslation.y > 0 && checkCanGetHit() && collidingEntity->getMovementController().getVelocities().y < 0) {

        getHit(1);
        collidingEntity->getMovementController().setVelocities(collidingEntity->getMovementController().getVelocities().x, 0);
    }

    return response;
}

void DestructibleBlock::setup() {

    //load the appropriate texture
    sprite.loadTexture("BrickBreak.png");

    createTextureRects();
}

void DestructibleBlock::createTextureRects() {

    sprite.insertTextureRect(SOLID, sf::IntRect(0, 0, 64, 64));
    sprite.insertTextureRect(DESTROYED, sf::IntRect(0, 0, 0, 0));

    for(int y = 0; y < 3; ++y) {

        for(int x = 0; x < 3; ++x) {

            sprite.insertTextureRect(DESTRYOING, sf::IntRect(x * 64, y * 64, 64, 64));
        }
    }
}
