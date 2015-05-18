#ifndef DYNAMICOBJECT_H_INCLUDED
#define DYNAMICOBJECT_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "EntityBase.h"
#include "CollisionResponse.h"
#include "glm/glm.hpp"

class DynamicObject : public EntityBase {

    public:

        DynamicObject(const glm::vec2 &gravity, const glm::vec2 &movementVelocity, const glm::vec2 &terminalVelocity, const unsigned &initialHealth) :
            EntityBase(gravity, movementVelocity, terminalVelocity, initialHealth)
            {

            }

        virtual CollisionResponse handleCollision(EntityBase *collidingEntity) = 0;
};

//calculates the minimum distance required for rectA to move in order to escape collision with rectB
glm::vec2 calculateCollisionResolutionTranslation(sf::FloatRect rectA, sf::FloatRect rectB);

#endif // DYNAMICOBJECT_H_INCLUDED
