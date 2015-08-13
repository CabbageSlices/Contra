#include "EntityBase.h"

using std::vector;
using std::shared_ptr;

EntityBase::EntityBase(const glm::vec2 &gravity, const glm::vec2 &movementVelocity, const glm::vec2 &terminalVelocity, const unsigned &initialHealth) :
    Collisionbox(),
    CollisionboxMovementController(gravity, terminalVelocity, &Collisionbox),
    MOVEMENT_VELOCITY(movementVelocity),
    health(initialHealth),
    entity(sf::Vector2f(64, 64)),
    currentState(-1),
    sprite(sf::milliseconds(75))
    {

    }

void EntityBase::draw(sf::RenderWindow &window) {

    window.draw(entity);
}

bool EntityBase::checkCanGetHit() {

    return checkIsAlive();
}

bool EntityBase::checkIsAlive() {

    return health > 0;
}

void EntityBase::getHit(int damage) {

    health -= damage;
}

ObjectCollisionbox &EntityBase::getCollisionbox() {

    return Collisionbox;
}

CollisionboxMovementController& EntityBase::getMovementController() {

    return CollisionboxMovementController;
}

const glm::vec2 EntityBase::getPosition() const {

    return Collisionbox.getOrigin();
}

void EntityBase::setHealth(const int &newVal) {

    health = newVal;

    if(newVal < 0) {

        health = 0;
    }
}

vector<shared_ptr<Tile> > EntityBase::getSurroundingTiles(const TileMap &map, const glm::vec2 &areaPadding) {

    sf::FloatRect bounding = Collisionbox.getActiveCollisionboxWorldSpace();

    //calculate region encompassed by object
    //extedn the region slightly because slope tiles need extra information about object previous position if he leaves a tile
    glm::vec2 regionTopLeft = glm::vec2(bounding.left, bounding.top) - areaPadding;
    glm::vec2 regionBottomRight = glm::vec2(bounding.left + bounding.width, bounding.top + bounding.height) + areaPadding;

    vector<shared_ptr<Tile> > tiles = map.getTilesInRegion(regionTopLeft, regionBottomRight);

    return tiles;
}

CollisionResponse EntityBase::handleTileCollision(TileMap &map, CollisionResponse(*collisionFunction)(shared_ptr<Tile>& tile, CollisionboxMovementController& object)) {

    //empty default
    return CollisionResponse();
}

CollisionResponse EntityBase::handleTileCollisionHorizontally(TileMap &map) {

    return handleTileCollision(map, &handleCollisionHorizontal);
}

CollisionResponse EntityBase::handleTileCollisionVertically(TileMap &map) {

    return handleTileCollision(map, &handleCollisionVertical);
}

void EntityBase::setState(const unsigned &state) {

    if(currentState == state) {

        return;
    }

    currentState = state;
    sprite.setAnimationState(state);
    Collisionbox.setActiveCollisionbox(0, state);
}

void EntityBase::setPosition(const glm::vec2 &position) {

    Collisionbox.setOrigin(position);
    sprite.getSprite().setPosition(position.x, position.y);
}
