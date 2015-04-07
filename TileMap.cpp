#include "TileMap.h"
#include "glm/glm.hpp"

using std::vector;
using std::shared_ptr;
using std::make_shared;

TileMap::TileMap() :
    tiles(),
    gridWidth(0),
    gridHeight(0)
    {

    }

TileMap::TileMap(const unsigned &mapWidth, const unsigned &mapHeight):
    tiles(),
    gridWidth(),
    gridHeight()
    {
        createMap(mapWidth, mapHeight);
    }

void TileMap::setTile(const sf::Vector2f& position, const TileType& type) {

    glm::i32vec2 gridPosition = convertToGridPosition(glm::vec2(position.x, position.y));

    if(!checkValidGridPosition(gridPosition)) {

        return;
    }

    int index = gridPosition.y * gridWidth + gridPosition.x;

    if(tiles.size() <= index) {

        return;
    }

    tiles[index]->setType(type);
}

vector<shared_ptr<Tile> > TileMap::getTilesInRegion(const glm::vec2 &upperLeft, const glm::vec2& bottomRight) {

    glm::i32vec2 gridTopLeft = convertToGridPosition(upperLeft);
    glm::i32vec2 gridBottomRight = convertToGridPosition(bottomRight);

    if(gridTopLeft.x > gridBottomRight.x || gridTopLeft.y > gridBottomRight.y) {

        return vector<shared_ptr<Tile> > ();
    }

    gridTopLeft.x = glm::clamp(gridTopLeft.x, 0, gridWidth - 1);
    gridTopLeft.y = glm::clamp(gridTopLeft.y, 0, gridHeight - 1);
    gridBottomRight.x = glm::clamp(gridBottomRight.x, 0, gridWidth - 1);
    gridBottomRight.y = glm::clamp(gridBottomRight.y, 0, gridHeight - 1);

    vector<shared_ptr<Tile> > tilesInRegion;

    for(unsigned y = gridTopLeft.y; y <= gridBottomRight.y; ++y) {

        for(unsigned x = gridTopLeft.x; x <= gridBottomRight.x; ++x) {

            int index = y * gridWidth + x;
            tilesInRegion.push_back(tiles[index]);
        }
    }

    return tilesInRegion;
}

void TileMap::draw(sf::RenderWindow& window, const glm::vec2 &upperLeft, const glm::vec2 &lowerRight) {

    vector<shared_ptr<Tile> > tilesInRegion = getTilesInRegion(upperLeft, lowerRight);

    for(unsigned i = 0; i < tilesInRegion.size(); ++i) {

        tilesInRegion[i]->draw(window);
    }
}

void TileMap::drawDebug(sf::RenderWindow& window, const glm::vec2 &upperLeft, const glm::vec2 &lowerRight) {

    vector<shared_ptr<Tile> > tilesInRegion = getTilesInRegion(upperLeft, lowerRight);

    for(unsigned i = 0; i < tilesInRegion.size(); ++i) {

        tilesInRegion[i]->drawDebug(window);
    }
}

void TileMap::createMap(const unsigned &mapWidth, const unsigned &mapHeight) {

    if(mapWidth < 0 || mapHeight < 0) {

        return;
    }

    gridWidth = glm::ceil(mapWidth / static_cast<float>(TILE_SIZE));
    gridHeight = glm::ceil(mapHeight / static_cast<float>(TILE_SIZE));

    tiles.clear();

    for(unsigned y = 0; y < gridHeight; ++y) {

        for(unsigned x = 0; x < gridWidth; ++x) {

            shared_ptr<Tile> tile = make_shared<Tile>(glm::vec2(x * TILE_SIZE, y * TILE_SIZE), TileType::EMPTY);
            tiles.push_back(tile);
        }
    }
}

glm::i32vec2 TileMap::convertToGridPosition(const glm::vec2 &worldPosition) const {

    return glm::i32vec2(worldPosition.x / TILE_SIZE, worldPosition.y / TILE_SIZE);
}

bool TileMap::checkValidGridPosition(const glm::i32vec2 &gridPosition) const {

    if(gridPosition.x < 0 || gridPosition.x >= gridWidth || gridPosition.y < 0 || gridPosition.y >= gridHeight) {

        return false;
    }

    return true;
}
