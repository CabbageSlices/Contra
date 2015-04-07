#include "TileCollisionHandling.h"
#include "ObjectSpaceManager.h"
#include "GlobalConstants.h"

#include <iostream>

using std::shared_ptr;
using std::cout;
using std::endl;

const int MAX_SLOPE_SNAPPING_DISTANCE = 10;

bool handleCollisionHorizontal(shared_ptr<Tile>& tile, PositionObject& object) {

    TileType type = tile->getType();

    switch(type) {

        case TileType::SOLID: {

            handleSolidTileCollisionHorizontal(tile, object);
            return false;
        }

        case TileType::UPWARD_LEFT_1_1:
        case TileType::UPWARD_RIGHT_1_1: {

            return handleUpSlopeTileCollision(tile, object);
        }

        default:
            return false;
    }

    return false;
}

bool handleCollisionVertical(shared_ptr<Tile>& tile, PositionObject& object) {

    TileType type = tile->getType();

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

void handleSolidTileCollisionHorizontal(shared_ptr<Tile>& tile, PositionObject& object) {

    sf::FloatRect tileBoundingBox = tile->getBoundingBox();

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

bool handleUpSlopeTileCollision(shared_ptr<Tile>& tile, PositionObject& object) {

    const ObjectSpaceManager& objectSpace = object.getObjectSpace();

    sf::FloatRect tileBoundingBox = tile->getBoundingBox();

    glm::vec2 tilePosObjectSpace = objectSpace.convertToObjectSpace(glm::vec2(tileBoundingBox.left, tileBoundingBox.top));
    glm::vec2 tileSizeObjectSpace = objectSpace.convertToObjectSpace(glm::vec2(tileBoundingBox.width, tileBoundingBox.height));

    glm::vec2 objPosObjectSpace = objectSpace.getPositionObjectSpace();
    glm::vec2 objSizeObjectSpace = objectSpace.getSizeObjectSpace();

    glm::vec2 tileSlope = getSlopeForTileType(tile->getType());
    glm::vec2 tileSlopeObjSpace = objectSpace.convertToObjectSpace(tileSlope);

    glm::vec2 tileIntercepts = getInterceptsForTileType(tile->getType());
    glm::vec2 tileInterceptsObjSpace = objectSpace.convertToObjectSpace(tileIntercepts);

    //for slope position calculations, use the center of the bottom of the object
    //also make it so tile position is at 0,0
    glm::vec2 objPosInTile = objPosObjectSpace + glm::vec2(objSizeObjectSpace.x / 2, objSizeObjectSpace.y) - tilePosObjectSpace;

    glm::vec2 previousPosition = objPosInTile - glm::vec2(object.getVelocitiesObjectSpace().x, 0) * METERS_TO_PIXEL_RATIO * object.getLastDelta();
    int previousPositionSlopeHeight = (tileSlopeObjSpace.y / tileSlopeObjSpace.x) * previousPosition.x + tileInterceptsObjSpace.y;

    //sometimes when player is standing on top of a slope, the height calculated in the previous frame differs from his current height
    //even though nothing has been changed. The player seems to move down by 1 pixel so when checking if player is below slope before
    //mvoe the slope down by one pixel
    if(previousPosition.y > previousPositionSlopeHeight + 1) {

        //player was below slope before so don't handle collision
        return false;
    }

    float rightEdge = glm::max(tilePosObjectSpace.x, tilePosObjectSpace.x + tileSizeObjectSpace.x) - tilePosObjectSpace.x;
    float leftEdge = glm::min(tilePosObjectSpace.x, tilePosObjectSpace.x + tileSizeObjectSpace.x) - tilePosObjectSpace.x;

    //if object is off the upper end of the slope in the last frame, as well as this frame, then don't snap him to slope
    //but he should be snapped to slope if he was on the slope last frame, but got off this frame
    //upper end of slope depends on direction of slope
    if(tileSlopeObjSpace.y  / tileSlopeObjSpace.x > 0 && objPosInTile.x < leftEdge && previousPosition.x < leftEdge) {

        return false;

    } else if(tileSlopeObjSpace.y  / tileSlopeObjSpace.x < 0 && objPosInTile.x > rightEdge && previousPosition.x > rightEdge) {

        return false;
    }

    //object is jumping, don't snap to slope
    if(object.getVelocitiesObjectSpace().y < 0) {

        return false;
    }

    int currentSlopeHeight = (tileSlopeObjSpace.y / tileSlopeObjSpace.x) * objPosInTile.x + tileInterceptsObjSpace.y;

    //object needs to snap to tiles, make sure the snapping distance isn't too large
    if(currentSlopeHeight - objPosInTile.y > MAX_SLOPE_SNAPPING_DISTANCE) {

        return false;
    }

    glm::vec2 tileSize = objectSpace.convertToObjectSpace(glm::vec2(TILE_SIZE, TILE_SIZE));

    //if object is off the lower end of the slope, it could have walked off the slope
    bool snapToBottom = (tileSlopeObjSpace.y  / tileSlopeObjSpace.x > 0 && objPosInTile.x > tileSize.x && previousPosition.x <= tileSize.x) ||
                        (tileSlopeObjSpace.y  / tileSlopeObjSpace.x < 0 && objPosInTile.x < 0 && previousPosition.x >= 0);

    if(snapToBottom) {

        float tileBottom = glm::max(tilePosObjectSpace.y, tilePosObjectSpace.y + tileSize.y);

        //object walked off edge of slope so snap to bottom of tile
        float yPosObjectSpace = tileBottom - objSizeObjectSpace.y;
        float xPosObjectSpace = objPosObjectSpace.x;

        object.setPositionObjectSpace(glm::vec2(xPosObjectSpace, yPosObjectSpace));
        return true;
    }

    float yPosObjectSpace = currentSlopeHeight + tilePosObjectSpace.y - objSizeObjectSpace.y;
    float xPosObjectSpace = objPosObjectSpace.x;

    object.setPositionObjectSpace(glm::vec2(xPosObjectSpace, yPosObjectSpace));
    object.setVelocities(object.getVelocitiesObjectSpace().x, 0);
    return true;
}

bool handleSolidTileCollisionVertical(shared_ptr<Tile>& tile, PositionObject& object) {

    sf::FloatRect tileBoundingBox = tile->getBoundingBox();

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
    if(glm::round(previousPosition) > tileTop && tile->getType() == TileType::ONE_WAY) {

        //object jumped from underneath the tile so don't collide
        return false;
    }

    object.setVelocities(object.getVelocitiesObjectSpace().x, 0);

    if(objPosObjectSpace.y < tileTop) {

        object.setPositionObjectSpace(glm::vec2(objPosObjectSpace.x, tileTop - objSizeObjectSpace.y));
        return true;

    } else if(tile->getType() != TileType::ONE_WAY){

        object.setPositionObjectSpace(glm::vec2(objPosObjectSpace.x, tileBottom));
    }

    return false;
}
