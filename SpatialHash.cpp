#include "SpatialHash.h"
#include <algorithm>
#include <iterator>

using std::forward_iterator_tag;
using std::remove_if;
using std::shared_ptr;
using std::pair;
using std::make_pair;

SpatialHash::SpatialHash(const unsigned &hashCellWidth, const unsigned &hashCellHeight) :
    hash(),
    cellWidth(hashCellWidth),
    cellHeight(hashCellHeight)
    {

    }

void SpatialHash::insert(shared_ptr<SpatialHashEntry> &entry) {

    //make sure the object is using its most up to date position
    entry->updateBoundingBox();

    //get current bounding box to determine which cells the entry occupies
    sf::FloatRect boundingBox = entry->getPreviousBoundingBox();

    sf::IntRect gridBounds = calculateGridBounds(boundingBox);

    insert(entry, gridBounds);
}

void SpatialHash::remove(shared_ptr<SpatialHashEntry> &entry) {

    //don't use the latest hitbox since objects position in the hash is determined by his previous hitbox, not current one
    sf::FloatRect boundingBox = entry->getPreviousBoundingBox();

    //find what grids object occupied and remove it from those places
    sf::IntRect gridBounds = calculateGridBounds(boundingBox);

    remove(entry, gridBounds);
}

void SpatialHash::updateLocation(std::shared_ptr<SpatialHashEntry> &entry) {

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

void SpatialHash::insert(shared_ptr<SpatialHashEntry> &entry, const sf::IntRect &gridBounds) {

    for(int y = gridBounds.top; y <= gridBounds.top + gridBounds.height; ++y) {

        for(int x = gridBounds.left; x <= gridBounds.left + gridBounds.width; ++x) {

            glm::ivec2 gridPosition(x, y);
            EntryContainer &currentContainer = hash[gridPosition];

            currentContainer.push_back(entry);
        }
    }
}


void SpatialHash::remove(shared_ptr<SpatialHashEntry> &entry, const sf::IntRect &gridBounds) {

    for(int y = gridBounds.top; y <= gridBounds.top + gridBounds.height; ++y) {

        for(int x = gridBounds.left; x <= gridBounds.left + gridBounds.width; ++x) {

            glm::ivec2 gridPosition(x, y);
            EntryContainer &currentContainer = hash[gridPosition];

            //find entity in the container and remove
            EntryContainer::iterator it = remove_if(currentContainer.begin(), currentContainer.end(),
                                                [&](shared_ptr<SpatialHashEntry> val)->bool{return

                                                    val->getId() == entry->getId();
                                                } );

            currentContainer.erase(it, currentContainer.end());
        }
    }
}

sf::IntRect SpatialHash::calculateGridBounds(const sf::FloatRect &entryBoundingBox) {

    sf::IntRect gridBounds;

    gridBounds.left = entryBoundingBox.left / cellWidth;
    gridBounds.top = entryBoundingBox.top / cellHeight;
    gridBounds.width = glm::ceil(entryBoundingBox.width / static_cast<float>(cellWidth));
    gridBounds.height = glm::ceil(entryBoundingBox.height / static_cast<float>(cellHeight));

    return gridBounds;
}
