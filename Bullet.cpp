#include "Bullet.h"
#include <iostream>

using std::cout;
using std::endl;

Bullet::Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, const float &bulletVel) :
    xVelocity(bulletVel),
    lifeTime(sf::seconds(5)),
    timeElapsed(0),
    bullet(sf::Vector2f(20, 20)),
    positionController(glm::vec2(20, 20), glm::vec2(0, 0), glm::vec2(1, 1) * xVelocity, directionWorldSpace, glm::vec2(-directionWorldSpace.y, directionWorldSpace.x))
    {
        positionController.setPositionWorldSpace(positionWorldSpace);
        positionController.setVelocities(xVelocity, 0);
    }

void Bullet::update(const float &delta, const sf::FloatRect &worldBounds) {

    timeElapsed += delta;

    float x = xVelocity * timeElapsed;
    positionController.setVelocities(glm::cos(timeElapsed * 10) * xVelocity + timeElapsed * xVelocity, glm::sin(timeElapsed * 10) * xVelocity);

    positionController.move(delta, worldBounds);

    glm::vec2 position = positionController.getObjectSpace().getPositionWorldSpace();

    bullet.setPosition(position.x, position.y);
}

bool Bullet::checkIsAlive() {

    return timeElapsed < lifeTime.asSeconds();
}

void Bullet::draw(sf::RenderWindow &window) {

    window.draw(bullet);
}
