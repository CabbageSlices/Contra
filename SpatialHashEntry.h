#ifndef SPATIALHASHENTRY_H_INCLUDED
#define SPATIALHASHENTRY_H_INCLUDED

#include "EntityBase.h"
#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"
#include <memory>

//spatial hash is defind as a map of <vec2i point, map<id, hashentry> > that way once you determine the location of the object in the hash using its position,
//you can use the objects id to quickly grab the object from the grid without having to do a linear search
template<class Object>
class SpatialHashEntry {

    public:

        SpatialHashEntry(std::shared_ptr<Object> &hashEntry);

        void updateBoundingBox();

        //checks if the object still exists, e.g the pointer is still valid
        bool checkObjectExists();

        std::shared_ptr<Object> getObject();
        unsigned getId() const;
        sf::FloatRect getPreviousBoundingBox() const;

    private:

        std::shared_ptr<Object> object;

        //bounding box of the object during the previous frame
        //keep track of this that way when the object moves you can compute the new area of the hash that the object encloses
        sf::FloatRect previousBoundingBox;
        unsigned id;

        //id of each hash entry will just be the number of entries created at the moment the object was created
        static unsigned hashEntriesCreated;
};

template<class Object> unsigned SpatialHashEntry<Object>::hashEntriesCreated = 0;

template<class Object> SpatialHashEntry<Object>::SpatialHashEntry(std::shared_ptr<Object> &hashEntry) :
    object(hashEntry),
    previousBoundingBox(0, 0, 0, 0),
    id(hashEntriesCreated)
    {

        hashEntriesCreated += 1;
    }

template<class Object> void SpatialHashEntry<Object>::updateBoundingBox() {

    if(!checkObjectExists()) {

        return;
    }

    previousBoundingBox = object.lock()->getHitbox().getTotalHitboxWorldSpace();
}

template<class Object> bool SpatialHashEntry<Object>::checkObjectExists() {

    return bool(object.lock());
}

template<class Object> std::shared_ptr<Object> SpatialHashEntry<Object>::getObject() {

    return object;
}

template<class Object> unsigned SpatialHashEntry<Object>::getId() const {

    return id;
}

template<class Object> sf::FloatRect SpatialHashEntry<Object>::getPreviousBoundingBox() const {

    return previousBoundingBox;
}


#endif // SPATIALHASHENTRY_H_INCLUDED
