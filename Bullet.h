#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "HitboxMovementController.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "glm/glm.hpp"
#include "TileMap.h"
#include "ObjectHitbox.h"
#include "EntityBase.h"
#include "AnimatedSprite.h"
#include "PreloadedData.h"
#include <memory>

/**
    CONSTRUCTOR CALLS THE VIRTUAL FUNCTION, SCALECOMPONENTS
    IF ANY DERIVATIVE OF THIS CLASS OVERRIDES THE SCALECOMPONENTS FUNCTION THEN THAT DERIVED CLASS WON'T WORK PROPERLY
**/

class Bullet : public EntityBase {

    public:

        Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, const float &bulletVel = 6.f);
        Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, const PreloadedBulletData &data);

        virtual ~Bullet() {

        }

        virtual void updatePhysics(const float &delta, const sf::FloatRect &worldBounds, TileMap& map);
        void updateRendering();
        virtual bool checkIsAlive();
        virtual void draw(sf::RenderWindow &window);

        void killBullet();

    private:

        void handleTileCollision(TileMap& map);
        virtual CollisionResponse handleTileCollision(TileMap &map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object))  {

            //empty since bullets don't need to handle collision
            return CollisionResponse();
        }

        void load(const PreloadedBulletData &data);

        void determineState();
        void setState(const unsigned &newState);

        sf::Time lifeTime;
        float timeElapsed;

        glm::vec2 direction;
};

#endif // BULLET_H_INCLUDED
