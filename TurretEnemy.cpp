#include "TurretEnemy.h"
#include "GlobalConstants.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;

TurretEnemy::TurretEnemy(const glm::vec2 &position, const int initialHealth) :
    ShootingEntity(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), initialHealth),
    hiddenStateTimer(),
    exposedStateTimer(),
    hiddenStateDuration(),
    exposedStateDuration(),
    STATE_HIDING(0),
    STATE_COMING_OUT_OF_HIDING(0),
    STATE_GOING_INTO_HIDING(0),
    STATE_SHOOTING(0),
    DOWN(0),
    DOWN_LEFT(0),
    LEFT(0),
    UP_LEFT(0),
    UP(0),
    UP_RIGHT(0),
    RIGHT(0),
    DOWN_RIGHT(0)
    {
        collisionbox.setOrigin(position);

        gun = make_shared<Gun>();
    }

void TurretEnemy::updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map, const vector<glm::vec2> &targetPositions) {

    //update previous bullets
    gun->updatePhysics(deltaTime, worldBounds, map);

    unsigned frame = sprite.getFrame();

    if(currentState != STATE_SHOOTING) {

        //enemy isn't shooting so no need to fire new bullets
        //the gun needs to update though so keep this after the gun physics update
        //when not shooting the animation frame adn the collisionbox match up
        //set the collisionbox according to the current frame of animation
        collisionbox.setActiveCollisionbox(frame, currentState);
        return;
    }

    //when shooting there is only one collisionbox
    collisionbox.setActiveCollisionbox(0, currentState);

    //start shooting at the closest player
    unsigned idClosestTarget = getIdOfClosestTarget(targetPositions);

    if(idClosestTarget < 0) {

        return;
    }

    glm::vec2 gunfireOrigin = calculateGunfireOrigin(targetPositions[idClosestTarget]);
    gun->fire(collisionbox.getOrigin(), gunfireOrigin, direction);
}

void TurretEnemy::updateRendering() {

    gun->updateRendering();

    //don't animate when entity is shooting because the shooting frames aren't animated
    //use the sprites animation state instead of the enemies because if the enemy state changes but the sprite doesn't then the sprite won't draw the correct state
    if(sprite.animate() && sprite.getAnimationState() != STATE_SHOOTING) {

        //animation finished so if its the transition animation then complete transitions
        //don't restart state timers if the current state isn't one of these
        //because turrets also animate while hiding and if you restart state timers at the end up the hiding animation
        //then the timers will be reset every frame so turret will stay hiding forever
        if(currentState == STATE_COMING_OUT_OF_HIDING) {

            setState(STATE_SHOOTING);
            restartStateDurationTimers();

        } else if(currentState == STATE_GOING_INTO_HIDING) {

            setState(STATE_HIDING);
            restartStateDurationTimers();
        }
    }

    //turret only really from hiding or shooting
    //determine if turret needs to start a transition animation
    //and if so which animation
    if(currentState == STATE_HIDING && hiddenStateTimer.getElapsedTime() > hiddenStateDuration) {

        setState(STATE_COMING_OUT_OF_HIDING);
        restartStateDurationTimers();

    } else if(currentState == STATE_SHOOTING && exposedStateTimer.getElapsedTime() > exposedStateDuration) {

        setState(STATE_GOING_INTO_HIDING);
        restartStateDurationTimers();

    } else if(currentState == STATE_SHOOTING) {

        if(direction.vertical == VerticalDirection::STRAIGHT) {

            if(direction.horizontal == HorizontalDirection::LEFT) {

                sprite.setFrame(LEFT);

            }  else {

                sprite.setFrame(RIGHT);
            }

        } else if(direction.vertical == VerticalDirection::UP) {

            if(direction.isFacingCompletelyVertical) {

                sprite.setFrame(UP);

            } else if(direction.horizontal == HorizontalDirection::LEFT) {

                sprite.setFrame(UP_LEFT);

            } else {

                sprite.setFrame(UP_RIGHT);
            }
        } else if(direction.vertical == VerticalDirection::DOWN) {

            if(direction.isFacingCompletelyVertical) {

                sprite.setFrame(DOWN);
            } else if(direction.horizontal == HorizontalDirection::LEFT) {

                sprite.setFrame(DOWN_LEFT);

            } else {

                sprite.setFrame(DOWN_RIGHT);
            }
        }

    } else {

        //going into hiding or coming out of hiding so use the correct collisionbox every frame
        collisionbox.setActiveCollisionbox(sprite.getFrame(), currentState);
    }

    glm::vec2 position = collisionbox.getOrigin();
    sprite.getSprite().setPosition(position.x, position.y);
}

bool TurretEnemy::checkCanGetHit() {

    return checkIsAlive() && currentState != STATE_HIDING;
}

void TurretEnemy::draw(sf::RenderWindow &window) {

    gun->draw(window);
    sprite.draw(window);
}

void TurretEnemy::load(PreloadedTurretData &data) {

    loadBase(data);
    loadShootingEntityData(data);
    scale(data.scale, data.scale);

    STATE_HIDING = data.STATE_HIDING;
    STATE_COMING_OUT_OF_HIDING = data.STATE_COMING_OUT_OF_HIDING;
    STATE_GOING_INTO_HIDING = data.STATE_GOING_INTO_HIDING;
    STATE_SHOOTING = data.STATE_SHOOTING;

    DOWN = data.DOWN;
    DOWN_LEFT = data.DOWN_LEFT;
    LEFT = data.LEFT;
    UP_LEFT = data.UP_LEFT;
    UP = data.UP;
    UP_RIGHT = data.UP_RIGHT;
    RIGHT = data.RIGHT;
    DOWN_RIGHT = data.DOWN_RIGHT;

    hiddenStateDuration = data.hiddenStateDuration;
    exposedStateDuration = data.exposedStateDuration;

    setState(STATE_HIDING);
    restartStateDurationTimers();
}

void TurretEnemy::createCollisionboxes(const vector<sf::FloatRect> &collisionboxes) {

    collisionbox.clearCollisionboxes();

    for(unsigned i = 0; i < collisionboxes.size(); ++i) {

        collisionbox.insertCollisionbox(collisionboxes[i]);
    }
}

unsigned TurretEnemy::getIdOfClosestTarget(const vector<glm::vec2> &targetPositions) const {

    if(targetPositions.size() == 0) {

        return -1;
    }

    unsigned closestId = 0;
    float distanceToClosest = glm::dot(targetPositions[0] - collisionbox.getOrigin(), targetPositions[0] - collisionbox.getOrigin());

    for(unsigned i = 1; i < targetPositions.size(); ++i) {

        float distanceToTarget = glm::dot(targetPositions[i] - collisionbox.getOrigin(), targetPositions[i] - collisionbox.getOrigin());

        if(distanceToTarget < distanceToClosest) {

            distanceToClosest = distanceToTarget;
            closestId = i;
        }
    }

    return closestId;
}

glm::vec2 TurretEnemy::calculateGunfireOrigin(const glm::vec2 &targetPosition) {

    determineDirection(targetPosition);

    return bulletOriginForState[convertToCombinedAxis(direction)];
}

void TurretEnemy::determineDirection(const glm::vec2 &targetPosition) {

    //determine angle to target
    float x = targetPosition.x - collisionbox.getOrigin().x;
    float y = targetPosition.y - collisionbox.getOrigin().y;

    float angleRadians = atan2(y, x);

    //angle goes positive in clockwise direction when tis hould be counter clockwise so negate it
    float angleDegrees = angleRadians * RAD_TO_DEG_RATIO * -1;

    direction.isFacingCompletelyVertical = false;

    if(angleDegrees <= 60 && angleDegrees >= -60) {

        direction.horizontal = HorizontalDirection::RIGHT;

    } else if(angleDegrees >= 120 || angleDegrees <= -120) {

        direction.horizontal = HorizontalDirection::LEFT;

    } else {

        direction.isFacingCompletelyVertical = true;
    }

    if(angleDegrees <= 150 && angleDegrees >= 30) {

        direction.vertical = VerticalDirection::UP;

    } else if(angleDegrees <= -30 && angleDegrees >= -150) {

        direction.vertical = VerticalDirection::DOWN;

    } else {

        direction.vertical = VerticalDirection::STRAIGHT;
    }
}

void TurretEnemy::restartStateDurationTimers() {

    hiddenStateTimer.restart();
    exposedStateTimer.restart();
}
