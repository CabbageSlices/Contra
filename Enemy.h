#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "PositionController.h"
#include "TileCollisionHandling.h"
#include "TileMap.h"
#include <memory>

class Enemy {

    public:

        Enemy(const glm::vec2 &positionWorldSpace, const glm::vec2 &sizeWorldSpace);

        void setInitialVelocity(const glm::vec2 &velocityObjectSpace);
        void update(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map);

        void draw(sf::RenderWindow &window);

    private:

        void handleTileCollision(TileMap& map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, PositionObject& object));
        void handleTileCollisionHorizontally(TileMap& map);
        void handleTileCollisionVertically(TileMap& map);

        PositionController positionController;
        sf::RectangleShape enemy;
};

#endif // ENEMY_H_INCLUDED
