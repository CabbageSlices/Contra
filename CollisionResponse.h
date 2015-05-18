#ifndef COLLISIONRESPONSE_H_INCLUDED
#define COLLISIONRESPONSE_H_INCLUDED

struct CollisionResponse {

    bool handledHorizontal = false;
    bool pushedToTop = false;
    bool canFallThroughGround = false;
};

#endif // COLLISIONRESPONSE_H_INCLUDED
