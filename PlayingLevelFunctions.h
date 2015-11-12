#ifndef PLAYINGLEVELFUNCTIONS_H_INCLUDED
#define PLAYINGLEVELFUNCTIONS_H_INCLUDED

#include <vector>
#include <memory>
#include <functional>
#include <unordered_set>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include "SpatialHash.h"
#include "Bullet.h"
#include "ShootingEntity.h"
#include "GameWorld.h"

using std::unordered_set;
using std::function;
using std::vector;
using std::shared_ptr;

class TurretEnemy;
class Enemy;
class OmniDirectionalTurret;
class Player;
class DestructibleBlock;
class PowerUp;
class EnemySpawnerCollection;
class SpawnPoint;
class EnemyCollection;
class EntityBase;

void updateEnemySpawners(GameWorld &world, EnemySpawnerCollection &spawnerCollection, bool applyBossData = false); //last parameter determines whether or not enemy is a boss and scaling should be applied
void removeEmptySpawners(vector<shared_ptr<SpawnPoint> > &spawnPoints);
void updateWorldPhyics(GameWorld &world, const float &deltaTime);
void updateCameraProperties(GameWorld &world, vector<glm::vec2> &playerPositions, const float &deltaTime);
void updateEnemyCollectionPhysics(EnemyCollection &collection, const float &delta, vector<glm::vec2> &playerPositions, GameWorld &world);
void updateWorldRendering(sf::RenderWindow &window, GameWorld &world);
void updateEnemyCollectionRendering(sf::RenderWindow &window, EnemyCollection &collection);
void drawEnemyCollection(sf::RenderWindow &window, EnemyCollection &collection);
void spawnPowerUp(const unsigned &occuranceThreshold, const unsigned &possibleOutcomes, const glm::vec2 &position, GameWorld &world);
void handleEntityCollisions(GameWorld &world);
void handleEnemyCollectionWorldCollision(GameWorld &world, EnemyCollection &enemyCollection);
void playerWorldCollision(GameWorld &world);
void playerEnemyCollectionCollision(shared_ptr<Player> &player, GameWorld &world, EnemyCollection &enemyCollection);
void enemyWorldCollision(GameWorld &world, EnemyCollection &enemyCollection);
void turretWorldCollision(GameWorld &world, EnemyCollection &enemyCollection);
void omnidirectionalTurretWorldCollision(GameWorld &world, EnemyCollection &enemyCollection);
void drawTiles(sf::RenderWindow &window, GameWorld &world);

template<class Entity>
void collidePlayerEntities(shared_ptr<Player> player, vector<shared_ptr<Entity> > &entities,
                                 function<void(shared_ptr<Player>, shared_ptr<Entity>)> collisionFunction) {

	for(unsigned i = 0; i < entities.size(); ++i) {

		collisionFunction(player, entities[i]);
	}
}

template<class ShootingEntity>
void collidePlayerShootingEntities(shared_ptr<Player> player, vector<shared_ptr<ShootingEntity> > &entities,
                                 function<void(shared_ptr<Player>, shared_ptr<ShootingEntity>)> collisionFunction) {

	for(unsigned i = 0; i < entities.size(); ++i) {

		if(!entities[i]->checkIsAlive()) {

			continue;
		}

		collisionFunction(player, entities[i]);

		auto bullets = entities[i]->getGun()->getBullets();

		for(auto &it : bullets) {

			bulletNonDeflectingEntityCollision(it, player);
		}
	}
}

template<class DynamicObject>
void collideEntityDynamicObjectHash(shared_ptr<EntityBase> entity, SpatialHash<DynamicObject> &hash,
                                 function<void(shared_ptr<DynamicObject>, shared_ptr<EntityBase>)> collisionFunction) {

	sf::FloatRect hurtbox = entity->getHitbox().getActiveHitboxWorldSpace();
	auto blocks = hash.getSurroundingEntites(hurtbox);

	collideEntityDynamicObjects(entity, blocks, collisionFunction);
}

template<class DynamicObject>
void collideShootingEntityDynamicObjectHash(shared_ptr<ShootingEntity> shooter, SpatialHash<DynamicObject> &hash,
                                 function<void(shared_ptr<DynamicObject>, shared_ptr<EntityBase>)> dynamicObjectEntityCollision,
                                 function<void(shared_ptr<Bullet>, shared_ptr<DynamicObject>)> bulletDynamicObjectCollision) {

	collideEntityDynamicObjectHash(shooter, hash, dynamicObjectEntityCollision);

	auto bullets = shooter->getGun()->getBullets();

	for(auto &it : bullets) {

		sf::FloatRect bulletHitbox = it->getHitbox().getActiveHitboxWorldSpace();
		auto collidingWithBullet = hash.getSurroundingEntites(bulletHitbox);

		collideBulletDynamicObjects(it, collidingWithBullet, bulletDynamicObjectCollision);
	}
}

template<class B, class T>
void collideBulletsEntities(vector<shared_ptr<B> > &bullets, vector<shared_ptr<T> > &entities,
                                 function<void(shared_ptr<Bullet>, shared_ptr<T>)> collisionFunction) {

    for(auto &bullet : bullets) {

        collideBulletEntities(bullet, entities, collisionFunction);
    }
}

template<class T>
void collideBulletEntities(shared_ptr<Bullet> bullet, vector<shared_ptr<T> > &entities,
                                 function<void(shared_ptr<Bullet>, shared_ptr<T>)> collisionFunction) {

	for(auto &it : entities) {

		collisionFunction(bullet, it);
	}
}

template<class DynamicObject>
void collideEntityDynamicObjects(shared_ptr<EntityBase> entity, unordered_set<shared_ptr<DynamicObject> > &objects,
                                 function<void(shared_ptr<DynamicObject>, shared_ptr<EntityBase>)> collisionFunction) {

	for(auto &it : objects) {

		collisionFunction(it, entity);
	}
}

template<class DynamicObject>
void collideBulletDynamicObjects(shared_ptr<Bullet> bullet, unordered_set<shared_ptr<DynamicObject> > &objects,
                                 function<void(shared_ptr<Bullet>, shared_ptr<DynamicObject>)> collisionFunction) {

	for(auto &it : objects) {

		collisionFunction(bullet, it);
	}
}

template<class T>
void removeDeadEntities(vector<shared_ptr<T> > &entities) {

	for(unsigned i = 0; i < entities.size();) {

        if(!entities[i]->checkIsAlive()) {

            entities.erase(entities.begin() + i);
            continue;
        }

        ++i;
	}
}

template<class T>
void removeDeadHashEntries(vector<shared_ptr<T> > &entities, SpatialHash<T> &hash) {

	for(unsigned i = 0; i < entities.size();) {

        if(!entities[i]->checkIsAlive()) {

            hash.remove(entities[i]);
            entities.erase(entities.begin() + i);
            continue;
        }

        ++i;
	}
}

template<class T>
void updateObjectRendering(vector<shared_ptr<T> > &entities) {

	for(unsigned i = 0; i < entities.size(); ++i) {

		entities[i]->updateRendering();
	}
}

template<class T>
void drawEntities(sf::RenderWindow &window, vector<shared_ptr<T> > &entities) {

	for(unsigned i = 0; i < entities.size(); ++i) {

		entities[i]->draw(window);
	}
}

extern unsigned powerUpDropPossibleOutcomes;
extern unsigned enemyPowerUpDropOccuranceThreshold;
extern unsigned turretPowerUpDropOccuranceThreshold;

extern function<void(shared_ptr<Bullet>, shared_ptr<DestructibleBlock>)> bulletBlockCollisionFunction;
extern function<void(shared_ptr<Bullet>, shared_ptr<Enemy>)> bulletEnemyCollisionFunction;
extern function<void(shared_ptr<Bullet>, shared_ptr<TurretEnemy>)> bulletTurretCollisionFunction;
extern function<void(shared_ptr<Bullet>, shared_ptr<OmniDirectionalTurret>)> bulletOmnidirectionalTurretCollisionFunction;
extern function<void(shared_ptr<Player>, shared_ptr<Enemy>)> playerEnemyCollisionFunction;
extern function<void(shared_ptr<Player>, shared_ptr<PowerUp>)> playerPowerUpCollisionFunction;
extern function<void(shared_ptr<Player>, shared_ptr<TurretEnemy>)> playerTurretCollisionFunction;
extern function<void(shared_ptr<Player>, shared_ptr<OmniDirectionalTurret>)> playerOmnidirectionalTurretCollisionFunction;
extern function<void(shared_ptr<DestructibleBlock>, shared_ptr<EntityBase>)> destructibleBlockEntityCollisionFunction;


#endif // PLAYINGLEVELFUNCTIONS_H_INCLUDED
