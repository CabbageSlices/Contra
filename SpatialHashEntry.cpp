#include "SpatialHashEntry.h"

using std::shared_ptr;

unsigned SpatialHashEntry::hashEntriesCreated = 0;

SpatialHashEntry::SpatialHashEntry(shared_ptr<Object> &hashEntry) :
    object(hashEntry),
    previousBoundingBox(0, 0, 0, 0),
    id(hashEntriesCreated)
    {

        hashEntriesCreated += 1;
    }

void SpatialHashEntry::updateBoundingBox() {

    if(!checkObjectExists()) {

        return;
    }

    previousBoundingBox = object.lock()->getHitbox().getTotalHitboxWorldSpace();
}

bool SpatialHashEntry::checkObjectExists() {

    return bool(object.lock());
}

unsigned SpatialHashEntry::getId() const {

    return id;
}

sf::FloatRect SpatialHashEntry::getPreviousBoundingBox() const {

    return previousBoundingBox;
}
