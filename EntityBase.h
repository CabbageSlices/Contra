#ifndef ENTITYBASE_H_INCLUDED
#define ENTITYBASE_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "TileMap.h"
#include "Tile.h"
#include "TileCollisionHandling.h"
#include "ObjectHitbox.h"
#include "CollisionResponse.h"
#include "HitboxMovementController.h"

#include "glm/glm.hpp"
#include <memory>
#include <vector>

class EntityBase {

    public:

        EntityBase(const glm::vec2 &gravity, const glm::vec2 &movementVelocity, const glm::vec2 &terminalVelocity, const unsigned &initialHealth);

        virtual void updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map) = 0;
        virtual void draw(sf::RenderWindow &window);
        virtual bool checkIsAlive();
        virtual bool checkCanGetHit();
        virtual void getHit(int damage = 1);

        //first handle collision is the function called by objects that affects other objects when they collide
        //i.e a block will call this function because it will push all colliding objects away
        virtual CollisionResponse handleCollision(std::shared_ptr<EntityBase> collidingEntity) {

            //empty
            return CollisionResponse();
        };

        //this one should be called
        virtual void respondToCollision(const CollisionResponse &collisionResponse) {

            //empty
        }

        ObjectHitbox& getHitbox();
        HitboxMovementController& getMovementController();
        const glm::vec2 getPosition() const;

    protected:

        //get all tiles that are covered by the surface area of this object
        //add padding to the area of this object incase you need extra tiles that are around the object but aren't touching the object
        //you might need to use padding for things like handling collision with slopes, since slopes require surounding tiles, not just colliding tiles
        std::vector<std::shared_ptr<Tile> > getSurroundingTiles(const TileMap &map, const glm::vec2 &areaPadding);

        //leave this virtual since different objects have different collision handling
        virtual CollisionResponse handleTileCollision(TileMap &map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object));
        CollisionResponse handleTileCollisionHorizontally(TileMap& map);
        CollisionResponse handleTileCollisionVertically(TileMap& map);

        ObjectHitbox hitbox;
        HitboxMovementController hitboxMovementController;
        const glm::vec2 MOVEMENT_VELOCITY; //measured in meters per second
        unsigned health;

        sf::RectangleShape entity;
};

#endif // ENTITYBASE_H_INCLUDED
