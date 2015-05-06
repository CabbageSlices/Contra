#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "HitboxMovementController.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "glm/glm.hpp"
#include "TileMap.h"
#include "ObjectHitbox.h"
#include <memory>

class Enemy;

class Bullet {

    public:

        Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, const float &bulletVel = 11.f);

        void update(const float &delta, const sf::FloatRect &worldBounds, TileMap& map);
        void draw(sf::RenderWindow &window);

        void handleEnemyCollision(std::shared_ptr<Enemy> &enemy);
        void killBullet();
        bool checkIsAlive();

        const ObjectHitbox &getHitbox() const;

    private:

        void handleTileCollision(TileMap& map);

        //magnitude of velocity
        //this is multiplied to the bulletdirection to get the velcoity of bullect
        const float velocity;
        sf::Time lifeTime;
        float timeElapsed;

        sf::RectangleShape bullet;
        glm::vec2 direction;

        ObjectHitbox hitbox;
        HitboxMovementController hitboxMovementController;
};

#endif // BULLET_H_INCLUDED
