#ifndef DYNAMICOBJECT_H_INCLUDED
#define DYNAMICOBJECT_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "EntityBase.h"
#include "CollisionResponse.h"
#include "glm/glm.hpp"

#include <memory>

class DynamicObject : public EntityBase {

    public:

        DynamicObject(const glm::vec2 &gravity, const glm::vec2 &movementVelocity, const glm::vec2 &terminalVelocity, const unsigned &initialHealth) :
            EntityBase(gravity, movementVelocity, terminalVelocity, initialHealth)
            {

            }

        virtual ~DynamicObject() {

        }
};



#endif // DYNAMICOBJECT_H_INCLUDED
