#include "ObjectCollisionbox.h"
#include <iostream>

using std::cout;
using std::endl;

ObjectCollisionbox::ObjectCollisionbox() :
    origin(0, 0),
    collisionboxes(),
    currentState(DEFAULT_STATE),
    idActiveCollisionbox(0)
    {

    }

void ObjectCollisionbox::insertCollisionbox(const sf::FloatRect &collisionboxObjectSpace, const unsigned &state) {

    collisionboxes[state].push_back(collisionboxObjectSpace);
}

void ObjectCollisionbox::removeCollisionbox(const unsigned &id, const unsigned &state) {

    if(checkIdValid(id, state)) {

        collisionboxes[state].erase(collisionboxes[state].begin() + id);
    }
}

void ObjectCollisionbox::clearCollisionboxes() {

    collisionboxes.clear();
    currentState = 0;
    idActiveCollisionbox = 0;
}

void ObjectCollisionbox::scale(const float &xFactor, const float &yFactor) {

    for(auto &iter : collisionboxes) {

        for(auto collisionboxIter = iter.second.begin(); collisionboxIter != iter.second.end(); ++collisionboxIter) {

            collisionboxIter->left *= xFactor;
            collisionboxIter->top *= yFactor;
            collisionboxIter->width *= xFactor;
            collisionboxIter->height *= yFactor;
        }
    }
}

void ObjectCollisionbox::move(const glm::vec2 &displacement) {

    origin += displacement;
}

void ObjectCollisionbox::setOrigin(const glm::vec2 &position) {

    origin = position;
}

void ObjectCollisionbox::setActiveCollisionbox(const unsigned &id, const unsigned &state) {

    if(checkIdValid(id, state)) {

        currentState = state;
        idActiveCollisionbox = id;
    }
}

glm::vec2 ObjectCollisionbox::getOrigin() const {

    return origin;
}

unsigned ObjectCollisionbox::getCollisionboxCount(const unsigned &state) const {

    return collisionboxes[state].size();
}

sf::FloatRect ObjectCollisionbox::getActiveCollisionboxWorldSpace() const {

    return getCollisionboxWorldSpace(idActiveCollisionbox, currentState);
}

sf::FloatRect ObjectCollisionbox::getCollisionboxWorldSpace(const unsigned &id, const unsigned &state) const {

    sf::FloatRect collisionbox = getCollisionboxObjectSpace(id, state);

    //check if collisionbox is valid
    if(collisionbox.width != 0) {

        collisionbox.left += origin.x;
        collisionbox.top += origin.y;
    }

    return collisionbox;
}

sf::FloatRect ObjectCollisionbox::getTotalCollisionboxWorldSpace(const unsigned &state) const {

    //find the minimum and maximum extents out of all the collisionboxes and return the enclosed area
    if(collisionboxes.count(state) == 0 || collisionboxes[state].size() == 0) {

        return sf::FloatRect(0, 0, 0, 0);
    }

    glm::vec2 topLeft(collisionboxes[state][0].left, collisionboxes[state][0].top);
    glm::vec2 bottomRight(collisionboxes[state][0].left + collisionboxes[state][0].width, collisionboxes[state][0].top + collisionboxes[state][0].height);

    for(unsigned i = 1; i < collisionboxes[state].size(); ++i) {

        topLeft.x = glm::min(topLeft.x, collisionboxes[state][i].left);
        topLeft.y = glm::min(topLeft.y, collisionboxes[state][i].top);
        bottomRight.x = glm::max(bottomRight.x, collisionboxes[state][i].left + collisionboxes[state][i].width);
        bottomRight.y = glm::max(bottomRight.y, collisionboxes[state][i].top + collisionboxes[state][i].height);
    }

    sf::FloatRect totalBounds(topLeft.x + origin.x, topLeft.y + origin.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
    return totalBounds;
}

sf::FloatRect ObjectCollisionbox::getTotalCollisionboxWorldSpace() const {

    return getTotalCollisionboxWorldSpace(currentState);
}

sf::FloatRect ObjectCollisionbox::getActiveCollisionboxObjectSpace() const {

    return getCollisionboxObjectSpace(idActiveCollisionbox, currentState);
}

sf::FloatRect ObjectCollisionbox::getCollisionboxObjectSpace(const unsigned &id, const unsigned &state) const {

    if(checkIdValid(id, state)) {

        return collisionboxes[state][id];
    }

    return sf::FloatRect(0, 0, 0, 0);
}

bool ObjectCollisionbox::checkIdValid(const unsigned &id, const unsigned &state) const {


    return (collisionboxes.count(state) != 0) && id >= 0 && id < collisionboxes[state].size();
}
