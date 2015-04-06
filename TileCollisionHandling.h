#ifndef TILECOLLISIONHANDLING_H_INCLUDED
#define TILECOLLISIONHANDLING_H_INCLUDED

#include "Tile.h"
#include "PositionController.h"

typedef PositionController PositionObject;

//functions that solve collisions between position objects and tiles
//character movement is broken into horizontal and vertical parts
//so collision should work seperately as well
//function returns wether object is standing on top of the block now, useful for resetting jump
bool handleCollisionHorizontal(Tile& tile, PositionObject& object);

void handleSolidTileCollisionHorizontal(Tile& tile, PositionObject& object);

bool handleCollisionVertical(Tile& tile, PositionObject& object);

bool handleSolidTileCollisionVertical(Tile& tile, PositionObject& object);

#endif // TILECOLLISIONHANDLING_H_INCLUDED
