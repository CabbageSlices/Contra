#include "DestructibleBlock.h"
#include "CollisionResolution.h"
#include "EntityAnimationStates.h"

#include <iostream>

using std::cout;
using std::endl;
using std::shared_ptr;

DestructibleBlock::DestructibleBlock(const glm::vec2 &position, const DestructibleBlockType& type, const PreloadedDestructibleBlockData &data) :
    DynamicObject(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 1),
    blockType(type)
    {
        /**
            CONSTRUCTOR CALLS THE VIRTUAL FUNCTION, SCALECOMPONENTS
            IF ANY DERIVATIVE OF THIS CLASS OVERRIDES THE SCALECOMPONENTS FUNCTION THEN THAT DERIVED CLASS WON'T WORK PROPERLY
        **/
        load(data);
        setPosition(position);

        updateRendering();
    }

void DestructibleBlock::updateRendering() {

    if(sprite.animate() && sprite.getAnimationState() == DestructibleBlockEnums::STATE_DESTROYING) {

        setState(DestructibleBlockEnums::STATE_DESTROYED);
    }

    sprite.getSprite().setPosition(hurtbox.getOrigin().x, hurtbox.getOrigin().y);
    matchHitboxPosition();
}

bool DestructibleBlock::checkCanGetHit() {

    return currentState == DestructibleBlockEnums::STATE_SOLID;
}

bool DestructibleBlock::checkIsAlive() {

    return currentState != DestructibleBlockEnums::STATE_DESTROYED;
}

void DestructibleBlock::getHit(int damage) {

    setHealth(health - damage);

    if(health == 0 && currentState != DestructibleBlockEnums::STATE_DESTROYED) {

        setState(DestructibleBlockEnums::STATE_DESTROYING);
    }
}

void DestructibleBlock::draw(sf::RenderWindow &window) {

    sprite.draw(window);
}

void DestructibleBlock::load(const PreloadedDestructibleBlockData &data) {

    loadBase(data);
    scale(data.scale, data.scale);

    setState(DestructibleBlockEnums::STATE_SOLID);
}
