#ifndef ENTITYBASE_H_INCLUDED
#define ENTITYBASE_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "TileMap.h"
#include "Tile.h"
#include "TileCollisionHandling.h"
#include "ObjectHitbox.h"
#include "HitboxMovementController.h"

#include "glm/glm.hpp"
#include <memory>
#include <vector>

class EntityBase {

    public:

        EntityBase(const glm::vec2 &gravity, const glm::vec2 &movementVelocity, const glm::vec2 &terminalVelocity, const unsigned &initialHealth);

        virtual void update(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map) = 0;
        void draw(sf::RenderWindow &window);
        virtual bool checkIsAlive();
        void getHit(int damage = 1);

        const ObjectHitbox& getHitbox() const;
        const glm::vec2 getPosition() const;

    protected:

        //get all tiles that are covered by the surface area of this object
        //add padding to the area of this object incase you need extra tiles that are around the object but aren't touching the object
        //you might need to use padding for things like handling collision with slopes, since slopes require surounding tiles, not just colliding tiles
        std::vector<std::shared_ptr<Tile> > getSurroundingTiles(const TileMap &map, const glm::vec2 &areaPadding);

        //leave this virtual since different objects have different collision handling
        virtual CollisionResponse handleTileCollision(TileMap &map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object)) = 0;
        CollisionResponse handleTileCollisionHorizontally(TileMap& map);
        CollisionResponse handleTileCollisionVertically(TileMap& map);

        ObjectHitbox hitbox;
        HitboxMovementController hitboxMovementController;
        const glm::vec2 MOVEMENT_VELOCITY;
        unsigned health;

        sf::RectangleShape entity;
};

#endif // ENTITYBASE_H_INCLUDED
