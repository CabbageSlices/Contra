#include "Player.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Gun.h"
#include "Tile.h"

using std::make_shared;
using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;

Player::Player(const PlayerKeys& keyConfiguration):
    ShootingEntity(glm::vec2(0, GRAVITY), glm::vec2(4.f, 4.05f), glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY), 3),
    STATE_STANDING_LEFT(0),
    STATE_STANDING_UP_FACING_LEFT(1),
    STATE_STANDING_UP_FACING_RIGHT(2),
    STATE_STANDING_RIGHT(3),
    STATE_WALKING_DOWN_LEFT(4),
    STATE_WALKING_LEFT(5),
    STATE_WALKING_UP_LEFT(6),
    STATE_WALKING_UP_RIGHT(7),
    STATE_WALKING_RIGHT(8),
    STATE_WALKING_DOWN_RIGHT(9),
    STATE_CROUCHING_LEFT(10),
    STATE_CROUCHING_RIGHT(11),
    STATE_FALLING_DOWN_FACING_LEFT(12),
    STATE_FALLING_DOWN_FACING_RIGHT(13),
    STATE_FALLING_DOWN_LEFT(14),
    STATE_FALLING_LEFT(15),
    STATE_FALLING_UP_LEFT(16),
    STATE_FALLING_UP_FACING_LEFT(17),
    STATE_FALLING_UP_FACING_RIGHT(18),
    STATE_FALLING_UP_RIGHT(19),
    STATE_FALLING_RIGHT(20),
    STATE_FALLING_DOWN_RIGHT(21),
    STATE_JUMPING(22),
    STATE_DYING_FACING_RIGHT(23),
    STATE_DYING_FACING_LEFT(24),
    STATE_DEAD(25),
    lifeState(ALIVE),
    standingOnSolid(false),
    standingOnTile(false),
    standingOnPassablePlatform(false),
    holdingJump(false),
    wasJumpButtonPressed(false),
    extraJumpTimer(),
    extraJumpDuration(sf::milliseconds(200)),
    controls(keyConfiguration),
    respawnTimer(),
    respawnDelay(sf::seconds(1.5)),
    respawnInvinsibilityTimer(),
    respawnInvinsibilityDuration(sf::seconds(3))
    {
        gun = std::make_shared<Gun>();

        if(!gun) {

            exit(-1);
        }

        entity.setSize(sf::Vector2f(50, 100));
    }

void Player::handleInputEvents(sf::Event& event, sf::RenderWindow& window) {

}

void Player::handleKeystate(sf::RenderWindow& window) {

    if(!checkIsAlive()) {

        CollisionboxMovementController.setVelocities(0, CollisionboxMovementController.getVelocities().y);
        return; //don't handle controls if player is dead
    }

    if(sf::Keyboard::isKeyPressed(controls.left)) {

        CollisionboxMovementController.setVelocities(-MOVEMENT_VELOCITY.x, CollisionboxMovementController.getVelocities().y);

    } else if(sf::Keyboard::isKeyPressed(controls.right)) {

        CollisionboxMovementController.setVelocities(MOVEMENT_VELOCITY.x, CollisionboxMovementController.getVelocities().y);

    } else {

        CollisionboxMovementController.setVelocities(0, CollisionboxMovementController.getVelocities().y);
    }


    if(sf::Keyboard::isKeyPressed(controls.jump) && sf::Keyboard::isKeyPressed(controls.down)) {

        jumpDown();

    }

    //don't put this in an if else statement with the downjumping because if you do then players won't be able to jump while running and holding the down button
    if(sf::Keyboard::isKeyPressed(controls.jump)) {

        jump();
    }

    if(sf::Keyboard::isKeyPressed(controls.fire)) {

        fireGun();
    }

    holdingJump = sf::Keyboard::isKeyPressed(controls.jump);

    determineDirection();
}

void Player::updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map) {

    //since player velocity only changes in the y direction you can prevent gravity from pulling player down
    //when player is holding jump button and trying ot extend his jump height dont let gravity pull player down
    if(!checkExtendJump()) {

        CollisionboxMovementController.updateVelocities(deltaTime);
    }

    CollisionboxMovementController.moveAlongXAxis(deltaTime, worldBounds);

    CollisionResponse collisionResponseHorizontal = handleTileCollisionHorizontally(map);

    if(CollisionboxMovementController.moveAlongYAxis(deltaTime, worldBounds)) {

        standingOnSolid = true;
        CollisionboxMovementController.setVelocities(CollisionboxMovementController.getVelocities().x, 0);

    } else {

        standingOnSolid = false;
    }

    CollisionResponse collisionResponseVertical = handleTileCollisionVertically(map);

    standingOnTile = (collisionResponseHorizontal.pushedToTop || collisionResponseVertical.pushedToTop);
    standingOnPassablePlatform = collisionResponseHorizontal.canFallThroughGround || collisionResponseVertical.canFallThroughGround;

    gun->updatePhysics(deltaTime, worldBounds, map);
}

void Player::updateRendering() {

    if(sprite.animate() && (currentState == STATE_DYING_FACING_LEFT || currentState == STATE_DYING_FACING_RIGHT)) {

        die();
    }

    determineRenderingState();
    Collisionbox.setActiveCollisionbox(sprite.getFrame(), currentState);

    sf::FloatRect activeCollisionbox = Collisionbox.getActiveCollisionboxWorldSpace();
    //cout << activeCollisionbox.top << "  " << activeCollisionbox.height << endl;
    entity.setPosition(activeCollisionbox.left, activeCollisionbox.top);
    entity.setSize(sf::Vector2f(activeCollisionbox.width, activeCollisionbox.height));

    setPosition(Collisionbox.getOrigin());

    gun->updateRendering();
}

bool Player::checkCanGetHit() {

    return respawnInvinsibilityTimer.getElapsedTime() > respawnInvinsibilityDuration && checkIsAlive();
}

void Player::getHit(int damage) {

    startDeathAnimation();

    ///later this should start a death animation and set lifestate to dying, and once the animation finishes the player should actually die
    ///for now just make player die since there are no animations yet
    ///die();
}

void Player::respondToCollision(const CollisionResponse &collisionResponse) {

    //check if player is able to jump now
    standingOnSolid = collisionResponse.pushedToTop || standingOnSolid;
    standingOnPassablePlatform = collisionResponse.canFallThroughGround || standingOnPassablePlatform;
}

void Player::load(PreloadedPlayerData &data) {

    loadBase(data);
    loadShootingEntityData(data);

    scale(data.scale, data.scale);

    STATE_STANDING_LEFT = data.STATE_STANDING_LEFT;
    STATE_STANDING_UP_FACING_LEFT = data.STATE_STANDING_UP_FACING_LEFT;
    STATE_STANDING_UP_FACING_RIGHT = data.STATE_STANDING_UP_FACING_RIGHT;
    STATE_STANDING_RIGHT = data.STATE_STANDING_RIGHT;

    STATE_WALKING_DOWN_LEFT = data.STATE_WALKING_DOWN_LEFT;
    STATE_WALKING_LEFT = data.STATE_WALKING_LEFT;
    STATE_WALKING_UP_LEFT = data.STATE_WALKING_UP_LEFT;
    STATE_WALKING_UP_RIGHT = data.STATE_WALKING_UP_RIGHT;
    STATE_WALKING_RIGHT = data.STATE_WALKING_RIGHT;
    STATE_WALKING_DOWN_RIGHT = data.STATE_WALKING_DOWN_RIGHT;

    STATE_CROUCHING_LEFT = data.STATE_CROUCHING_LEFT;
    STATE_CROUCHING_RIGHT = data.STATE_CROUCHING_RIGHT;

    STATE_FALLING_DOWN_FACING_LEFT = data.STATE_FALLING_DOWN_FACING_LEFT;
    STATE_FALLING_DOWN_FACING_RIGHT = data.STATE_FALLING_DOWN_FACING_RIGHT;
    STATE_FALLING_DOWN_LEFT = data.STATE_FALLING_DOWN_LEFT;
    STATE_FALLING_LEFT = data.STATE_FALLING_LEFT;
    STATE_FALLING_UP_LEFT = data.STATE_FALLING_UP_LEFT;
    STATE_FALLING_UP_FACING_LEFT = data.STATE_FALLING_UP_FACING_LEFT;
    STATE_FALLING_UP_FACING_RIGHT = data.STATE_FALLING_UP_FACING_RIGHT;
    STATE_FALLING_UP_RIGHT = data.STATE_FALLING_UP_RIGHT;
    STATE_FALLING_RIGHT = data.STATE_FALLING_RIGHT;
    STATE_FALLING_DOWN_RIGHT = data.STATE_FALLING_DOWN_RIGHT;

    STATE_JUMPING = data.STATE_JUMPING;

    STATE_DYING_FACING_LEFT = data.STATE_DYING_FACING_LEFT;
    STATE_DYING_FACING_RIGHT = data.STATE_DYING_FACING_RIGHT;

    STATE_DEAD = data.STATE_DEAD;

    setState(STATE_STANDING_RIGHT);
}

bool Player::checkIsAlive() {

    return lifeState == ALIVE;
}

bool Player::checkCanRespawn() {

    return lifeState == DEAD && health > 0 && respawnTimer.getElapsedTime() > respawnDelay;
}

void Player::respawn(const sf::FloatRect &cameraBounds) {

    lifeState = ALIVE;

    //move player slightly below camera beucase once they respawn if they hold jump then he wil always be able to jump
    //because he will be touchign top of screen so the game registers it as if he is standing on the ground
    glm::vec2 spawnPosition(cameraBounds.left + Collisionbox.getActiveCollisionboxObjectSpace().width, cameraBounds.top + 1);
    setPosition(spawnPosition);

    setLives(health - 1);

    respawnInvinsibilityTimer.restart();
}

void Player::draw(sf::RenderWindow& window) {

    if(lifeState == DEAD) {

        return;
    }

    ShootingEntity::draw(window);
    sprite.draw(window);
}

void Player::setLives(const int &newLives) {

    //health parameter is the player's lives
    health = newLives;
}

int Player::getLives() const {

    return health;
}

bool Player::checkCanJump() const {

    //if player is falling it means he isn't standing on top of any object because if he was, his velocity would be 0
    //therefore he can't jump if his velocity isn't 0
    return (standingOnSolid || standingOnTile) && (CollisionboxMovementController.getVelocities().y == 0) && (lifeState == ALIVE);
}

//don't let players jump down while running
bool Player::checkCanJumpDown() const {

    return standingOnPassablePlatform && checkCanJump() && CollisionboxMovementController.getVelocities().x == 0;
}

bool Player::checkIsJumping() const {

    return checkIsInAir() && wasJumpButtonPressed;
}

bool Player::checkIsCrouching() const {

    //player is crouching if he is holding down, not moving, and on the ground
    return checkCanJump() && sf::Keyboard::isKeyPressed(controls.down) && CollisionboxMovementController.getVelocities().x == 0;
}

bool Player::checkIsInAir() const {

    //player is only in the air if he isn't standing on anything
    return !standingOnSolid && !standingOnTile && !standingOnPassablePlatform;
}

//check if player can extend his jump by holding the jump button
bool Player::checkExtendJump() const {

    return holdingJump && extraJumpTimer.getElapsedTime() < extraJumpDuration;
}

void Player::fireGun() {

    bool canFire = checkIsAlive();

    if(canFire) {

        gun->fire(Collisionbox.getOrigin(), calculateGunfireOrigin(), direction);
    }
}

void Player::startDeathAnimation() {

    lifeState = DYING;
}

void Player::determineRenderingState() {

    if(lifeState == LifeState::DYING) {

        if(direction.horizontal == HorizontalDirection::LEFT) {

            setState(STATE_DYING_FACING_LEFT);
        }

        if(direction.horizontal == HorizontalDirection::RIGHT) {

            setState(STATE_DYING_FACING_RIGHT);
        }

        return;
    }

    if(lifeState == LifeState::DEAD) {

        setState(STATE_DEAD);
        return;
    }

    //if player is no longer in the air then his jump has finished
    //so indicate that he didn't press the jump button so the animation doesn't draw the player jumping
    wasJumpButtonPressed = checkIsInAir() && wasJumpButtonPressed;

    if(checkIsJumping()) {

        setState(STATE_JUMPING);

        return;
    }

    CombinedAxis::Direction combinedDirection = convertToCombinedAxis(direction);

    if(checkIsInAir()) {

        switch(combinedDirection) {

            case CombinedAxis::LEFT : {

                setState(STATE_FALLING_LEFT);
                return;
            }

            case CombinedAxis::UP_LEFT : {

                setState(STATE_FALLING_UP_LEFT);
                return;
            }

            case CombinedAxis::UP : {

                if(direction.horizontal == HorizontalDirection::LEFT) {

                    setState(STATE_FALLING_UP_FACING_LEFT);
                    return;
                }

                if(direction.horizontal == HorizontalDirection::RIGHT) {

                    setState(STATE_FALLING_UP_FACING_RIGHT);
                    return;
                }
            }

            case CombinedAxis::UP_RIGHT : {

                setState(STATE_FALLING_UP_RIGHT);
                return;
            }

            case CombinedAxis::RIGHT : {

                setState(STATE_FALLING_RIGHT);
                return;
            }

            case CombinedAxis::DOWN_RIGHT : {

                setState(STATE_FALLING_DOWN_RIGHT);
                return;
            }

            case CombinedAxis::DOWN : {

                if(direction.horizontal == HorizontalDirection::LEFT) {

                    setState(STATE_FALLING_DOWN_FACING_LEFT);
                    return;
                }

                if(direction.horizontal == HorizontalDirection::RIGHT) {

                    setState(STATE_FALLING_DOWN_FACING_RIGHT);
                    return;
                }
            }

            default : {

                setState(STATE_FALLING_DOWN_FACING_LEFT);
                return;
            }
        }

        return;
    }

    if(checkIsCrouching()) {

        if(direction.horizontal == HorizontalDirection::LEFT) {

            setState(STATE_CROUCHING_LEFT);
        }

        if(direction.horizontal == HorizontalDirection::RIGHT) {

            setState(STATE_CROUCHING_RIGHT);
        }

        return;
    }

    if(CollisionboxMovementController.getVelocities().x == 0) {

        if(direction.vertical == VerticalDirection::STRAIGHT && direction.horizontal == HorizontalDirection::LEFT) {

            setState(STATE_STANDING_LEFT);
        }

        if(direction.vertical == VerticalDirection::STRAIGHT && direction.horizontal == HorizontalDirection::RIGHT) {

            setState(STATE_STANDING_RIGHT);
        }

        if(direction.vertical == VerticalDirection::UP) {

            if(direction.horizontal == HorizontalDirection::LEFT) {

                setState(STATE_STANDING_UP_FACING_LEFT);
            }

            if(direction.horizontal == HorizontalDirection::RIGHT) {

                setState(STATE_STANDING_UP_FACING_RIGHT);
            }
        }

        return;
    }

    //player is moving and doing stuff now
    if(direction.vertical == VerticalDirection::STRAIGHT && direction.horizontal == HorizontalDirection::LEFT) {

        setState(STATE_WALKING_LEFT);
        return;
    }

    if(direction.vertical == VerticalDirection::STRAIGHT && direction.horizontal == HorizontalDirection::RIGHT) {

        setState(STATE_WALKING_RIGHT);
        return;
    }

    if(direction.vertical == VerticalDirection::UP && direction.horizontal == HorizontalDirection::LEFT) {

        setState(STATE_WALKING_UP_LEFT);
        return;
    }

    if(direction.vertical == VerticalDirection::UP && direction.horizontal == HorizontalDirection::RIGHT) {

        setState(STATE_WALKING_UP_RIGHT);
        return;
    }

    if(direction.vertical == VerticalDirection::DOWN && direction.horizontal == HorizontalDirection::LEFT) {

        setState(STATE_WALKING_DOWN_LEFT);
        return;
    }

    if(direction.vertical == VerticalDirection::DOWN && direction.horizontal == HorizontalDirection::RIGHT) {

        setState(STATE_WALKING_DOWN_RIGHT);
        return;
    }
}

glm::vec2 Player::calculateGunfireOrigin() {

    //if player isn't jumping then his gunfire direction is dtermined by his currnet state
    if(currentState != STATE_JUMPING) {

        return bulletOriginForState[currentState];
    }

    //jumping so bullet direction is determine by the direction the player is facing
    CombinedAxis::Direction combinedAxisDirection = convertToCombinedAxis(direction);
    return bulletOriginForState[combinedAxisDirection];

    //default facing right so its at the right side of the player
    glm::vec2 gunPosition(entity.getGlobalBounds().width, entity.getGlobalBounds().height / 2);

    if(direction.isFacingCompletelyVertical && !checkIsCrouching()) {

        gunPosition.x = entity.getGlobalBounds().width / 2;

        if(direction.vertical == VerticalDirection::UP) {

            gunPosition.y = 0;

        } else {

            gunPosition.y = entity.getGlobalBounds().height;
        }

        return gunPosition;
    }

    if(direction.horizontal == HorizontalDirection::RIGHT) {

        gunPosition.x = entity.getGlobalBounds().width;

    } else {

        gunPosition.x = 0;
    }

    if(direction.vertical == VerticalDirection::UP) {

        gunPosition.y = 0;

    } else if(direction.vertical == VerticalDirection::DOWN) {

        gunPosition.y = entity.getGlobalBounds().height;

    } else {

        gunPosition.y = entity.getGlobalBounds().height / 2;
    }

    if(checkIsCrouching()) {

        gunPosition.y = entity.getGlobalBounds().height;
    }

    return gunPosition;
}

CollisionResponse Player::handleTileCollision(TileMap& map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, CollisionboxMovementController& object)) {

    vector<shared_ptr<Tile> > tiles = getSurroundingTiles(map, glm::vec2(TILE_SIZE, TILE_SIZE));

    CollisionResponse collisionResponse;
    collisionResponse.canFallThroughGround = true;

    for(unsigned i = 0; i < tiles.size(); ++i) {

        CollisionResponse response = collisionFunction(tiles[i], CollisionboxMovementController);

        if(response.pushedToTop) {

            collisionResponse.pushedToTop = true;

            //if player ever stands on a solid tile that prevents fall through then don't let him fall through floor
            //this makes it so player can only fall through ground if all checks allow him to fall through ground
            collisionResponse.canFallThroughGround = collisionResponse.canFallThroughGround && response.canFallThroughGround;

        }

        if(response.handledHorizontal) {

            collisionResponse.handledHorizontal = true;
        }
    }

    //if vertical collision was never handled then don't let player call through ground
    collisionResponse.canFallThroughGround = collisionResponse.canFallThroughGround && collisionResponse.pushedToTop;

    return collisionResponse;
}

void Player::determineDirection() {

    if(CollisionboxMovementController.getVelocities().x < 0) {

        direction.horizontal = HorizontalDirection::LEFT;

    } else if(CollisionboxMovementController.getVelocities().x > 0) {

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

    }

    direction.isFacingCompletelyVertical = (CollisionboxMovementController.getVelocities().x == 0 && direction.vertical != VerticalDirection::STRAIGHT);
}

void Player::jump() {

    if(checkCanJump()) {

        extraJumpTimer.restart();
        CollisionboxMovementController.setVelocities(CollisionboxMovementController.getVelocities().x, -MOVEMENT_VELOCITY.y);
        stopStandingOnPlatforms();
        wasJumpButtonPressed = true;
    }
}

void Player::jumpDown() {

    if(checkCanJumpDown()) {

        stopStandingOnPlatforms();

        //when jumping down just push the player down into the ground since the collision detection code should allow him to pass
        //push him just out of the threshold so collision resolution won't occur
        Collisionbox.move(glm::vec2(0, fallThroughTopThreshold + 1.f));

        //also set his velocity to go downwards that way it looks a bit smoother
        CollisionboxMovementController.setVelocities(CollisionboxMovementController.getVelocities().x, 1.0f);
    }
}

void Player::stopStandingOnPlatforms() {

    standingOnPassablePlatform = false;
    standingOnSolid = false;
    standingOnTile = false;
}

void Player::die() {

    ///later this should stop animations but for now just kill player
    lifeState = DEAD;

    //reset the gun so player starts with a basic gun again
    gun = make_shared<Gun>();
    respawnTimer.restart();
}
