#include "ObjectHitbox.h"
#include <iostream>

using std::cout;
using std::endl;

ObjectHitbox::ObjectHitbox() :
    origin(0, 0),
    hitboxes(),
    currentState(DEFAULT_STATE),
    idActiveHitbox(0)
    {

    }

void ObjectHitbox::insertHitbox(const sf::FloatRect &hitboxObjectSpace, const unsigned &state) {

    hitboxes[state].push_back(hitboxObjectSpace);
}

void ObjectHitbox::removeHitbox(const unsigned &id, const unsigned &state) {

    if(checkIdValid(id, state)) {

        hitboxes[state].erase(hitboxes[state].begin() + id);
    }
}

void ObjectHitbox::clearHitboxes() {

    hitboxes.clear();
    currentState = 0;
    idActiveHitbox = 0;
}

void ObjectHitbox::scale(const float &xFactor, const float &yFactor) {

    for(auto &iter : hitboxes) {

        for(auto hitboxIter = iter.second.begin(); hitboxIter != iter.second.end(); ++hitboxIter) {

            hitboxIter->left *= xFactor;
            hitboxIter->top *= yFactor;
            hitboxIter->width *= xFactor;
            hitboxIter->height *= yFactor;
        }
    }
}

void ObjectHitbox::move(const glm::vec2 &displacement) {

    origin += displacement;
}

void ObjectHitbox::setOrigin(const glm::vec2 &position) {

    origin = position;
}

void ObjectHitbox::setActiveHitbox(const unsigned &id, const unsigned &state) {

    if(checkIdValid(id, state)) {

        currentState = state;
        idActiveHitbox = id;
    }
}

glm::vec2 ObjectHitbox::getOrigin() const {

    return origin;
}

unsigned ObjectHitbox::getHitboxCount(const unsigned &state) const {

    return hitboxes[state].size();
}

sf::FloatRect ObjectHitbox::getActiveHitboxWorldSpace() const {

    return getHitboxWorldSpace(idActiveHitbox, currentState);
}

sf::FloatRect ObjectHitbox::getHitboxWorldSpace(const unsigned &id, const unsigned &state) const {

    sf::FloatRect hurtbox = getHitboxObjectSpace(id, state);

    //check if hitbox is valid
    if(hurtbox.width != 0) {

        hurtbox.left += origin.x;
        hurtbox.top += origin.y;
    }

    return hurtbox;
}

sf::FloatRect ObjectHitbox::getTotalHitboxWorldSpace(const unsigned &state) const {

    //find the minimum and maximum extents out of all the hitboxes and return the enclosed area
    if(hitboxes.count(state) == 0 || hitboxes[state].size() == 0) {

        return sf::FloatRect(0, 0, 0, 0);
    }

    glm::vec2 topLeft(hitboxes[state][0].left, hitboxes[state][0].top);
    glm::vec2 bottomRight(hitboxes[state][0].left + hitboxes[state][0].width, hitboxes[state][0].top + hitboxes[state][0].height);

    for(unsigned i = 1; i < hitboxes[state].size(); ++i) {

        topLeft.x = glm::min(topLeft.x, hitboxes[state][i].left);
        topLeft.y = glm::min(topLeft.y, hitboxes[state][i].top);
        bottomRight.x = glm::max(bottomRight.x, hitboxes[state][i].left + hitboxes[state][i].width);
        bottomRight.y = glm::max(bottomRight.y, hitboxes[state][i].top + hitboxes[state][i].height);
    }

    sf::FloatRect totalBounds(topLeft.x + origin.x, topLeft.y + origin.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
    return totalBounds;
}

sf::FloatRect ObjectHitbox::getTotalHitboxWorldSpace() const {

    return getTotalHitboxWorldSpace(currentState);
}

sf::FloatRect ObjectHitbox::getActiveHitboxObjectSpace() const {

    return getHitboxObjectSpace(idActiveHitbox, currentState);
}

sf::FloatRect ObjectHitbox::getHitboxObjectSpace(const unsigned &id, const unsigned &state) const {

    if(checkIdValid(id, state)) {

        return hitboxes[state][id];
    }

    return sf::FloatRect(0, 0, 0, 0);
}

bool ObjectHitbox::checkIdValid(const unsigned &id, const unsigned &state) const {


    return (hitboxes.count(state) != 0) && id >= 0 && id < hitboxes[state].size();
}
