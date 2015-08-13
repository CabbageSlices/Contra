#include "TileCollisionHandling.h"
#include "ObjectHitbox.h"
#include "GlobalConstants.h"

#include <iostream>

using std::shared_ptr;
using std::cout;
using std::endl;

const float fallThroughTopThreshold = 10.f;
const int MAX_SLOPE_SNAPPING_DISTANCE = 10;

bool checkSolidTileIntersection(std::shared_ptr<Tile>& tile, HitboxMovementController& object) {

    if(!object.getHitbox()) {

        return false;
    }

    TileType type = tile->getType();

    sf::FloatRect tileBoundingBox = tile->getBoundingBox();
    sf::FloatRect objBoundingBox = object.getHitbox()->getActiveHitboxWorldSpace();

    switch(type) {

        case TileType::SOLID: {

            return tileBoundingBox.intersects(objBoundingBox);
        }

        default:
            return false;
    }

    return false;
}

CollisionResponse handleCollisionHorizontal(shared_ptr<Tile>& tile, HitboxMovementController& object) {

    TileType type = tile->getType();

    CollisionResponse response;

    switch(type) {

        case TileType::SOLID: {

            response.handledHorizontal = handleSolidTileCollisionHorizontal(tile, object);
            return response;
        }

        case TileType::UPWARD_LEFT_1_1:
        case TileType::UPWARD_RIGHT_1_1: {

            response.pushedToTop = handleUpSlopeTileCollision(tile, object);
            response.canFallThroughGround = response.pushedToTop; //object can fall through this tile if the collision was handled
            return response;
        }

        default:
            return response;
    }

    return response;
}

CollisionResponse handleCollisionVertical(shared_ptr<Tile>& tile, HitboxMovementController& object) {

    TileType type = tile->getType();

    CollisionResponse response;

    switch(type) {

        case TileType::ONE_WAY: {

            response.pushedToTop = handleSolidTileCollisionVertical(tile, object);
            response.canFallThroughGround = response.pushedToTop; //object can fall through this tile if the collision was handled
            return response;
        }
        case TileType::SOLID: {

            response.pushedToTop = handleSolidTileCollisionVertical(tile, object);
            return response;
        }

        default:
            return response;
    }

    return response;
}

bool handleSolidTileCollisionHorizontal(shared_ptr<Tile>& tile, HitboxMovementController& object) {

    if(!object.getHitbox()) {

        return false;
    }

    sf::FloatRect tileBoundingBox = tile->getBoundingBox();
    const sf::FloatRect objectHitbox = object.getHitbox()->getActiveHitboxWorldSpace();

    if(!tileBoundingBox.intersects(objectHitbox)) {

        return false;
    }

    float tileLeft = tileBoundingBox.left;
    float tileRight = tileBoundingBox.left + tileBoundingBox.width;

    float offset = 0;

    if(objectHitbox.left < tileLeft) {

        offset = tileLeft - (objectHitbox.left + objectHitbox.width);

    } else {

        offset = tileRight - objectHitbox.left;
    }

    object.getHitbox()->move(glm::vec2(offset, 0));

    return true;
}

bool handleUpSlopeTileCollision(shared_ptr<Tile>& tile, HitboxMovementController& object) {

    if(!object.getHitbox()) {

        return false;
    }

    const sf::FloatRect& objectHitbox = object.getHitbox()->getActiveHitboxWorldSpace();
    sf::FloatRect tileBoundingBox = tile->getBoundingBox();

    glm::vec2 tileSlope = getSlopeForTileType(tile->getType());
    glm::vec2 tileIntercepts = getInterceptsForTileType(tile->getType());
    glm::vec2 tilePosition(tileBoundingBox.left, tileBoundingBox.top);

    glm::vec2 objPosition(objectHitbox.left, objectHitbox.top);
    glm::vec2 objSize(objectHitbox.width, objectHitbox.height);

    //for slope position calculations, use the center of the bottom of the object
    //also make it so tile position is at 0,0
    glm::vec2 objPosInTile = objPosition + glm::vec2(objSize.x / 2, objSize.y) - tilePosition;

    glm::vec2 previousPosition = objPosInTile - glm::vec2(object.getVelocities().x, 0) * METERS_TO_PIXEL_RATIO * object.getLastDelta();
    int previousPositionSlopeHeight = (tileSlope.y / tileSlope.x) * previousPosition.x + tileIntercepts.y;

    //sometimes when player is standing on top of a slope, the height calculated in the previous frame differs from his current height
    //even though nothing has been changed. The player seems to move down by 1 pixel so when checking if player is below slope before
    //mvoe the slope down by one pixel
    if(previousPosition.y > previousPositionSlopeHeight + 1) {

        //player was below slope before so don't handle collision
        return false;
    }

    float leftEdgeTileSpace = 0;
    float rightEdgeTileSpace = tileBoundingBox.width;

    //if object is off the upper end of the slope in the last frame, as well as this frame, then don't snap him to slope
    //but he should be snapped to slope if he was on the slope last frame, but got off this frame
    //upper end of slope depends on direction of slope
    if(tileSlope.y  / tileSlope.x > 0 && objPosInTile.x < leftEdgeTileSpace && previousPosition.x < leftEdgeTileSpace) {

        return false;

    } else if(tileSlope.y  / tileSlope.x < 0 && objPosInTile.x > rightEdgeTileSpace && previousPosition.x > rightEdgeTileSpace) {

        return false;
    }

    //when object is jumping, if he moves towards the slope he will jump into the slope and skip collision
    //so also check if he is moving into the slope while jumping
    if(object.getVelocities().y < 0) {

        bool movingIntoLeftSlope = (tileSlope.y / tileSlope.x > 0 && object.getVelocities().x < 0);
        bool movingIntoRightSlope = (tileSlope.y / tileSlope.x < 0 && object.getVelocities().x > 0);

        //object is jumping without moving into a slope so its fine to skip this tile
        if(!movingIntoLeftSlope && !movingIntoRightSlope) {

            return false;
        }
    }

    int currentSlopeHeight = (tileSlope.y / tileSlope.x) * objPosInTile.x + tileIntercepts.y;

    //object needs to snap to tiles, make sure the snapping distance isn't too large
    if(currentSlopeHeight - objPosInTile.y > MAX_SLOPE_SNAPPING_DISTANCE) {

        return false;
    }

    //if object is off the lower end of the slope, it could have walked off the slope
    bool snapToBottom = (tileSlope.y  / tileSlope.x > 0 && objPosInTile.x > rightEdgeTileSpace && previousPosition.x <= rightEdgeTileSpace) ||
                        (tileSlope.y  / tileSlope.x < 0 && objPosInTile.x < leftEdgeTileSpace && previousPosition.x >= leftEdgeTileSpace);

    if(snapToBottom) {

        //calculate offset needed to move object so that it will be standing at bottom of slope
        float objectBottom = objectHitbox.top + objectHitbox.height;
        float tileBottom = tileBoundingBox.top + tileIntercepts.y;// bottom of tile is the y-intercept because its the point on the slope with the largest y value that can be reached with the slope

        float yOffset = tileBottom - objectBottom;

        object.getHitbox()->move(glm::vec2(0, yOffset));
        return true;
    }

    //object standing off lower end of slope but doesn't need to snap so don't do collision
    if((tileSlope.y  / tileSlope.x > 0 && objPosInTile.x > rightEdgeTileSpace && previousPosition.x > rightEdgeTileSpace)) {

        return false;

    } else if(tileSlope.y  / tileSlope.x < 0 && objPosInTile.x < leftEdgeTileSpace && previousPosition.x < leftEdgeTileSpace) {

        return false;
    }

    //object is in the tile and needs to snap to the y position at its current x position
    //calculate offset requried
    float yPositionTileSpace = (tileSlope.y / tileSlope.x) * objPosInTile.x + tileIntercepts.y;
    float yPositionWorldSpace = yPositionTileSpace + tileBoundingBox.top;
    float yOffset = yPositionWorldSpace - (objectHitbox.top + objectHitbox.height);

    object.getHitbox()->move(glm::vec2(0, yOffset));
    object.setVelocities(object.getVelocities().x, 0);
    return true;
}

bool handleSolidTileCollisionVertical(shared_ptr<Tile>& tile, HitboxMovementController& object) {

    if(!object.getHitbox()) {

        return false;
    }

    sf::FloatRect objectHitbox = object.getHitbox()->getActiveHitboxWorldSpace();
    sf::FloatRect tileBoundingBox = tile->getBoundingBox();

    if(!tileBoundingBox.intersects(objectHitbox)) {

        return false;
    }

    float tileTop = tileBoundingBox.top;
    float tileBottom = tileBoundingBox.top + tileBoundingBox.height;

    float previousPosition = objectHitbox.top + objectHitbox.height - object.getVelocities().y * METERS_TO_PIXEL_RATIO * object.getLastDelta();

    //one way tiles only collide if object jumps on top of the tile
    //round the position because floating point math might cause inaccuracies
    if(glm::round(previousPosition) > tileTop && tile->getType() == TileType::ONE_WAY) {

        //object jumped from underneath the tile so don't collide
        return false;
    }

    object.setVelocities(object.getVelocities().x, 0);

    if(objectHitbox.top + objectHitbox.height < tileTop + fallThroughTopThreshold) {

        float offset = tileTop - (objectHitbox.top + objectHitbox.height);
        object.getHitbox()->move(glm::vec2(0, offset));
        return true;

    } else if(tile->getType() != TileType::ONE_WAY) {

        float offset = tileBottom - objectHitbox.top;
        object.getHitbox()->move(glm::vec2(0, offset));
    }

    return false;
}
