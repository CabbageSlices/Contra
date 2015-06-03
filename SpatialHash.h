#ifndef SPATIALHASH_H_INCLUDED
#define SPATIALHASH_H_INCLUDED

#include "SpatialHashEntry.h"
#include "glm/glm.hpp"
#include <vector>
#include <unordered_map>
#include <memory>

//the key-value pair for spatial hash should be <ivec2, map<unsigned, hashentry> >
//this way when you get the grid position of an object you can use the object's id to insert/remove it from the grid without using a linear search
//for now use a vector because i think using a map wouldn't give much benefits

typedef std::vector<std::shared_ptr<SpatialHashEntry> > entryContainer;

class SpatialHash {

    public:

        SpatialHash(const unsigned &hashCellWidth, const unsigned &hashCellHeight);

        void insert(shared_ptr<SpatialHashEntry> &entry);
        void remove(shared_ptr<SpatialHashEntry> &entry);
        void updatePosition(shared_ptr<SpatialHashEntry> &entry);

        entryContainer getSurroundingEntites(const sf::FloatRect boundingBox);

    private:

        std::unordered_map<glm::ivec2, entryContainer> hash;

        unsigned cellWidth;
        unsigned cellHeight;
};

#endif // SPATIALHASH_H_INCLUDED
