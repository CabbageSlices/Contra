#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "ObjectHitbox.h"
#include "TileCollisionHandling.h"
#include "TileMap.h"
#include "Direction.h"
#include "EntityBase.h"
#include "DataLoaders.h"
#include "AnimatedSprite.h"
#include "PreloadedData.h"

#include <string>
#include <vector>
#include <map>

class Enemy : public EntityBase {

    public:

        Enemy(const glm::vec2 &position, const Direction &initialDirection, const int initialHealth = 1);

        virtual ~Enemy() {

        }

        virtual void updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map);
        void updateRendering();
        void setInitialVelocity(const glm::vec2 &velocity);

        virtual void respondToCollision(const CollisionResponse &response);

        virtual void draw(sf::RenderWindow &window);

        void load(const PreloadedEnemyData &data);

    private:

        virtual CollisionResponse handleTileCollision(TileMap& map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object));

        void changeDirectionHorizontally();
        void determineHorizontalDirection();
        void determineAnimationState();

        unsigned STATE_WALKING_LEFT;
        unsigned STATE_WALKING_RIGHT;
        unsigned STATE_FALLING_LEFT;
        unsigned STATE_FALLING_RIGHT;

        Direction direction;
};

#endif // ENEMY_H_INCLUDED
