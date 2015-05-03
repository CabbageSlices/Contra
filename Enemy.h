#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "HitboxMovementController.h"
#include "ObjectHitbox.h"
#include "TileCollisionHandling.h"
#include "TileMap.h"
#include "Direction.h"
#include <memory>

class Enemy {

    public:

        Enemy(const glm::vec2 &position, const Direction &initialDirection, const int initialHealth = 1);

        bool checkIsAlive() const;
        void update(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map);
        void draw(sf::RenderWindow &window);
        bool getHit(int damage = 1); //returns true if enemy is still alive, false if dead

        void setInitialVelocity(const glm::vec2 &velocity);

        const ObjectHitbox &getHitbox() const;

    private:

        void handleTileCollision(TileMap& map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object));
        void handleTileCollisionHorizontally(TileMap& map);
        void handleTileCollisionVertically(TileMap& map);

        ObjectHitbox hitbox;
        HitboxMovementController hitboxMovementController;
        sf::RectangleShape enemy;
        int health;
};

#endif // ENEMY_H_INCLUDED
