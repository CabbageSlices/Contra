#include "Gun.h"
#include "Bullet.h"
#include <iostream>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;
using std::endl;

Gun::Gun(const sf::Time &gunfireDelay) :
    timeSinceLastFired(sf::seconds(0)),
    fireDelay(gunfireDelay),
    bullets()
    {

    }

void Gun::fire(const glm::vec2 &userPosition, const glm::vec2 &bulletOriginUserSpace, const Direction &fireDirection) {

    if(!checkCanFire()) {

        return;
    }

    glm::vec2 directionVector = getDirectionVector(fireDirection);
    directionVector = glm::normalize(directionVector);

    glm::vec2 bulletOrigin = userPosition + bulletOriginUserSpace;

    createBullet(bulletOrigin, directionVector);

    timeSinceLastFired = sf::seconds(0);
}

void Gun::updatePhysics(const float &delta, const sf::FloatRect &worldBounds, TileMap& map) {

    timeSinceLastFired += sf::seconds(delta);

    for(unsigned i = 0; i < bullets.size();) {

        bullets[i]->updatePhysics(delta, worldBounds, map);

        if(!bullets[i]->checkIsAlive()) {

            bullets.erase(bullets.begin() + i);
            continue;
        }

        ++i;
    }
}

void Gun::updateRendering() {

    for(unsigned i = 0; i < bullets.size(); i++) {

        bullets[i]->updateRendering();
    }
}

void Gun::draw(sf::RenderWindow& window) {

    for(unsigned i = 0; i < bullets.size(); ++i) {

        bullets[i]->draw(window);
    }
}

vector<shared_ptr<Bullet> > &Gun::getBullets() {

    return bullets;
}

void Gun::setFireDelay(const sf::Time &delay) {

    fireDelay = delay;
}

void Gun::createBullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace) {

    shared_ptr<Bullet> bullet = make_shared<Bullet>(positionWorldSpace, directionWorldSpace);

    if(bullet) {

       bullets.push_back(bullet);
    }
}
