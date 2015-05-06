#ifndef TILECOLLISIONHANDLING_H_INCLUDED
#define TILECOLLISIONHANDLING_H_INCLUDED

#include "Tile.h"
#include "HitboxMovementController.h"
#include <memory>

//tile collision information to indicate on which axis collision was handled
struct CollisionResponse {

    bool handledHorizontal = false;
    bool handledVertical = false;
    bool canFallThroughGround = false;
};

//if the bottom of an object is within this distance from the top of a colliding tile the collision resoultion occurs
//otherwise it is skipped
//this allows players to jump down from passable platforms
extern const float fallThroughTopThreshold;

typedef HitboxMovementController HitboxMovementController;

//when player is on a slope he needs to clip to his new height
//but if he clips from too far away it looks wrong, so save the max distance player can clip
extern const int MAX_SLOPE_SNAPPING_DISTANCE;

bool checkSolidTileIntersection(std::shared_ptr<Tile>& tile, HitboxMovementController& object);

//functions that solve collisions between position objects and tiles
//character movement is broken into horizontal and vertical parts
//so collision should work seperately as well
//function returns wether object is standing on top of the block now, useful for resetting jump
CollisionResponse handleCollisionHorizontal(std::shared_ptr<Tile>& tile, HitboxMovementController& object);
CollisionResponse handleCollisionVertical(std::shared_ptr<Tile>& tile, HitboxMovementController& object);

bool handleSolidTileCollisionHorizontal(std::shared_ptr<Tile>& tile, HitboxMovementController& object);
bool handleUpSlopeTileCollision(std::shared_ptr<Tile>& tile, HitboxMovementController& object);

bool handleSolidTileCollisionVertical(std::shared_ptr<Tile>& tile, HitboxMovementController& object);

#endif // TILECOLLISIONHANDLING_H_INCLUDED
