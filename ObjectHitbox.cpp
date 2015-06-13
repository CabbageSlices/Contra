#include "ObjectHitbox.h"
#include <iostream>

using std::cout;
using std::endl;

ObjectHitbox::ObjectHitbox() :
    origin(0, 0),
    hitboxes(),
    idActiveHitbox(0)
    {

    }

void ObjectHitbox::insertHitbox(const sf::FloatRect &hitboxObjectSpace) {

    hitboxes.push_back(hitboxObjectSpace);
}

void ObjectHitbox::removeHitbox(const unsigned &id) {

    if(checkIdValid(id)) {

        hitboxes.erase(hitboxes.begin() + id);
    }
}

void ObjectHitbox::clearHitboxes() {

    hitboxes.clear();
    idActiveHitbox = 0;
}

void ObjectHitbox::move(const glm::vec2 &displacement) {

    origin += displacement;
}

void ObjectHitbox::setOrigin(const glm::vec2 &position) {

    origin = position;
}

void ObjectHitbox::setActiveHitbox(const unsigned &id) {

    if(checkIdValid(id)) {

        idActiveHitbox = id;
    }
}

glm::vec2 ObjectHitbox::getOrigin() const {

    return origin;
}

unsigned ObjectHitbox::getHitboxCount() const {

    return hitboxes.size();
}

sf::FloatRect ObjectHitbox::getActiveHitboxWorldSpace() const {

    return getHitboxWorldSpace(idActiveHitbox);
}

sf::FloatRect ObjectHitbox::getHitboxWorldSpace(const unsigned &id) const {

    sf::FloatRect hitbox = getHitboxObjectSpace(id);

    //check if hitbox is valid
    if(hitbox.width != 0) {

        hitbox.left += origin.x;
        hitbox.top += origin.y;
    }

    return hitbox;
}

sf::FloatRect ObjectHitbox::getTotalHitboxWorldSpace() const {

    //find the minimum and maximum extents out of all the hitboxes and return the enclosed area
    if(hitboxes.size() == 0) {

        return sf::FloatRect(0, 0, 0, 0);
    }

    glm::vec2 topLeft(hitboxes[0].left, hitboxes[0].top);
    glm::vec2 bottomRight(hitboxes[0].left + hitboxes[0].width, hitboxes[0].top + hitboxes[0].height);

    for(unsigned i = 1; i < hitboxes.size(); ++i) {

        topLeft.x = glm::min(topLeft.x, hitboxes[i].left);
        topLeft.y = glm::min(topLeft.y, hitboxes[i].top);
        bottomRight.x = glm::max(bottomRight.x, hitboxes[i].left + hitboxes[i].width);
        bottomRight.y = glm::max(bottomRight.y, hitboxes[i].top + hitboxes[i].height);
    }

    sf::FloatRect totalBounds(topLeft.x + origin.x, topLeft.y + origin.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
    return totalBounds;
}

sf::FloatRect ObjectHitbox::getActiveHitboxObjectSpace() const {

    return getHitboxObjectSpace(idActiveHitbox);
}

sf::FloatRect ObjectHitbox::getHitboxObjectSpace(const unsigned &id) const {

    if(checkIdValid(id)) {

        return hitboxes[id];
    }

    return sf::FloatRect(0, 0, 0, 0);
}

bool ObjectHitbox::checkIdValid(const unsigned &id) const {

    return id >= 0 && id < hitboxes.size();
}
