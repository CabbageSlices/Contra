#ifndef DESTRUCTIBLEBLOCK_H_INCLUDED
#define DESTRUCTIBLEBLOCK_H_INCLUDED

#include "DynamicObject.h"
#include "AnimatedSprite.h"
#include "PreloadedData.h"
#include "ObjectTypes.h"

#include <memory>

/**
    CONSTRUCTOR CALLS THE VIRTUAL FUNCTION, SCALECOMPONENTS
    IF ANY DERIVATIVE OF THIS CLASS OVERRIDES THE SCALECOMPONENTS FUNCTION THEN THAT DERIVED CLASS WON'T WORK PROPERLY
**/

class DestructibleBlock : public DynamicObject {

    public:

        DestructibleBlock(const glm::vec2 &position, const DestructibleBlockType& blockType, const PreloadedDestructibleBlockData &data);

        virtual ~DestructibleBlock() {

        }

        virtual void updatePhysics(const float &deltaTime, const sf::FloatRect &worldBounds, TileMap &map) {

            //empty, only here so vtable is created
        }

        void updateRendering();

        virtual bool checkCanGetHit();
        virtual bool checkIsAlive();
        virtual void getHit(int damage = 1);
        virtual void draw(sf::RenderWindow &window);

        void load(const PreloadedDestructibleBlockData &data);

        const DestructibleBlockType& getType() {

            return blockType;
        }

    private:

        DestructibleBlockType blockType;
};

#endif // DESTRUCTIBLEBLOCK_H_INCLUDED
