#include "Gun.h"
#include "Bullet.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;

Gun::Gun(const ObjectSpaceManager &userObjSpace, const glm::vec2 &relativePosition) :
    positionUserSpace(relativePosition),
    userObjectSpace(userObjSpace),
    bullets()
    {

    }

void Gun::fire(const glm::vec2 &userPositionWorldSpace, const Direction &fireDirection) {

    glm::vec2 directionVector = getDirectionVector(fireDirection);
    directionVector = glm::normalize(directionVector);

    //calculate position and direction of gun in world space
    glm::vec2 positionWorldSpace = userPositionWorldSpace + userObjectSpace.convertToWorldSpace(positionUserSpace);
    glm::vec2 directionWorldSpace = userObjectSpace.convertToWorldSpace(directionVector);

    createBullet(positionWorldSpace, directionWorldSpace);
}

void Gun::update(const float &delta, const sf::FloatRect &worldBounds) {

    for(unsigned i = 0; i < bullets.size();) {

        bullets[i]->update(delta, worldBounds);

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
