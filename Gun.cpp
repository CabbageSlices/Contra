#include "Gun.h"
#include "Bullet.h"
#include <iostream>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;
using std::endl;

Gun::Gun() :
    timeSinceLastFired(sf::seconds(0)),
    fireDelay(sf::seconds(0.3)),
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

void Gun::update(const float &delta, const sf::FloatRect &worldBounds, TileMap& map) {

    timeSinceLastFired += sf::seconds(delta);

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

vector<shared_ptr<Bullet> > &Gun::getBullets() {

    return bullets;
}

void Gun::createBullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace) {

    shared_ptr<Bullet> bullet = make_shared<Bullet>(positionWorldSpace, directionWorldSpace);

    if(bullet) {

       bullets.push_back(bullet);
    }
}
