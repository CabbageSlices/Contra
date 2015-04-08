#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "PositionController.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "glm/glm.hpp"
#include "TileMap.h"

//Bullet is fired in the given direction
//the given direction becomes x-axis for the bullet, and a y-axis is calculated automatically
//thus the bullet travels horizontally in object space
class Bullet {

    public:

        Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, const float &bulletVel = 5.f);

        void update(const float &delta, const sf::FloatRect &worldBounds, TileMap& map);
        void killBullet();
        bool checkIsAlive();

        void draw(sf::RenderWindow &window);

    private:

        void handleTileCollision(TileMap& map);

        //velocity in the x direction
        const float xVelocity;
        sf::Time lifeTime;
        float timeElapsed;

        sf::RectangleShape bullet;

        PositionController positionController;
};

#endif // BULLET_H_INCLUDED
