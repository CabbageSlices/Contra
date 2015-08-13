#ifndef POSITIONCONTROLLER_H_INCLUDED
#define POSITIONCONTROLLER_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include "ObjectHitbox.h"

//objects that other classes can use to manage their velocities and handle movement
//the objects using this will have to keep track of their own hitboxes
//this object is used to that all other objects have a reusable interface when colliding with tiles
///VELOCITY DATA GIVEN TO THIS OBJECT NEEDS TO BE IN METERS PER SECOND
class HitboxMovementController {

    public:

        //specify the size of the bounding box
        //and the max velocity the object can reach on each axis
        HitboxMovementController(const glm::vec2& gravity, const glm::vec2& terminalVelocities, ObjectHitbox *hurtbox);

        void setHitbox(ObjectHitbox *hurtbox);
        void setVelocities(const glm::vec2& velocities);
        void setVelocities(const float& xVelocity, const float& yVelocity);

        const glm::vec2 getVelocities() const;
        const float getLastDelta() const;
        ObjectHitbox* getHitbox();

        //update velocities applies gravity to the velocities
        void updateVelocities(const float& delta);

        //does not check if out of bounds
        void move(const float& delta);

        //returns true if object hit the world bounds and snapped to the edge
        //useful if you want to set velocity to 0 or allow jumping when on top of a surface
        bool moveAlongXAxis(const float& delta, const sf::FloatRect& worldBounds);
        bool moveAlongYAxis(const float& delta, const sf::FloatRect& worldBounds);

    private:

        void capVelocities();

        ObjectHitbox* objectHitbox;

        //units are all in meters/second
        //gravity only affects y-axis
        const glm::vec2 gravity;
        glm::vec2 terminalVelocities;

        glm::vec2 velocities;

        //keep track of the last delta time because tile collision requires dt in order to calculate previous position
        float previousDeltaTime;
};

#endif // POSITIONCONTROLLER_H_INCLUDED
