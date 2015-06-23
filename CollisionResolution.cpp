#include "CollisionResolution.h"

#include "EntityBase.h"
#include "Player.h"
#include "Bullet.h"
#include "DestructibleBlock.h"

using std::shared_ptr;

glm::vec2 calculateCollisionResolutionTranslation(sf::FloatRect rectA, sf::FloatRect rectB) {

    //not intersecting at all
    if(!rectA.intersects(rectB)) {

        return glm::vec2(0, 0);
    }

    //smallest overlap on x and y axis
    float distanceBRightALeft = rectB.left + rectB.width - rectA.left;
    float distanceARightBLeft = rectA.left + rectA.width - rectB.left;
    float distanceBBottomATop = rectB.top + rectB.height - rectA.top;
    float distanceABottomBTop = rectA.top + rectA.height - rectB.top;

    //find minimum displacement on x and y axis
    float minX = glm::min(distanceBRightALeft, distanceARightBLeft);
    float minY = glm::min(distanceBBottomATop, distanceABottomBTop);

    if(minX < minY) {

        //x displacement is smaller so translate on horizontal direction
        if(distanceBRightALeft < distanceARightBLeft) {

            return glm::vec2(distanceBRightALeft, 0);

        } else {

            return glm::vec2(-distanceARightBLeft, 0);
        }
    }

    if(distanceBBottomATop < distanceABottomBTop) {

        return glm::vec2(0, distanceBBottomATop);

    } else {

        return glm::vec2(0, -distanceABottomBTop);
    }

    return glm::vec2(0, 0);
}


void destructibleBlockEntityCollision(shared_ptr<DestructibleBlock> block, shared_ptr<EntityBase> entity) {

    if(!block->checkCanGetHit()) {

        return;
    }

    sf::FloatRect entityRect = entity->getHitbox().getActiveHitboxWorldSpace();
    sf::FloatRect blockRect = block->getHitbox().getActiveHitboxWorldSpace();

    if(!blockRect.intersects(entityRect)) {

        return;
    }

    glm::vec2 minimumTranslation = calculateCollisionResolutionTranslation(entityRect, blockRect);

    entity->getHitbox().move(minimumTranslation);

    CollisionResponse response;

    //check if object is standing on top of block
    response.pushedToTop = (minimumTranslation.y < 0);

    //if block was hit from the bottom then destory the block, only do so if object was jumping
    if(minimumTranslation.y > 0 && entity->getMovementController().getVelocities().y < 0) {

        block->getHit(1);
    }

    //check if there was any horizontal translation involved
    response.handledHorizontal = (minimumTranslation.x != 0);

    //if a vertical translation is there it means there was a vertical collision resolution
    //so stop the entity from moving in the y direction
    if(minimumTranslation.y != 0) {

        entity->getMovementController().setVelocities(entity->getMovementController().getVelocities().x, 0);
    }

    //make entity respond to collision
    entity->respondToCollision(response);
}

void bulletNonDeflectingEntityCollision(shared_ptr<Bullet> bullet, shared_ptr<EntityBase> entity) {

    if(!bullet->checkIsAlive() || !entity->checkCanGetHit()) {

        return;
    }

    sf::FloatRect entityRect = entity->getHitbox().getActiveHitboxWorldSpace();
    sf::FloatRect bulletRect = bullet->getHitbox().getActiveHitboxWorldSpace();

    if(!entityRect.intersects(bulletRect)) {

        return;
    }

    entity->getHit(1);
    bullet->killBullet();
}

void playerEnemyEntityCollision(shared_ptr<Player> player, shared_ptr<EntityBase> enemy) {

    if(!player->checkCanGetHit() || !enemy->checkCanGetHit()) {

        return;
    }

    sf::FloatRect enemyRect = enemy->getHitbox().getActiveHitboxWorldSpace();
    sf::FloatRect playerRect = player->getHitbox().getActiveHitboxWorldSpace();

    if(!enemyRect.intersects(playerRect)) {

        return;
    }

    player->getHit();
}
