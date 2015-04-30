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

    return getHitBoxWorldSpace(idActiveHitbox);
}

sf::FloatRect ObjectHitbox::getHitBoxWorldSpace(const unsigned &id) const {

    sf::FloatRect hitbox = getHitBoxObjectSpace(id);

    //check if hitbox is valid
    if(hitbox.width != 0) {

        hitbox.left += origin.x;
        hitbox.top += origin.y;
    }

    return hitbox;
}

sf::FloatRect ObjectHitbox::getActiveHitboxObjectSpace() const {

    return getHitBoxObjectSpace(idActiveHitbox);
}

sf::FloatRect ObjectHitbox::getHitBoxObjectSpace(const unsigned &id) const {

    if(checkIdValid(id)) {

        return hitboxes[id];
    }

    return sf::FloatRect(0, 0, 0, 0);
}

bool ObjectHitbox::checkIdValid(const unsigned &id) const {

    return id >= 0 && id < hitboxes.size();
}
