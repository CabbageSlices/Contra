#include "Player.h"
#include "TileCollisionHandling.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Tile.h"

using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;

Player::Player(const PlayerKeys& keyConfiguration):
    MOVEMENT_VELOCITY(4.f, 3.9f),
    canJump(false),
    holdingJump(false),
    extraJumpTimer(),
    extraJumpDuration(sf::milliseconds(220)),
    positionController(glm::vec2(64, 64), glm::vec2(0, GRAVITY), glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY), glm::vec2(0, -1), glm::vec2(1, 0)),
    player(sf::Vector2f(100, 50)),
    controls(keyConfiguration)
    {
        player.setSize(sf::Vector2f(positionController.getBoundingBoxWorldSpace().width, positionController.getBoundingBoxWorldSpace().height));
    }

void Player::handleInputEvents(sf::Event& event, sf::RenderWindow& window) {

}

void Player::handleKeystate(sf::RenderWindow& window) {

    if(sf::Keyboard::isKeyPressed(controls.left)) {

        positionController.setVelocities(-MOVEMENT_VELOCITY.x, positionController.getVelocitiesObjectSpace().y);

    } else if(sf::Keyboard::isKeyPressed(controls.right)) {

        positionController.setVelocities(MOVEMENT_VELOCITY.x, positionController.getVelocitiesObjectSpace().y);

    } else {

        positionController.setVelocities(0, positionController.getVelocitiesObjectSpace().y);
    }

    if(sf::Keyboard::isKeyPressed(controls.jump)) {

        jump();
    }

    holdingJump = sf::Keyboard::isKeyPressed(controls.jump);
}

void Player::update(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map) {

    //since player velocity only changes in the y direction you can prevent gravity from pulling player down
    //when player is holding jump button and trying ot extend his jump height dont let gravity pull player down
    if(!checkExtendJump()) {

        positionController.updateVelocities(deltaTime);
    }

    positionController.moveAlongXAxis(deltaTime, worldBounds);

    handleTileCollisionHorizontally(map);

    if(positionController.moveAlongYAxis(deltaTime, worldBounds)) {

        canJump = true;
        positionController.setVelocities(positionController.getVelocitiesObjectSpace().x, 0);
    }

    handleTileCollisionVertically(map);

    player.setPosition(positionController.getBoundingBoxWorldSpace().left, positionController.getBoundingBoxWorldSpace().top);
}

void Player::draw(sf::RenderWindow& window) {

    window.draw(player);
}

void Player::handleTileCollision(TileMap& map, bool(*collisionFunction)(std::shared_ptr<Tile>& tile, PositionObject& object)) {

    //calculate region encompassed by object
    //extedn the region slightly because slope tiles need extra information about object previous position if he leaves a tile
    glm::vec2 regionTopLeft = positionController.getObjectSpace().getPositionWorldSpace() - glm::vec2(TILE_SIZE, TILE_SIZE);
    glm::vec2 regionBottomRight = positionController.getObjectSpace().getPositionWorldSpace() + positionController.getObjectSpace().getSizeWorldSpace() + glm::vec2(TILE_SIZE, TILE_SIZE);

    vector<shared_ptr<Tile> > tiles = map.getTilesInRegion(regionTopLeft, regionBottomRight);

    for(unsigned i = 0; i < tiles.size(); ++i) {

        if(collisionFunction(tiles[i], positionController)) {

            canJump = true;
        }
    }
}

void Player::handleTileCollisionHorizontally(TileMap& map) {

    handleTileCollision(map, &handleCollisionHorizontal);
}

void Player::handleTileCollisionVertically(TileMap& map) {

    handleTileCollision(map, &handleCollisionVertical);
}

void Player::jump() {

    if(checkCanJump()) {

        extraJumpTimer.restart();
        positionController.setVelocities(positionController.getVelocitiesObjectSpace().x, -MOVEMENT_VELOCITY.y);
        canJump = false;
    }
}
