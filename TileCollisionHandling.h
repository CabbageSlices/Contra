#ifndef TILECOLLISIONHANDLING_H_INCLUDED
#define TILECOLLISIONHANDLING_H_INCLUDED

#include "Tile.h"
#include "PositionController.h"

typedef PositionController PositionObject;

//when player is on a slope he needs to clip to his new height
//but if he clips from too far away it looks wrong, so save the max distance player can clip
extern const int MAX_SLOPE_SNAPPING_DISTANCE;

//functions that solve collisions between position objects and tiles
//character movement is broken into horizontal and vertical parts
//so collision should work seperately as well
//function returns wether object is standing on top of the block now, useful for resetting jump
bool handleCollisionHorizontal(Tile& tile, PositionObject& object);
bool handleCollisionVertical(Tile& tile, PositionObject& object);

void handleSolidTileCollisionHorizontal(Tile& tile, PositionObject& object);
bool handleUpSlopeTileCollision(Tile& tile, PositionObject& object);

bool handleSolidTileCollisionVertical(Tile& tile, PositionObject& object);

#endif // TILECOLLISIONHANDLING_H_INCLUDED
