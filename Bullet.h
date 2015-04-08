#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "PositionController.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "glm/glm.hpp"
#include "TileMap.h"

class Bullet {

    public:

        Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, const float &bulletVel = 5.f);

        void update(const float &delta, const sf::FloatRect &worldBounds, TileMap& map);
        void killBullet();
        bool checkIsAlive();

        void draw(sf::RenderWindow &window);

    private:

        void handleTileCollision(TileMap& map);

        //magnitude of velocity
        //this is multiplied to the bulletdirection to get the velcoity of bullect
        const float velocity;
        sf::Time lifeTime;
        float timeElapsed;

        sf::RectangleShape bullet;
        glm::vec2 direction;

        PositionController positionController;
};

#endif // BULLET_H_INCLUDED
