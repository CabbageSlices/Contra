#ifndef PIRANHAPLANT_H_INCLUDED
#define PIRANHAPLANT_H_INCLUDED

#include "AnimatedSprite.h"
#include "TurretEnemy.h"

class PiranhaPlant : public TurretEnemy {

    public:

        PiranhaPlant(const glm::vec2 &topLeftPosition);

        virtual void updatePhysics(const float &deltaTime, const sf::FloatRect &worldBounds, TileMap &map);
        virtual void draw(sf::RenderWindow &window);

    protected:

        //drawing states of the plant
        const unsigned STATE_HIDING;
        const unsigned STATE_EXPOSING;
        const unsigned STATE_SHOOTING;

        //keep track of texture rect numbers for when plant is shooting
        //when its shooting it needs to draw the texture that matches its current rotation


        //frame number

        AnimatedSprite sprite;
};

#endif // PIRANHAPLANT_H_INCLUDED
