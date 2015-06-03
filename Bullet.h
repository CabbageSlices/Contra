#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "HitboxMovementController.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "glm/glm.hpp"
#include "TileMap.h"
#include "ObjectHitbox.h"
#include "EntityBase.h"
#include <memory>
#include <memory>

class Enemy;

class Bullet : public EntityBase {

    public:

        Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, const float &bulletVel = 11.f);

        virtual void updatePhysics(const float &delta, const sf::FloatRect &worldBounds, TileMap& map);
        void updateRendering();
        virtual bool checkIsAlive();

        //take a raw pointer instead of shared pointer because shared pointers don't support polymorphism
        void handleEntityCollision(std::shared_ptr<EntityBase> collidingEntity);
        void killBullet();

    private:

        void handleTileCollision(TileMap& map);
        virtual CollisionResponse handleTileCollision(TileMap &map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object))  {

            //empty since bullets don't need to handle collision
            return CollisionResponse();
        }

        sf::Time lifeTime;
        float timeElapsed;

        glm::vec2 direction;
};

#endif // BULLET_H_INCLUDED
