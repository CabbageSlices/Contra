#include "Player.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Gun.h"
#include "Tile.h"

using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;

Player::Player(const PlayerKeys& keyConfiguration):
    MOVEMENT_VELOCITY(4.f, 3.9f),
    standingOnSolid(false),
    standingOnTile(false),
    holdingJump(false),
    extraJumpTimer(),
    extraJumpDuration(sf::milliseconds(220)),
    hitbox(),
    hitboxMovementController(glm::vec2(0, GRAVITY), glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY), &hitbox),
    direction(),
    player(sf::Vector2f(50, 100)),
    gun(),
    controls(keyConfiguration)
    {
        gun = std::make_shared<Gun>();

        if(!gun) {

            exit(-1);
        }

        hitbox.insertHitbox(sf::FloatRect(0, 0, 50, 100));
        hitbox.insertHitbox(sf::FloatRect(-25, 50, 100, 50));
        hitbox.setActiveHitbox(0);
    }

void Player::handleInputEvents(sf::Event& event, sf::RenderWindow& window) {

}

void Player::handleKeystate(sf::RenderWindow& window) {

    if(sf::Keyboard::isKeyPressed(controls.left)) {

        hitboxMovementController.setVelocities(-MOVEMENT_VELOCITY.x, hitboxMovementController.getVelocities().y);

    } else if(sf::Keyboard::isKeyPressed(controls.right)) {

        hitboxMovementController.setVelocities(MOVEMENT_VELOCITY.x, hitboxMovementController.getVelocities().y);

    } else {

        hitboxMovementController.setVelocities(0, hitboxMovementController.getVelocities().y);
    }

    if(sf::Keyboard::isKeyPressed(controls.jump)) {

        jump();
    }

    if(sf::Keyboard::isKeyPressed(controls.fire)) {

        gun->fire(hitbox.getOrigin(), calculateGunfireOrigin(), direction);
    }

    holdingJump = sf::Keyboard::isKeyPressed(controls.jump);

    determineDirection();
}

void Player::update(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map) {

    //since player velocity only changes in the y direction you can prevent gravity from pulling player down
    //when player is holding jump button and trying ot extend his jump height dont let gravity pull player down
    if(!checkExtendJump()) {

        hitboxMovementController.updateVelocities(deltaTime);
    }

    hitboxMovementController.moveAlongXAxis(deltaTime, worldBounds);

    CollisionResponse collisionResponseHorizontal = handleTileCollisionHorizontally(map);

    if(hitboxMovementController.moveAlongYAxis(deltaTime, worldBounds)) {

        standingOnSolid = true;
        hitboxMovementController.setVelocities(hitboxMovementController.getVelocities().x, 0);

    } else {

        standingOnSolid = false;
    }

    CollisionResponse collisionResponseVertical = handleTileCollisionVertically(map);

    standingOnTile = (collisionResponseHorizontal.handledVertical || collisionResponseVertical.handledVertical);

    sf::FloatRect activeHitbox = hitbox.getActiveHitboxWorldSpace();
    player.setPosition(activeHitbox.left, activeHitbox.top);
    player.setSize(sf::Vector2f(activeHitbox.width, activeHitbox.height));

    gun->update(deltaTime, worldBounds, map);
}

void Player::draw(sf::RenderWindow& window) {

    gun->draw(window);
    window.draw(player);
}

const glm::vec2 Player::getPosition() const {

    return hitbox.getOrigin();
}

glm::vec2 Player::calculateGunfireOrigin() const {

    //default facing right so its at the right side of the player
    glm::vec2 gunPosition(player.getGlobalBounds().width, player.getGlobalBounds().height / 2);

    if(direction.isFacingCompletelyVertical && !checkIsCrouching()) {

        gunPosition.x = player.getGlobalBounds().width / 2;

        if(direction.vertical == VerticalDirection::UP) {

            gunPosition.y = 0;

        } else {

            gunPosition.y = player.getGlobalBounds().height;
        }

        return gunPosition;
    }

    if(direction.horizontal == HorizontalDirection::RIGHT) {

        gunPosition.x = player.getGlobalBounds().width;

    } else {

        gunPosition.x = 0;
    }

    if(direction.vertical == VerticalDirection::UP) {

        gunPosition.y = 0;

    } else if(direction.vertical == VerticalDirection::DOWN) {

        gunPosition.y = player.getGlobalBounds().height;

    } else {

        gunPosition.y = player.getGlobalBounds().height / 2;
    }

    if(checkIsCrouching()) {

        gunPosition.y = player.getGlobalBounds().height;
    }

    return gunPosition;
}

CollisionResponse Player::handleTileCollision(TileMap& map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object)) {

    sf::FloatRect bounding = hitbox.getActiveHitboxWorldSpace();

    //calculate region encompassed by object
    //extedn the region slightly because slope tiles need extra information about object previous position if he leaves a tile
    glm::vec2 regionTopLeft = glm::vec2(bounding.left, bounding.top) - glm::vec2(TILE_SIZE, TILE_SIZE);
    glm::vec2 regionBottomRight = glm::vec2(bounding.left + bounding.width, bounding.top + bounding.height) + glm::vec2(TILE_SIZE, TILE_SIZE);

    vector<shared_ptr<Tile> > tiles = map.getTilesInRegion(regionTopLeft, regionBottomRight);

    CollisionResponse collisionResponse;

    for(unsigned i = 0; i < tiles.size(); ++i) {

        CollisionResponse response = collisionFunction(tiles[i], hitboxMovementController);

        if(response.handledVertical) {

            collisionResponse.handledVertical = true;
        }

        if(response.handledHorizontal) {

            collisionResponse.handledHorizontal = true;
        }
    }

    return collisionResponse;
}

CollisionResponse Player::handleTileCollisionHorizontally(TileMap& map) {

    return handleTileCollision(map, &handleCollisionHorizontal);
}

CollisionResponse Player::handleTileCollisionVertically(TileMap& map) {

    return handleTileCollision(map, &handleCollisionVertical);
}

void Player::determineDirection() {

    if(hitboxMovementController.getVelocities().x < 0) {

        direction.horizontal = HorizontalDirection::LEFT;

    } else if(hitboxMovementController.getVelocities().x > 0) {

        direction.horizontal = HorizontalDirection::RIGHT;
    }

    if(sf::Keyboard::isKeyPressed(controls.up)) {

        direction.vertical = VerticalDirection::UP;

    } else if(sf::Keyboard::isKeyPressed(controls.down)) {

        direction.vertical = VerticalDirection::DOWN;

    } else {

        direction.vertical = VerticalDirection::STRAIGHT;
    }

    if(checkIsCrouching()) {

        direction.vertical = VerticalDirection::STRAIGHT;
        hitbox.setActiveHitbox(1);

    } else {

        hitbox.setActiveHitbox(0);
    }

    direction.isFacingCompletelyVertical = (hitboxMovementController.getVelocities().x == 0 && direction.vertical != VerticalDirection::STRAIGHT);
}

void Player::jump() {

    if(checkCanJump()) {

        extraJumpTimer.restart();
        hitboxMovementController.setVelocities(hitboxMovementController.getVelocities().x, -MOVEMENT_VELOCITY.y);
        standingOnSolid = false;
    }
}
