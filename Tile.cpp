#include "Tile.h"

const int TILE_SIZE = 64;

glm::vec2 getSlopeForTileType(const TileType& type) {

    switch(type) {

        case TileType::UPWARD_RIGHT_1_1:
        case TileType::DOWNWARD_LEFT_1_1: {

            return glm::vec2(1, -1);
        }

        case TileType::UPWARD_LEFT_1_1:
        case TileType::DOWNWARD_RIGHT_1_1: {

            return glm::vec2(1, 1);
        }

        default:
            return glm::vec2(1, 0);
    }

    return glm::vec2(1, 0);
}

Tile::Tile(const glm::vec2& worldPosition,  const TileType& tileType) :
    type(tileType),
    tile(sf::Vector2f(TILE_SIZE, TILE_SIZE))
    {
        setupEmptyTileColor();
        tile.setPosition(sf::Vector2f(worldPosition.x, worldPosition.y));
    }

Tile::Tile() :
    type(TileType::EMPTY),
    tile(sf::Vector2f(TILE_SIZE, TILE_SIZE))
    {
        setupEmptyTileColor();
    }

void Tile::setType(const TileType& tileType) {

    type = tileType;
}

sf::FloatRect Tile::getBoundingBox() const {

    return tile.getGlobalBounds();
}

TileType Tile::getType() const {

    return type;
}

void Tile::draw(sf::RenderWindow& window) {

    if(type != TileType::EMPTY) {

        window.draw(tile);
    }
}

void Tile::drawDebug(sf::RenderWindow& window) {

    window.draw(tile);
}

void Tile::setupEmptyTileColor() {

    tile.setFillColor(sf::Color::Black);
    tile.setOutlineColor(sf::Color::Red);
    tile.setOutlineThickness(-2);
}
