#ifndef DESTRUCTIBLEBLOCK_H_INCLUDED
#define DESTRUCTIBLEBLOCK_H_INCLUDED

#include "EntityBase.h"
#include "AnimatedSprite.h"

class DestructibleBlock : public EntityBase {

    public:

        DestructibleBlock(const glm::vec2 &position);

        virtual void update(const float &deltaTime, const sf::FloatRect &worldBounds, TileMap &map);

        virtual bool checkCanGetHit();
        virtual void getHit(int damage = 1);
        virtual void draw(sf::RenderWindow &window);

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
