#include "Gun.h"
#include "Bullet.h"
#include <iostream>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;
using std::endl;

Gun::Gun(const ObjectSpaceManager &userObjSpace, const glm::vec2 &relativePosition) :
    positionUserSpace(relativePosition),
    userObjectSpace(userObjSpace),
    bullets()
    {

    }

void Gun::fire(const glm::vec2 &userPositionWorldSpace, const Direction &fireDirection) {

    glm::vec2 directionVector = getDirectionVector(fireDirection);
    directionVector = glm::normalize(directionVector);

    glm::vec2 userSpacePos = userObjectSpace.convertToWorldSpace(positionUserSpace);


    //calculate position and direction of gun in world space
    glm::vec2 positionWorldSpace = userPositionWorldSpace;
    glm::vec2 directionWorldSpace = userObjectSpace.convertToWorldSpace(directionVector);

    //origin of bullet varies depending on direction the gun is being fired in
    //assume the bullet origin lies on the circurference of a circle formed at the center of the user where the radius is equal to half the user's largest size
    //then the direction to the position on this circle where the bullet fires from is the same as the direction the gun is firing in, aka the directionVector
    //requires the position of the center of the user in world space, which is obtained by adding half the width and height of the user to his position
    glm::vec2 objectSpaceDistanceToCenter = userObjectSpace.getSizeObjectSpace() / 2.f;
    glm::vec2 worldSpaceDistanceToCenter = userObjectSpace.convertToWorldSpace(objectSpaceDistanceToCenter);
    glm::vec2 userSizeWorldSpace = userObjectSpace.getSizeWorldSpace();

    positionWorldSpace +=  worldSpaceDistanceToCenter + directionWorldSpace * glm::max(userSizeWorldSpace.x, userSizeWorldSpace.y) / 2.f;

    createBullet(positionWorldSpace, directionWorldSpace);
}

void Gun::update(const float &delta, const sf::FloatRect &worldBounds, TileMap& map) {

    for(unsigned i = 0; i < bullets.size();) {

        bullets[i]->update(delta, worldBounds, map);

        if(!bullets[i]->checkIsAlive()) {

            bullets.erase(bullets.begin() + i);
            continue;
        }

        ++i;
    }
}

void Gun::draw(sf::RenderWindow& window) {

    for(unsigned i = 0; i < bullets.size(); ++i) {

        bullets[i]->draw(window);
    }
}

void Gun::createBullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace) {

    shared_ptr<Bullet> bullet = make_shared<Bullet>(positionWorldSpace, directionWorldSpace);

    if(bullet) {

       bullets.push_back(bullet);

    }
}
