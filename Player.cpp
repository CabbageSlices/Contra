#include "Player.h"
#include "TileCollisionHandling.h"
#include <iostream>

using std::cout;
using std::endl;

Player::Player(const PlayerKeys& keyConfiguration):
    MOVEMENT_VELOCITY(4.f, 3.8f),
    canJump(false),
    holdingJump(false),
    extraJumpTimer(),
    extraJumpDuration(sf::milliseconds(220)),
    positionController(glm::vec2(50, 50), glm::vec2(0, GRAVITY), glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY), glm::vec2(1, 0), glm::vec2(0, 1)),
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

void Player::update(const float& deltaTime, const sf::FloatRect& worldBounds, Tile& tile) {

    //since player velocity only changes in the y direction you can prevent gravity from pulling player down
    //when player is holding jump button and trying ot extend his jump height dont let gravity pull player down
    if(!checkExtendJump()) {

        positionController.updateVelocities(deltaTime);
    }

    positionController.moveAlongXAxis(deltaTime, worldBounds);

    handleCollisionHorizontal(tile, positionController);

    if(positionController.moveAlongYAxis(deltaTime, worldBounds)) {

        canJump = true;
        positionController.setVelocities(positionController.getVelocitiesObjectSpace().x, 0);
    }

    if(handleCollisionVertical(tile, positionController)) {

        canJump = true;
    }

    player.setPosition(positionController.getBoundingBoxWorldSpace().left, positionController.getBoundingBoxWorldSpace().top);
}

void Player::draw(sf::RenderWindow& window) {

    window.draw(player);
}

void Player::jump() {

    if(canJump) {

        extraJumpTimer.restart();
        positionController.setVelocities(positionController.getVelocitiesObjectSpace().x, -MOVEMENT_VELOCITY.y);
        canJump = false;
    }
}
