#include "ObjectCollisionbox.h"
#include <iostream>

using std::cout;
using std::endl;

ObjectCollisionbox::ObjectCollisionbox() :
    origin(0, 0),
    Collisionboxes(),
    currentState(DEFAULT_STATE),
    idActiveCollisionbox(0)
    {

    }

void ObjectCollisionbox::insertCollisionbox(const sf::FloatRect &CollisionboxObjectSpace, const unsigned &state) {

    Collisionboxes[state].push_back(CollisionboxObjectSpace);
}

void ObjectCollisionbox::removeCollisionbox(const unsigned &id, const unsigned &state) {

    if(checkIdValid(id, state)) {

        Collisionboxes[state].erase(Collisionboxes[state].begin() + id);
    }
}

void ObjectCollisionbox::clearCollisionboxes() {

    Collisionboxes.clear();
    currentState = 0;
    idActiveCollisionbox = 0;
}

void ObjectCollisionbox::scale(const float &xFactor, const float &yFactor) {

    for(auto &iter : Collisionboxes) {

        for(auto CollisionboxIter = iter.second.begin(); CollisionboxIter != iter.second.end(); ++CollisionboxIter) {

            CollisionboxIter->left *= xFactor;
            CollisionboxIter->top *= yFactor;
            CollisionboxIter->width *= xFactor;
            CollisionboxIter->height *= yFactor;
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

    return Collisionboxes[state].size();
}

sf::FloatRect ObjectCollisionbox::getActiveCollisionboxWorldSpace() const {

    return getCollisionboxWorldSpace(idActiveCollisionbox, currentState);
}

sf::FloatRect ObjectCollisionbox::getCollisionboxWorldSpace(const unsigned &id, const unsigned &state) const {

    sf::FloatRect Collisionbox = getCollisionboxObjectSpace(id, state);

    //check if Collisionbox is valid
    if(Collisionbox.width != 0) {

        Collisionbox.left += origin.x;
        Collisionbox.top += origin.y;
    }

    return Collisionbox;
}

sf::FloatRect ObjectCollisionbox::getTotalCollisionboxWorldSpace(const unsigned &state) const {

    //find the minimum and maximum extents out of all the Collisionboxes and return the enclosed area
    if(Collisionboxes.count(state) == 0 || Collisionboxes[state].size() == 0) {

        return sf::FloatRect(0, 0, 0, 0);
    }

    glm::vec2 topLeft(Collisionboxes[state][0].left, Collisionboxes[state][0].top);
    glm::vec2 bottomRight(Collisionboxes[state][0].left + Collisionboxes[state][0].width, Collisionboxes[state][0].top + Collisionboxes[state][0].height);

    for(unsigned i = 1; i < Collisionboxes[state].size(); ++i) {

        topLeft.x = glm::min(topLeft.x, Collisionboxes[state][i].left);
        topLeft.y = glm::min(topLeft.y, Collisionboxes[state][i].top);
        bottomRight.x = glm::max(bottomRight.x, Collisionboxes[state][i].left + Collisionboxes[state][i].width);
        bottomRight.y = glm::max(bottomRight.y, Collisionboxes[state][i].top + Collisionboxes[state][i].height);
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

        return Collisionboxes[state][id];
    }

    return sf::FloatRect(0, 0, 0, 0);
}

bool ObjectCollisionbox::checkIdValid(const unsigned &id, const unsigned &state) const {


    return (Collisionboxes.count(state) != 0) && id >= 0 && id < Collisionboxes[state].size();
}
