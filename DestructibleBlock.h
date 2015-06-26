#ifndef DESTRUCTIBLEBLOCK_H_INCLUDED
#define DESTRUCTIBLEBLOCK_H_INCLUDED

#include "DynamicObject.h"
#include "AnimatedSprite.h"
#include "PreloadedData.h"

#include <memory>

class DestructibleBlock : public DynamicObject {

    public:

        DestructibleBlock(const glm::vec2 &position, const PreloadedDestructibleBlockData &data);

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

    private:

        unsigned STATE_SOLID;
        unsigned STATE_DESTROYING;
        unsigned STATE_DESTROYED;
};

#endif // DESTRUCTIBLEBLOCK_H_INCLUDED
