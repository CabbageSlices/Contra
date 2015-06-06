#ifndef SPATIALHASH_H_INCLUDED
#define SPATIALHASH_H_INCLUDED

#include "SpatialHashEntry.h"
#include "glm/glm.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <boost/functional/hash.hpp>


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

        EntryContainer getSurroundingEntites(const sf::FloatRect boundingBox);

    private:

        //insert the entry from the given grid bounds
        void insert(std::shared_ptr<HashEntry> &entry, const sf::IntRect &gridBounds);

        //remove an entry from the given grid bounds
        void remove(std::shared_ptr<HashEntry> &entry, const sf::IntRect &gridBounds);
        sf::IntRect calculateGridBounds(const sf::FloatRect &entryBoundingBox);

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

    sf::IntRect gridBounds = calculateGridBounds(boundingBox);

    insert(entry, gridBounds);
}

template<class Object> void SpatialHash<Object>::remove(std::shared_ptr<HashEntry> &entry) {

    //don't use the latest hitbox since objects position in the hash is determined by his previous hitbox, not current one
    sf::FloatRect boundingBox = entry->getPreviousBoundingBox();

    //find what grids object occupied and remove it from those places
    sf::IntRect gridBounds = calculateGridBounds(boundingBox);

    remove(entry, gridBounds);
}

template<class Object> void SpatialHash<Object>::updateLocation(std::shared_ptr<HashEntry> &entry) {

    //check if objects previous position and current position coincide
    //if they do, then it hasn't moved so no need to move it
    sf::FloatRect previousBoundingBox = entry->getPreviousBoundingBox();
    entry->updateBoundingBox();
    sf::FloatRect currentBoundingBox = entry->getPreviousBoundingBox();

    //find what grids object occupied and remove it from those places
    sf::IntRect previousGridBounds = calculateGridBounds(previousBoundingBox);
    sf::IntRect currentGridBounds = calculateGridBounds(currentBoundingBox);

    //no change
    if(previousGridBounds == currentGridBounds) {

        return;
    }

    //changed so remove object from previous position
    //use the stored value of previousGridBounds because entry has been updated so calling getPreviousBoundingBox will return current box
    remove(entry, previousGridBounds);
    insert(entry, currentGridBounds);
}

template<class Object> void SpatialHash<Object>::insert(std::shared_ptr<HashEntry> &entry, const sf::IntRect &gridBounds) {

    for(int y = gridBounds.top; y <= gridBounds.top + gridBounds.height; ++y) {

        for(int x = gridBounds.left; x <= gridBounds.left + gridBounds.width; ++x) {

            glm::ivec2 gridPosition(x, y);
            EntryContainer &currentContainer = hash[gridPosition];

            currentContainer.push_back(entry);
        }
    }
}


template<class Object> void SpatialHash<Object>::remove(std::shared_ptr<HashEntry> &entry, const sf::IntRect &gridBounds) {

    for(int y = gridBounds.top; y <= gridBounds.top + gridBounds.height; ++y) {

        for(int x = gridBounds.left; x <= gridBounds.left + gridBounds.width; ++x) {

            glm::ivec2 gridPosition(x, y);
            EntryContainer &currentContainer = hash[gridPosition];

            //find entity in the container and remove
            typename EntryContainer::iterator it = remove_if(currentContainer.begin(), currentContainer.end(),
                                                [&](std::shared_ptr<HashEntry> val)->bool{return

                                                    val->getId() == entry->getId();
                                                } );

            currentContainer.erase(it, currentContainer.end());
        }
    }
}

template<class Object> sf::IntRect SpatialHash<Object>::calculateGridBounds(const sf::FloatRect &entryBoundingBox) {

    sf::IntRect gridBounds;

    gridBounds.left = entryBoundingBox.left / cellWidth;
    gridBounds.top = entryBoundingBox.top / cellHeight;
    gridBounds.width = glm::ceil(entryBoundingBox.width / static_cast<float>(cellWidth));
    gridBounds.height = glm::ceil(entryBoundingBox.height / static_cast<float>(cellHeight));

    return gridBounds;
}


#endif // SPATIALHASH_H_INCLUDED
