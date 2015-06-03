#ifndef SPATIALHASHENTRY_H_INCLUDED
#define SPATIALHASHENTRY_H_INCLUDED

#include "EntityBase.h"
#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"
#include <memory>

//typedef the type of object managed by the entry that way i can easily change it later
typedef EntityBase Object;

//spatial hash is defind as a map of <vec2i point, map<id, hashentry> > that way once you determine the location of the object in the hash using its position,
//you can use the objects id to quickly grab the object from the grid without having to do a linear search
class SpatialHashEntry {

    public:

        SpatialHashEntry(std::shared_ptr<Object> &hashEntry);

        void updateBoundingBox();

        //checks if the object still exists, e.g the pointer is still valid
        bool checkObjectExists();

        std::weak_ptr<Object> getObject();
        unsigned getId() const;
        sf::FloatRect getPreviousBoundingBox() const;

    private:

        std::weak_ptr<Object> object;

        //bounding box of the object during the previous frame
        //keep track of this that way when the object moves you can compute the new area of the hash that the object encloses
        sf::FloatRect previousBoundingBox;
        unsigned id;

        //id of each hash entry will just be the number of entries created at the moment the object was created
        static unsigned hashEntriesCreated;
    };

#endif // SPATIALHASHENTRY_H_INCLUDED
