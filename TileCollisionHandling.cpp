#include "TileCollisionHandling.h"
#include "ObjectSpaceManager.h"
#include "GlobalConstants.h"

#include <iostream>

using std::cout;
using std::endl;

bool handleCollisionHorizontal(Tile& tile, PositionObject& object) {

    TileType type = tile.getType();

    switch(type) {

        case TileType::SOLID: {

            handleSolidTileCollisionHorizontal(tile, object);
            return false;
        }

        default:
            return false;
    }

    return false;
}

bool handleCollisionVertical(Tile& tile, PositionObject& object) {

    TileType type = tile.getType();

    switch(type) {

        case TileType::ONE_WAY:
        case TileType::SOLID: {

            return handleSolidTileCollisionVertical(tile, object);
        }

        default:
            return false;
    }

    return false;
}

void handleSolidTileCollisionHorizontal(Tile& tile, PositionObject& object) {

    sf::FloatRect tileBoundingBox = tile.getBoundingBox();

    if(!tileBoundingBox.intersects(object.getBoundingBoxWorldSpace())) {

        return;
    }

    const ObjectSpaceManager &objectSpace = object.getObjectSpace();

    //handle collision in the objects space because tile could have different edge in object space
    glm::vec2 tilePosObjectSpace = objectSpace.convertToObjectSpace(glm::vec2(tileBoundingBox.left, tileBoundingBox.top));
    glm::vec2 tileSizeObjectSpace = objectSpace.convertToObjectSpace(glm::vec2(tileBoundingBox.width, tileBoundingBox.height));

    glm::vec2 objPosObjectSpace = objectSpace.getPositionObjectSpace();
    glm::vec2 objSizeObjectSpace = objectSpace.getSizeObjectSpace();

    float tileLeft = glm::min(tilePosObjectSpace.x, tilePosObjectSpace.x + tileSizeObjectSpace.x);
    float tileRight = glm::max(tilePosObjectSpace.x, tilePosObjectSpace.x + tileSizeObjectSpace.x);

    if(objPosObjectSpace.x < tileLeft) {

        object.setPositionObjectSpace(glm::vec2(tileLeft - objSizeObjectSpace.x, objPosObjectSpace.y));
        return;

    } else {

        object.setPositionObjectSpace(glm::vec2(tileRight, objPosObjectSpace.y));
    }
}

bool handleSolidTileCollisionVertical(Tile& tile, PositionObject& object) {

    sf::FloatRect tileBoundingBox = tile.getBoundingBox();

    if(!object.getBoundingBoxObjectSpace().intersects(object.getObjectSpace().convertToObjectSpace(tileBoundingBox)  )) {

        return false;
    }

    const ObjectSpaceManager &objectSpace = object.getObjectSpace();

    //handle collision in the objects space because tile could have different top in object space
    glm::vec2 tilePosObjectSpace = objectSpace.convertToObjectSpace(glm::vec2(tileBoundingBox.left, tileBoundingBox.top));
    glm::vec2 tileSizeObjectSpace = objectSpace.convertToObjectSpace(glm::vec2(tileBoundingBox.width, tileBoundingBox.height));

    glm::vec2 objPosObjectSpace = objectSpace.getPositionObjectSpace();
    glm::vec2 objSizeObjectSpace = objectSpace.getSizeObjectSpace();

    float tileTop = glm::min(tilePosObjectSpace.y, tilePosObjectSpace.y + tileSizeObjectSpace.y);
    float tileBottom = glm::max(tilePosObjectSpace.y, tilePosObjectSpace.y + tileSizeObjectSpace.y);

    float previousPosition = objPosObjectSpace.y + objSizeObjectSpace.y - object.getVelocitiesObjectSpace().y * METERS_TO_PIXEL_RATIO * object.getLastDelta();

    //one way tiles only collide if object jumps on top of the tile
    //round the position because transforming between coordinate spaces might introduce some errors
    if(glm::round(previousPosition) > tileTop && tile.getType() == TileType::ONE_WAY) {

        //object jumped from underneath the tile so don't collide
        return false;
    }

    object.setVelocities(object.getVelocitiesObjectSpace().x, 0);

    if(objPosObjectSpace.y < tileTop) {

        object.setPositionObjectSpace(glm::vec2(objPosObjectSpace.x, tileTop - objSizeObjectSpace.y));
        return true;

    } else if(tile.getType() != TileType::ONE_WAY){

        object.setPositionObjectSpace(glm::vec2(objPosObjectSpace.x, tileBottom));
    }

    return false;
}
