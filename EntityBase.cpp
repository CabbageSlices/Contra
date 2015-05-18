#include "EntityBase.h"

using std::vector;
using std::shared_ptr;

EntityBase::EntityBase(const glm::vec2 &gravity, const glm::vec2 &movementVelocity, const glm::vec2 &terminalVelocity, const unsigned &initialHealth) :
    hitbox(),
    hitboxMovementController(gravity, terminalVelocity, &hitbox),
    MOVEMENT_VELOCITY(movementVelocity),
    health(initialHealth),
    entity(sf::Vector2f(64, 64))
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

const ObjectHitbox &EntityBase::getHitbox() const {

    return hitbox;
}

const glm::vec2 EntityBase::getPosition() const {

    return hitbox.getOrigin();
}

vector<shared_ptr<Tile> > EntityBase::getSurroundingTiles(const TileMap &map, const glm::vec2 &areaPadding) {

    sf::FloatRect bounding = hitbox.getActiveHitboxWorldSpace();

    //calculate region encompassed by object
    //extedn the region slightly because slope tiles need extra information about object previous position if he leaves a tile
    glm::vec2 regionTopLeft = glm::vec2(bounding.left, bounding.top) - areaPadding;
    glm::vec2 regionBottomRight = glm::vec2(bounding.left + bounding.width, bounding.top + bounding.height) + areaPadding;

    vector<shared_ptr<Tile> > tiles = map.getTilesInRegion(regionTopLeft, regionBottomRight);

    return tiles;
}

CollisionResponse EntityBase::handleTileCollision(TileMap &map, CollisionResponse(*collisionFunction)(shared_ptr<Tile>& tile, HitboxMovementController& object)) {

    //empty default
    return CollisionResponse();
}

CollisionResponse EntityBase::handleTileCollisionHorizontally(TileMap &map) {

    return handleTileCollision(map, &handleCollisionHorizontal);
}

CollisionResponse EntityBase::handleTileCollisionVertically(TileMap &map) {

    return handleTileCollision(map, &handleCollisionVertical);
}
