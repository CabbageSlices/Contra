#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include "HitboxMovementController.h"
#include <string>
#include <map>

extern const int TILE_SIZE;

//typedef the position controller incase you want to change to pointer type later
typedef HitboxMovementController HitboxMovementController;

enum TileType {

    EMPTY = 0,
    SOLID,
    PASS_THROUGH,//tiles that dont handle collision but have an image
    UPWARD_RIGHT_1_1, //upward slopes collide with bottom of entity
    UPWARD_LEFT_1_1,  //different direction chooses whether it collides with left or right side
    DOWNWARD_RIGHT_1_1, //downward slopes only collide with top of entity
    DOWNWARD_LEFT_1_1, //numbers are absolute value of slope, first number is run, second number is rise
    ONE_WAY,
};

glm::vec2 getSlopeForTileType(const TileType& type);
glm::vec2 getInterceptsForTileType(const TileType& type);

sf::Color getColorForTileType(const TileType& type);

class Tile {

    public:

        Tile(const glm::vec2 &worldPosition, const TileType& tileType);

        static bool loadTexture(const std::string &textureFileName);
        static void clearTextures();

        void setType(const TileType& tileType);
        bool setTexture(const std::string &texture);
        void setTextureRect(const sf::IntRect &textureRect);

        //bounding box in world space
        sf::FloatRect getBoundingBox() const;
        TileType getType() const;
        std::string getTextureName() const; //returns the filename of the texture being used by this tile
        sf::IntRect getTextureRect() const;

        void draw(sf::RenderTarget& window);
        void drawDebug(sf::RenderTarget& window);

    private:

        void setupEmptyTileColor();

        static std::map<std::string, sf::Texture> textures;

        TileType type;

        //drawing purposes
        sf::RectangleShape tile;

        //name of texture used by this tile
        std::string textureName;

        sf::IntRect textureRect;
};

#endif // TILE_H_INCLUDED
