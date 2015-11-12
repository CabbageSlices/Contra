#include "Player.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Gun.h"
#include "Tile.h"
#include "EntityAnimationStates.h"

using std::make_shared;
using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;

Player::Player(const glm::vec2 &spawnPosition, const PlayerKeys& keyConfiguration):
    ShootingEntity(glm::vec2(0, GRAVITY), glm::vec2(4.f, 4.5f), glm::vec2(TERMINAL_VELOCITY, TERMINAL_VELOCITY), 3),
    jumpingHitboxState(0),
    lifeState(ALIVE),
    standingOnSolid(false),
    standingOnTile(false),
    standingOnPassablePlatform(false),
    holdingJump(false),
    wasJumpButtonPressed(false),
    extraJumpTimer(),
    extraJumpDuration(sf::milliseconds(210)),
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
        setPosition(spawnPosition);
    }

void Player::handleInputEvents(sf::Event& event, sf::RenderWindow& window) {

}

void Player::handleKeystate(sf::RenderWindow& window) {

    if(!checkIsAlive()) {

        hitboxMovementController.setVelocities(0, hitboxMovementController.getVelocities().y);
        return; //don't handle controls if player is dead
    }

    if(sf::Keyboard::isKeyPressed(controls.left)) {

        hitboxMovementController.setVelocities(-MOVEMENT_VELOCITY.x, hitboxMovementController.getVelocities().y);

    } else if(sf::Keyboard::isKeyPressed(controls.right)) {

        hitboxMovementController.setVelocities(MOVEMENT_VELOCITY.x, hitboxMovementController.getVelocities().y);

    } else {

        hitboxMovementController.setVelocities(0, hitboxMovementController.getVelocities().y);
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

    standingOnTile = (collisionResponseHorizontal.pushedToTop || collisionResponseVertical.pushedToTop);
    standingOnPassablePlatform = collisionResponseHorizontal.canFallThroughGround || collisionResponseVertical.canFallThroughGround;

    gun->updatePhysics(deltaTime, worldBounds, map);

    //if player is no longer in the air then his jump has finished
    //so indicate that he didn't press the jump button so the animation doesn't draw the player jumping
    //binary and operator with checkIsInAir is used because once the player lets go of the jump button
    //there is no other function that sets the wasJumpButtonPressed to false
    //which is why its only set to false if he is on the ground again, since to set it to true
    //you would have to jump again in order to be in the air
    wasJumpButtonPressed = checkIsInAir() && wasJumpButtonPressed;

    //if player isn't jumping use the default hitbox
    if(!checkIsJumping()) {

        hitbox.setActiveHitbox(0, defaultHitboxState);
    }

    matchHitboxPosition();
}

void Player::updateRendering() {

    if(sprite.animate() && (currentState == PlayerEnums::STATE_DYING_FACING_LEFT || currentState == PlayerEnums::STATE_DYING_FACING_RIGHT)) {

        die();
    }

    determineRenderingState();
    hurtbox.setActiveHitbox(sprite.getFrame(), currentState);

    sf::FloatRect box = hitbox.getActiveHitboxWorldSpace();
    entity.setPosition(box.left, box.top);
    entity.setSize(sf::Vector2f(box.width, box.height));

    matchHitboxPosition();

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

    //see updatephysics function for explanation of why you set was jumpbuttonpressed to this.
    //reset wasjumpbuttonpressed becasue if player is now standing on a solid platform we need to indicate that he is no longer jumping
    wasJumpButtonPressed = checkIsInAir() && wasJumpButtonPressed;

    //if player isn't jumping use the default hitbox
    if(!checkIsJumping()) {

        hitbox.setActiveHitbox(0, defaultHitboxState);
    }

    matchHitboxPosition();
}

void Player::load(const PreloadedPlayerData &data) {

    loadBase(data);
    loadShootingEntityData(data);

    scale(data.scale, data.scale);

    jumpingHitboxState = data.jumpingHitboxState;

    setState(PlayerEnums::STATE_STANDING_RIGHT);
    hitbox.setActiveHitbox(0, defaultHitboxState);
}

bool Player::checkIsAlive() {

    return lifeState == ALIVE;
}

bool Player::checkCanRespawn() {

    return lifeState == DEAD && health > 0 && respawnTimer.getElapsedTime() > respawnDelay;
}

void Player::spawn(const glm::vec2 &spawnPosition) {

    //pretty much just used to set the psotiion of the player
    setPosition(spawnPosition);
    respawnInvinsibilityTimer.restart();
}

void Player::respawn(const sf::FloatRect &cameraBounds) {

    lifeState = ALIVE;

    //move player slightly below camera beucase once they respawn if they hold jump then he wil always be able to jump
    //because he will be touchign top of screen so the game registers it as if he is standing on the ground
    glm::vec2 spawnPosition(cameraBounds.left + hitbox.getActiveHitboxObjectSpace().width, cameraBounds.top + 1);
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
    return (standingOnSolid || standingOnTile) && (hitboxMovementController.getVelocities().y == 0) && (lifeState == ALIVE);
}

//don't let players jump down while running
bool Player::checkCanJumpDown() const {

    return standingOnPassablePlatform && checkCanJump() && hitboxMovementController.getVelocities().x == 0;
}

bool Player::checkIsJumping() const {

    return checkIsInAir() && wasJumpButtonPressed;
}

bool Player::checkIsCrouching() const {

    //player is crouching if he is holding down, not moving, and on the ground
    return checkCanJump() && sf::Keyboard::isKeyPressed(controls.down) && hitboxMovementController.getVelocities().x == 0;
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

        gun->fire(hurtbox.getOrigin(), calculateGunfireOrigin(), direction);
    }
}

void Player::startDeathAnimation() {

    lifeState = DYING;
}

void Player::determineRenderingState() {

    if(lifeState == LifeState::DYING) {

        if(direction.horizontal == HorizontalDirection::LEFT) {

            setState(PlayerEnums::STATE_DYING_FACING_LEFT);
        }

        if(direction.horizontal == HorizontalDirection::RIGHT) {

            setState(PlayerEnums::STATE_DYING_FACING_RIGHT);
        }

        return;
    }

    if(lifeState == LifeState::DEAD) {

        setState(PlayerEnums::STATE_DEAD);
        return;
    }

    if(checkIsJumping()) {

        setState(PlayerEnums::STATE_JUMPING);

        return;
    }

    CombinedAxis::Direction combinedDirection = convertToCombinedAxis(direction);

    if(checkIsInAir()) {

        switch(combinedDirection) {

            case CombinedAxis::LEFT : {

                setState(PlayerEnums::STATE_FALLING_LEFT);
                return;
            }

            case CombinedAxis::UP_LEFT : {

                setState(PlayerEnums::STATE_FALLING_UP_LEFT);
                return;
            }

            case CombinedAxis::UP : {

                if(direction.horizontal == HorizontalDirection::LEFT) {

                    setState(PlayerEnums::STATE_FALLING_UP_FACING_LEFT);
                    return;
                }

                if(direction.horizontal == HorizontalDirection::RIGHT) {

                    setState(PlayerEnums::STATE_FALLING_UP_FACING_RIGHT);
                    return;
                }
            }

            case CombinedAxis::UP_RIGHT : {

                setState(PlayerEnums::STATE_FALLING_UP_RIGHT);
                return;
            }

            case CombinedAxis::RIGHT : {

                setState(PlayerEnums::STATE_FALLING_RIGHT);
                return;
            }

            case CombinedAxis::DOWN_RIGHT : {

                setState(PlayerEnums::STATE_FALLING_DOWN_RIGHT);
                return;
            }

            case CombinedAxis::DOWN : {

                if(direction.horizontal == HorizontalDirection::LEFT) {

                    setState(PlayerEnums::STATE_FALLING_DOWN_FACING_LEFT);
                    return;
                }

                if(direction.horizontal == HorizontalDirection::RIGHT) {

                    setState(PlayerEnums::STATE_FALLING_DOWN_FACING_RIGHT);
                    return;
                }
            }

            default : {

                setState(PlayerEnums::STATE_FALLING_DOWN_FACING_LEFT);
                return;
            }
        }

        return;
    }

    if(checkIsCrouching()) {

        if(direction.horizontal == HorizontalDirection::LEFT) {

            setState(PlayerEnums::STATE_CROUCHING_LEFT);
        }

        if(direction.horizontal == HorizontalDirection::RIGHT) {

            setState(PlayerEnums::STATE_CROUCHING_RIGHT);
        }

        return;
    }

    if(hitboxMovementController.getVelocities().x == 0) {

        if(direction.vertical == VerticalDirection::STRAIGHT && direction.horizontal == HorizontalDirection::LEFT) {

            setState(PlayerEnums::STATE_STANDING_LEFT);
        }

        if(direction.vertical == VerticalDirection::STRAIGHT && direction.horizontal == HorizontalDirection::RIGHT) {

            setState(PlayerEnums::STATE_STANDING_RIGHT);
        }

        if(direction.vertical == VerticalDirection::UP) {

            if(direction.horizontal == HorizontalDirection::LEFT) {

                setState(PlayerEnums::STATE_STANDING_UP_FACING_LEFT);
            }

            if(direction.horizontal == HorizontalDirection::RIGHT) {

                setState(PlayerEnums::STATE_STANDING_UP_FACING_RIGHT);
            }
        }

        return;
    }

    //player is moving and doing stuff now
    if(direction.vertical == VerticalDirection::STRAIGHT && direction.horizontal == HorizontalDirection::LEFT) {

        setState(PlayerEnums::STATE_WALKING_LEFT);
        return;
    }

    if(direction.vertical == VerticalDirection::STRAIGHT && direction.horizontal == HorizontalDirection::RIGHT) {

        setState(PlayerEnums::STATE_WALKING_RIGHT);
        return;
    }

    if(direction.vertical == VerticalDirection::UP && direction.horizontal == HorizontalDirection::LEFT) {

        setState(PlayerEnums::STATE_WALKING_UP_LEFT);
        return;
    }

    if(direction.vertical == VerticalDirection::UP && direction.horizontal == HorizontalDirection::RIGHT) {

        setState(PlayerEnums::STATE_WALKING_UP_RIGHT);
        return;
    }

    if(direction.vertical == VerticalDirection::DOWN && direction.horizontal == HorizontalDirection::LEFT) {

        setState(PlayerEnums::STATE_WALKING_DOWN_LEFT);
        return;
    }

    if(direction.vertical == VerticalDirection::DOWN && direction.horizontal == HorizontalDirection::RIGHT) {

        setState(PlayerEnums::STATE_WALKING_DOWN_RIGHT);
        return;
    }
}

glm::vec2 Player::calculateGunfireOrigin() {

    //if player isn't jumping then his gunfire direction is dtermined by his currnet state
    if(currentState != PlayerEnums::STATE_JUMPING) {

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

CollisionResponse Player::handleTileCollision(TileMap& map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object)) {

    vector<shared_ptr<Tile> > tiles = getSurroundingTiles(map, glm::vec2(TILE_SIZE, TILE_SIZE));

    CollisionResponse collisionResponse;
    collisionResponse.canFallThroughGround = true;

    for(unsigned i = 0; i < tiles.size(); ++i) {

        CollisionResponse response = collisionFunction(tiles[i], hitboxMovementController);

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

    }

    direction.isFacingCompletelyVertical = (hitboxMovementController.getVelocities().x == 0 && direction.vertical != VerticalDirection::STRAIGHT);
}

void Player::jump() {

    if(checkCanJump()) {

        extraJumpTimer.restart();
        hitboxMovementController.setVelocities(hitboxMovementController.getVelocities().x, -MOVEMENT_VELOCITY.y);
        stopStandingOnPlatforms();
        wasJumpButtonPressed = true;
        hitbox.setActiveHitbox(0, jumpingHitboxState);

        ///make player go slightly upwards when jumping, because when he jumps his hitbox becomes smaller and the jump starts from the ground,
        ///and this causes him to be smaller than a tile and allows him to go under tiles
        hitbox.move(glm::vec2(0, -hitbox.getActiveHitboxObjectSpace().height / 2));
    }
}

void Player::jumpDown() {

    if(checkCanJumpDown()) {

        stopStandingOnPlatforms();

        //when jumping down just push the player down into the ground since the collision detection code should allow him to pass
        //push him just out of the threshold so collision resolution won't occur
        hitbox.move(glm::vec2(0, fallThroughTopThreshold + 1.f));

        //also set his velocity to go downwards that way it looks a bit smoother
        hitboxMovementController.setVelocities(hitboxMovementController.getVelocities().x, 1.0f);
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
