#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "Tile.h"
#include "glm/gtc/type_precision.hpp"

#include <memory>
#include <vector>

class TileMap {

    public:

        TileMap();

        //map width and height are in world coordinates, not grid coordinates, measured in pixels
        TileMap(const unsigned &width, const unsigned &height);

        void resize(const unsigned &width, const unsigned &height);
        void setTile(const sf::Vector2f& position, const TileType& type);

        //return all the tiles enclosed in the given region
        //if any part of the region is out of bounds it will only return the tiles that are in bounds
        //region coordinates are in world position in pixels
        std::vector<std::shared_ptr<Tile> > getTilesInRegion(const glm::vec2 &upperLeft, const glm::vec2 &lowerRight) const;

        void draw(sf::RenderWindow& window, const glm::vec2 &upperLeft, const glm::vec2 &lowerRight);
        void drawDebug(sf::RenderWindow& window, const glm::vec2 &upperLeft, const glm::vec2 &lowerRight);

    private:

        //turns the given world coordinate into grid coordinate
        //doesn't check if its a valid coordinate
        glm::i32vec2 convertToGridPosition(const glm::vec2 &worldPosition) const;
        bool checkValidGridPosition(const glm::i32vec2 &gridPosition) const;

        std::vector<std::shared_ptr<Tile> > tiles;
        int gridWidth;
        int gridHeight;
};

#endif // TILEMAP_H_INCLUDED
