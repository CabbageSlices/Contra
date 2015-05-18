#ifndef DESTRUCTIBLEBLOCK_H_INCLUDED
#define DESTRUCTIBLEBLOCK_H_INCLUDED

#include "DynamicObject.h"
#include "AnimatedSprite.h"

class DestructibleBlock : public DynamicObject {

    public:

        DestructibleBlock(const glm::vec2 &position);

        virtual void updatePhysics(const float &deltaTime, const sf::FloatRect &worldBounds, TileMap &map);
        void updateRendering();

        virtual bool checkCanGetHit();
        virtual void getHit(int damage = 1);
        virtual void draw(sf::RenderWindow &window);

        virtual CollisionResponse handleCollision(EntityBase *collidingEntity);

    private:

        enum AnimationStates {

            SOLID,
            DESTRYOING,
            DESTROYED
        };

        void setup();
        void createTextureRects();

        AnimatedSprite sprite;
};

#endif // DESTRUCTIBLEBLOCK_H_INCLUDED
