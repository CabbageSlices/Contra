#ifndef SPATIALHASH_H_INCLUDED
#define SPATIALHASH_H_INCLUDED

#include "SpatialHashEntry.h"
#include "glm/glm.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <boost/functional/hash.hpp>
#include <unordered_set>
#include "Enemy.h"


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

template<class T>
class SpatialHash {

    public:

        SpatialHash(const unsigned &hashCellWidth, const unsigned &hashCellHeight);

        //if its known that an objects arealdy been inserted into the hash its better to call updatePosition
        //instead of using insert
        //insert assumes object isn't inside the grid
        void insert(std::shared_ptr<T> &entry);
        void remove(std::shared_ptr<T> &entry);
        void remove(std::shared_ptr<T> &entry, const sf::FloatRect &boundingBox); //remove the entity from the grid given its bounding box

        std::unordered_set<std::shared_ptr<T> > getSurroundingEntites(const sf::FloatRect boundingBox);

        bool checkShouldUpdateHashLocation(const sf::FloatRect &previousBoundingBox, const sf::FloatRect &currentBoundingBox) const;

    private:

        //insert the entry from its given bounding box
        void insert(std::shared_ptr<T> &entry, const sf::IntRect &enclosedGrid);

        //remove an entry from the given bounding box
        void remove(std::shared_ptr<T> &entry, const sf::IntRect &enclosedGrid);

        //returns the extens of the grid enclosed by the object
        //width and height refer to the right edge of the grid, and not the actual widht and height
        //i.e width and height refer to left + width, top + height respectively
        sf::IntRect calculateEnclosedGrid(const sf::FloatRect &entryBoundingBox) const;

        std::unordered_multimap<glm::ivec2, std::shared_ptr<T>, Hasher> hash;

        unsigned cellWidth;
        unsigned cellHeight;
};

template<class T> SpatialHash<T>::SpatialHash(const unsigned &hashCellWidth, const unsigned &hashCellHeight) :
    hash(),
    cellWidth(hashCellWidth),
    cellHeight(hashCellHeight)
    {

    }

template<class T> void SpatialHash<T>::insert(std::shared_ptr<T> &entry) {

    //get current bounding box to determine which cells the entry occupies
    sf::FloatRect boundingBox = entry->getCollisionbox().getActiveCollisionboxWorldSpace();

    sf::IntRect gridBounds = calculateEnclosedGrid(boundingBox);

    insert(entry, gridBounds);
}

template<class T> void SpatialHash<T>::remove(std::shared_ptr<T> &entry) {

    //don't use the latest collisionbox since objects position in the hash is determined by his previous collisionbox, not current one
    sf::FloatRect boundingBox = entry->getCollisionbox().getActiveCollisionboxWorldSpace();

    //find what grids object occupied and remove it from those places
    sf::IntRect gridBounds = calculateEnclosedGrid(boundingBox);

    remove(entry, gridBounds);
}

template<class T> void SpatialHash<T>::remove(std::shared_ptr<T> &entry, const sf::FloatRect &boundingBox) {

    //find what grids object occupied and remove it from those places
    sf::IntRect gridBounds = calculateEnclosedGrid(boundingBox);

    remove(entry, gridBounds);
}

template<class T> std::unordered_set<std::shared_ptr<T> > SpatialHash<T>::getSurroundingEntites(const sf::FloatRect boundingBox) {

    std::unordered_set<std::shared_ptr<T> > container;

    sf::IntRect enclosedGrid = calculateEnclosedGrid(boundingBox);

    for(int y = enclosedGrid.top; y <= enclosedGrid.height; ++y) {

        for(int x = enclosedGrid.left; x <= enclosedGrid.width; ++x) {

            glm::ivec2 gridPosition(x, y);
            auto objects = hash.equal_range(gridPosition);

            for(auto it = objects.first; it != objects.second; ++it) {

                if(container.find(it->second) == container.end())
                container.insert(it->second);
            }
        }
    }

    return container;
}

template<class T> bool SpatialHash<T>::checkShouldUpdateHashLocation(const sf::FloatRect &previousBoundingBox, const sf::FloatRect &currentBoundingBox) const {

    sf::IntRect previousGridBounds = calculateEnclosedGrid(previousBoundingBox);
    sf::IntRect currentGridBounds = calculateEnclosedGrid(currentBoundingBox);

    return (previousGridBounds != currentGridBounds);
}

template<class T> void SpatialHash<T>::insert(std::shared_ptr<T> &entry, const sf::IntRect &enclosedGrid) {

    for(int y = enclosedGrid.top; y <= enclosedGrid.height; ++y) {

        for(int x = enclosedGrid.left; x <= enclosedGrid.width; ++x) {

            auto hashedEntry = std::make_pair(glm::ivec2(x, y), entry);
            hash.insert(hashedEntry);
        }
    }
}


template<class T> void SpatialHash<T>::remove(std::shared_ptr<T> &entry, const sf::IntRect &enclosedGrid) {

    for(int y = enclosedGrid.top; y <= enclosedGrid.height; ++y) {

        for(int x = enclosedGrid.left; x <= enclosedGrid.width; ++x) {

            glm::ivec2 gridPosition(x, y);

            auto removedObjects = hash.equal_range(gridPosition);

            for(auto it = removedObjects.first; it != removedObjects.second; ++it) {

                if(it->second == entry) {

                    hash.erase(it);
                    break;
                }
            }
        }
    }
}

template<class T> sf::IntRect SpatialHash<T>::calculateEnclosedGrid(const sf::FloatRect &entryBoundingBox) const {

    sf::IntRect enclosedGrid;

    enclosedGrid.left = entryBoundingBox.left / cellWidth;
    enclosedGrid.top = entryBoundingBox.top / cellHeight;
    enclosedGrid.width = ((entryBoundingBox.left + entryBoundingBox.width) / static_cast<float>(cellWidth));
    enclosedGrid.height = ((entryBoundingBox.top + entryBoundingBox.height) / static_cast<float>(cellHeight));

    return enclosedGrid;
}


#endif // SPATIALHASH_H_INCLUDED
