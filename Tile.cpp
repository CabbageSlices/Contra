#include "Tile.h"

const int TILE_SIZE = 64;

using std::string;

std::map<std::string, sf::Texture> Tile::textures;

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

glm::vec2 getInterceptsForTileType(const TileType& type) {

    //when getting intercepts the point 0,0 is at top left of the tile
    //positive x axis goes to right, positive y axis goes down
    switch(type) {

        case TileType::UPWARD_RIGHT_1_1:
        case TileType::DOWNWARD_LEFT_1_1: {

            return glm::vec2(TILE_SIZE, TILE_SIZE);
        }

        case TileType::UPWARD_LEFT_1_1:
        case TileType::DOWNWARD_RIGHT_1_1: {

            return glm::vec2(0, 0);
        }

        default:
            return glm::vec2(0, 0);
    }

    return glm::vec2(0, 0);
}

sf::Color getColorForTileType(const TileType& type) {

    switch(type) {

        case TileType::UPWARD_RIGHT_1_1:
        case TileType::DOWNWARD_LEFT_1_1: {

            return sf::Color::Red;
        }

        case TileType::UPWARD_LEFT_1_1:
        case TileType::DOWNWARD_RIGHT_1_1: {

            return sf::Color::Blue;
        }

        case TileType::SOLID: {

            return sf::Color::White;
        }

        case TileType::ONE_WAY: {

            return sf::Color::Green;
        }

        default:
            return sf::Color::Black;
    }

    return sf::Color::Black;
}

Tile::Tile(const glm::vec2& worldPosition,  const TileType& tileType) :
    type(tileType),
    tile(sf::Vector2f(TILE_SIZE, TILE_SIZE)),
    textureName(),
    textureRect(1, 1, 1, 1)
    {
        setupEmptyTileColor();
        tile.setPosition(sf::Vector2f(worldPosition.x, worldPosition.y));

        tile.setFillColor(getColorForTileType(type));
    }

bool Tile::loadTexture(const string &textureFilename) {

    if(textures.count(textureFilename) != 0) {

        //already loaded so just return true
        return true;
    }

    bool success = textures[textureFilename].loadFromFile(textureFilename);

    if(!success) {

        textures.erase(textureFilename);
    }

    return success;
}

void Tile::clearTextures() {

    textures.clear();
}

void Tile::setType(const TileType& tileType) {

    type = tileType;
}

bool Tile::setTexture(const std::string &textureName) {

    if(loadTexture(textureName)) {

        tile.setTexture(&textures[textureName]);
        tile.setFillColor(sf::Color::White);
        tile.setOutlineThickness(0);
        return true;
    }

    return false;
}

void Tile::setTextureRect(const sf::IntRect &textureRect) {

    tile.setTextureRect(textureRect);

    if(textureRect.width - textureRect.left == 0) {

        setupEmptyTileColor();
    }
}

sf::FloatRect Tile::getBoundingBox() const {

    sf::FloatRect bounds = tile.getGlobalBounds();

    return bounds;
}

TileType Tile::getType() const {

    return type;
}

std::string Tile::getTextureName() const {

    return textureName;
}

sf::IntRect Tile::getTextureRect() const {

    return textureRect;
}

void Tile::draw(sf::RenderTarget& window) {

    if(type != TileType::EMPTY) {

        window.draw(tile);
    }
}

void Tile::drawDebug(sf::RenderTarget& window) {

    window.draw(tile);
}

void Tile::setupEmptyTileColor() {

    tile.setFillColor(sf::Color::Black);
    tile.setOutlineColor(sf::Color::Red);
    tile.setOutlineThickness(-2);
}
