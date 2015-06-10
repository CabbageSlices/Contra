#ifndef SPATIALHASH_H_INCLUDED
#define SPATIALHASH_H_INCLUDED

#include "SpatialHashEntry.h"
#include "glm/glm.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <boost/functional/hash.hpp>
#include <set>


//the key-value pair for spatial hash should be <ivec2, map<unsigned, hashentry> >
//this way when you get the grid position of an object you can use the object's id to insert/remove it from the grid without using a linear search
//for now use a vector because i think using a map wouldn't give much benefits

struct Hasher {

    size_t operator()(const glm::ivec2 &point) const {

        size_t seed = 0;
        boost::hash_combine(seed, point.x);
        boost::hash_combine(seed, point.y);

        return seed;
    }
};

template<class Object>
class SpatialHash {

    public:

        typedef SpatialHashEntry<Object> HashEntry;
        typedef std::vector<std::shared_ptr<HashEntry> > EntryContainer;

        SpatialHash(const unsigned &hashCellWidth, const unsigned &hashCellHeight);

        //if its known that an objects arealdy been inserted into the hash its better to call updatePosition
        //instead of using insert
        //insert assumes object isn't inside the grid
        void insert(std::shared_ptr<HashEntry> &entry);
        void remove(std::shared_ptr<HashEntry> &entry);
        void updateLocation(std::shared_ptr<HashEntry> &entry);

        std::set<std::shared_ptr<HashEntry> > getSurroundingEntites(const sf::FloatRect boundingBox);

    private:

        //insert the entry from its given bounding box
        void insert(std::shared_ptr<HashEntry> &entry, const sf::IntRect &enclosedGrid);

        //remove an entry from the given bounding box
        void remove(std::shared_ptr<HashEntry> &entry, const sf::IntRect &enclosedGrid);

        //returns the extens of the grid enclosed by the object
        //width and height refer to the right edge of the grid, and not the actual widht and height
        //i.e width and height refer to left + width, top + height respectively
        sf::IntRect calculateEnclosedGrid(const sf::FloatRect &entryBoundingBox);

        std::unordered_map<glm::ivec2, EntryContainer, Hasher> hash;

        unsigned cellWidth;
        unsigned cellHeight;
};

template<class Object> SpatialHash<Object>::SpatialHash(const unsigned &hashCellWidth, const unsigned &hashCellHeight) :
    hash(),
    cellWidth(hashCellWidth),
    cellHeight(hashCellHeight)
    {

    }

template<class Object> void SpatialHash<Object>::insert(std::shared_ptr<HashEntry> &entry) {

    //make sure the object is using its most up to date position
    entry->updateBoundingBox();

    //get current bounding box to determine which cells the entry occupies
    sf::FloatRect boundingBox = entry->getPreviousBoundingBox();

    sf::IntRect gridBounds = calculateEnclosedGrid(boundingBox);

    insert(entry, gridBounds);
}

template<class Object> void SpatialHash<Object>::remove(std::shared_ptr<HashEntry> &entry) {

    //don't use the latest hitbox since objects position in the hash is determined by his previous hitbox, not current one
    sf::FloatRect boundingBox = entry->getPreviousBoundingBox();

    //find what grids object occupied and remove it from those places
    sf::IntRect gridBounds = calculateEnclosedGrid(boundingBox);

    remove(entry, gridBounds);
}

template<class Object> void SpatialHash<Object>::updateLocation(std::shared_ptr<HashEntry> &entry) {

    //check if objects previous position and current position coincide
    //if they do, then it hasn't moved so no need to move it
    sf::FloatRect previousBoundingBox = entry->getPreviousBoundingBox();
    entry->updateBoundingBox();
    sf::FloatRect currentBoundingBox = entry->getPreviousBoundingBox();

    //find what grids object occupied and remove it from those places
    sf::IntRect previousGridBounds = calculateEnclosedGrid(previousBoundingBox);
    sf::IntRect currentGridBounds = calculateEnclosedGrid(currentBoundingBox);

    //no change
    if(previousGridBounds == currentGridBounds) {

        return;
    }

    //changed so remove object from previous position
    //use the stored value of previousGridBounds because entry has been updated so calling getPreviousBoundingBox will return current box
    remove(entry, previousGridBounds);
    insert(entry, currentGridBounds);
}

template<class Object> std::set<std::shared_ptr<SpatialHashEntry<Object> > > SpatialHash<Object>::getSurroundingEntites(const sf::FloatRect boundingBox) {

    std::set<std::shared_ptr<HashEntry> > container;

    sf::IntRect enclosedGrid = calculateEnclosedGrid(boundingBox);

    for(int y = enclosedGrid.top; y <= enclosedGrid.height; ++y) {

        for(int x = enclosedGrid.left; x <= enclosedGrid.width; ++x) {

            glm::ivec2 gridPosition(x, y);
            EntryContainer &currentContainer = hash[gridPosition];

            container.insert(currentContainer.begin(), currentContainer.end());
        }
    }

    return container;
}


template<class Object> void SpatialHash<Object>::insert(std::shared_ptr<HashEntry> &entry, const sf::IntRect &enclosedGrid) {

    for(int y = enclosedGrid.top; y <= enclosedGrid.height; ++y) {

        for(int x = enclosedGrid.left; x <= enclosedGrid.width; ++x) {

            glm::ivec2 gridPosition(x, y);
            EntryContainer &currentContainer = hash[gridPosition];

            currentContainer.push_back(entry);
        }
    }
}


template<class Object> void SpatialHash<Object>::remove(std::shared_ptr<HashEntry> &entry, const sf::IntRect &enclosedGrid) {

    for(int y = enclosedGrid.top; y <= enclosedGrid.height; ++y) {

        for(int x = enclosedGrid.left; x <= enclosedGrid.width; ++x) {

            glm::ivec2 gridPosition(x, y);
            EntryContainer &currentContainer = hash[gridPosition];

            //find entity in the container and remove
            typename EntryContainer::iterator it = remove_if(currentContainer.begin(), currentContainer.end(),
                                                [&](std::shared_ptr<HashEntry> val)->bool{

                                                    return val->getId() == entry->getId();
                                                } );

            currentContainer.erase(it, currentContainer.end());
        }
    }
}

template<class Object> sf::IntRect SpatialHash<Object>::calculateEnclosedGrid(const sf::FloatRect &entryBoundingBox) {

    sf::IntRect enclosedGrid;

    enclosedGrid.left = entryBoundingBox.left / cellWidth;
    enclosedGrid.top = entryBoundingBox.top / cellHeight;
    enclosedGrid.width = ((entryBoundingBox.left + entryBoundingBox.width) / static_cast<float>(cellWidth));
    enclosedGrid.height = ((entryBoundingBox.top + entryBoundingBox.height) / static_cast<float>(cellHeight));

    return enclosedGrid;
}


#endif // SPATIALHASH_H_INCLUDED
