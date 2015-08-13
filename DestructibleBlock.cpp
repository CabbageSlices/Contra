#include "DestructibleBlock.h"
#include "CollisionResolution.h"

#include <iostream>

using std::cout;
using std::endl;
using std::shared_ptr;

DestructibleBlock::DestructibleBlock(const glm::vec2 &position, const PreloadedDestructibleBlockData &data) :
    DynamicObject(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 1),
    STATE_SOLID(0),
    STATE_DESTROYING(0),
    STATE_DESTROYED(0)
    {
        setPosition(position);
        load(data);

        updateRendering();
    }

void DestructibleBlock::updateRendering() {

    if(sprite.animate() && sprite.getAnimationState() == STATE_DESTROYING) {

        setState(STATE_DESTROYED);
    }

    sprite.getSprite().setPosition(hitbox.getOrigin().x, hitbox.getOrigin().y);
}

bool DestructibleBlock::checkCanGetHit() {

    return currentState == STATE_SOLID;
}

bool DestructibleBlock::checkIsAlive() {

    return currentState != STATE_DESTROYED;
}

void DestructibleBlock::getHit(int damage) {

    setHealth(health - damage);

    if(health == 0) {

        setState(STATE_DESTROYING);
    }
}

void DestructibleBlock::draw(sf::RenderWindow &window) {

    sprite.draw(window);
}

void DestructibleBlock::load(const PreloadedDestructibleBlockData &data) {

    loadBase(data);
    scale(data.scale, data.scale);

    STATE_SOLID = data.STATE_SOLID;
    STATE_DESTROYING = data.STATE_DESTROYING;
    STATE_DESTROYED = data.STATE_DESTROYED;

    setState(STATE_SOLID);
}
