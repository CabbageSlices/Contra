#include "Bullet.h"
#include "TileCollisionHandling.h"

#include <vector>
#include <memory>
#include <iostream>

using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;

Bullet::Bullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace, const float &bulletVel) :
    xVelocity(bulletVel),
    lifeTime(sf::seconds(5)),
    timeElapsed(0),
    bullet(sf::Vector2f(20, 20)),
    positionController(glm::vec2(20, 20), glm::vec2(0, 0), glm::vec2(1, 1) * xVelocity, directionWorldSpace, glm::vec2(-directionWorldSpace.y, directionWorldSpace.x))
    {
        bullet.setSize(sf::Vector2f(positionController.getObjectSpace().getSizeWorldSpace().x, positionController.getObjectSpace().getSizeWorldSpace().y));
        positionController.setPositionWorldSpace(positionWorldSpace);
        positionController.setVelocities(xVelocity, 0);
    }

void Bullet::update(const float &delta, const sf::FloatRect &worldBounds, TileMap& map) {

    timeElapsed += delta;

    float x = xVelocity * timeElapsed;
    positionController.setVelocities(glm::cos(timeElapsed * 10) * xVelocity + timeElapsed * xVelocity, glm::sin(timeElapsed * 10) * xVelocity);

    positionController.move(delta, worldBounds);
    handleTileCollision(map);

    glm::vec2 position = positionController.getObjectSpace().getPositionWorldSpace();

    bullet.setPosition(position.x, position.y);
}

void Bullet::killBullet() {

    timeElapsed = lifeTime.asSeconds() * 2;
}

bool Bullet::checkIsAlive() {

    return timeElapsed < lifeTime.asSeconds();
}

void Bullet::draw(sf::RenderWindow &window) {

    window.draw(bullet);

    sf::FloatRect rect = positionController.getObjectSpace().getBoundingBoxWorldSpace();

    cout << rect.left << "  " << rect.top << "  " << rect.width << "  " << rect.height << endl;

    sf::RectangleShape debug(sf::Vector2f(rect.width, rect.height));
    debug.setPosition(rect.left, rect.top);
    debug.setFillColor(sf::Color::Magenta);
    window.draw(debug);
}

void Bullet::handleTileCollision(TileMap& map) {

    sf::FloatRect bounding = positionController.getObjectSpace().getBoundingBoxWorldSpace();

    //calculate region encompassed by object
    //extedn the region slightly because slope tiles need extra information about object previous position if he leaves a tile
    glm::vec2 regionTopLeft = glm::vec2(bounding.left, bounding.top) - glm::vec2(TILE_SIZE, TILE_SIZE);
    glm::vec2 regionBottomRight = glm::vec2(bounding.left + bounding.width, bounding.top + bounding.height) + glm::vec2(TILE_SIZE, TILE_SIZE);

    vector<shared_ptr<Tile> > tiles = map.getTilesInRegion(regionTopLeft, regionBottomRight);

    for(unsigned i = 0; i < tiles.size(); ++i) {

        if(checkSolidTileIntersection(tiles[i], positionController)) {

            killBullet();
        }
    }
}
