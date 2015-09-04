#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "GameWorld.h"
#include "Player.h"
#include "Tile.h"
#include "TileMap.h"
#include "Gun.h"
#include "Camera.h"
#include "Enemy.h"
#include "EnemySpawners.h"
#include "Bullet.h"
#include "TurretEnemy.h"
#include "DestructibleBlock.h"
#include "SpatialHash.h"
#include "DataLoaders.h"
#include "CollisionResolution.h"
#include "PreloadedData.h"
#include "BackgroundManager.h"
#include "PowerUp.h"
#include "Random.h"
#include "WorldSavingLoading.h"

#include <functional>
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>
#include <unordered_set>
#include <cstdlib>

using std::function;
using std::unordered_set;
using std::remove_if;
using std::forward_iterator_tag;
using std::shared_ptr;
using std::make_shared;
using std::vector;

void loadDataCollection(PreloadedDataCollection &collection);
void handleWindowEvents(sf::RenderWindow &window, sf::Event &event, GameWorld &world);
void handleObjectEvents(sf::RenderWindow &window, sf::Event &event, GameWorld &world);
void handleObjectKeystate(sf::RenderWindow &window, GameWorld &world);
void updateWorld(sf::RenderWindow &window, GameWorld &world);
void drawWorld(sf::RenderWindow &window, GameWorld &world);
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
                                 function<void(shared_ptr<Player>, shared_ptr<Entity>)> collisionFunction);

template<class ShootingEntity>
void collidePlayerShootingEntities(shared_ptr<Player> player, vector<shared_ptr<ShootingEntity> > &entities,
                                 function<void(shared_ptr<Player>, shared_ptr<ShootingEntity>)> collisionFunction);

template<class DynamicObject>
void collideEntityDynamicObjectHash(shared_ptr<EntityBase> entity, SpatialHash<DynamicObject> &hash,
                                 function<void(shared_ptr<DynamicObject>, shared_ptr<EntityBase>)> collisionFunction);

template<class DynamicObject>
void collideShootingEntityDynamicObjectHash(shared_ptr<ShootingEntity> shooter, SpatialHash<DynamicObject> &hash,
                                 function<void(shared_ptr<DynamicObject>, shared_ptr<EntityBase>)> dynamicObjectEntityCollision,
                                 function<void(shared_ptr<Bullet>, shared_ptr<DynamicObject>)> bulletDynamicObjectCollision);

template<class B, class T>
void collideBulletsEntities(vector<shared_ptr<B> > &bullets, vector<shared_ptr<T> > &entities,
                                 function<void(shared_ptr<Bullet>, shared_ptr<T>)> collisionFunction);

template<class T>
void collideBulletEntities(shared_ptr<Bullet> bullet, vector<shared_ptr<T> > &entities,
                                 function<void(shared_ptr<Bullet>, shared_ptr<T>)> collisionFunction);

template<class DynamicObject>
void collideEntityDynamicObjects(shared_ptr<EntityBase> entity, unordered_set<shared_ptr<DynamicObject> > &objects,
                                 function<void(shared_ptr<DynamicObject>, shared_ptr<EntityBase>)> collisionFunction);

template<class DynamicObject>
void collideBulletDynamicObjects(shared_ptr<Bullet> bullet, unordered_set<shared_ptr<DynamicObject> > &objects,
                                 function<void(shared_ptr<Bullet>, shared_ptr<DynamicObject>)> collisionFunction);
template<class T>
void removeDeadEntities(vector<shared_ptr<T> > &entities);

template<class T>
void removeDeadHashEntries(vector<shared_ptr<T> > &entities, SpatialHash<T> &hash);

template<class T>
void updateObjectRendering(vector<shared_ptr<T> > &entities);

template<class T>
void drawEntities(sf::RenderWindow &window, vector<shared_ptr<T> > &entities);

function<void(shared_ptr<Bullet>, shared_ptr<DestructibleBlock>)> bulletBlockCollisionFunction = bulletNonDeflectingEntityCollision;
function<void(shared_ptr<Bullet>, shared_ptr<Enemy>)> bulletEnemyCollisionFunction = bulletNonDeflectingEntityCollision;
function<void(shared_ptr<Bullet>, shared_ptr<TurretEnemy>)> bulletTurretCollisionFunction = bulletNonDeflectingEntityCollision;
function<void(shared_ptr<Bullet>, shared_ptr<OmniDirectionalTurret>)> bulletOmnidirectionalTurretCollisionFunction = bulletNonDeflectingEntityCollision;
function<void(shared_ptr<Player>, shared_ptr<Enemy>)> playerEnemyCollisionFunction = playerEnemyEntityCollision;
function<void(shared_ptr<Player>, shared_ptr<PowerUp>)> playerPowerUpCollisionFunction = playerPowerUpCollision;
function<void(shared_ptr<Player>, shared_ptr<TurretEnemy>)> playerTurretCollisionFunction = playerEnemyEntityCollision;
function<void(shared_ptr<Player>, shared_ptr<OmniDirectionalTurret>)> playerOmnidirectionalTurretCollisionFunction = playerEnemyEntityCollision;
function<void(shared_ptr<DestructibleBlock>, shared_ptr<EntityBase>)> destructibleBlockEntityCollisionFunction = destructibleBlockEntityCollision;

//when an enemy dies theres a certain chance it will drop a power up
//these values determine the likelihood of a powerup being dropped
unsigned powerUpDropPossibleOutcomes = 100;
unsigned enemyPowerUpDropOccuranceThreshold = 7;
unsigned turretPowerUpDropOccuranceThreshold = 10;

void loadDataCollection(PreloadedDataCollection &collection) {

    loadBossData(collection.bossData, "scaling.txt");
    loadEnemyData(collection.goombaData, "goomba.txt");
    loadTurretData(collection.piranhaData, "piranha.txt");
    loadOmniDirectionalTurretData(collection.mushroomData, "mushroom.txt");
    loadBulletData(collection.slowBulletData, "slow");
    loadBulletData(collection.mediumBulletData, "medium");
    loadBulletData(collection.fastBulletData, "fast");
    loadDestrutibleBlockData(collection.basicDestructibleBlockData, "asdf");
    loadPowerUpData(collection.powerUpData, "powerups.txt");
}

void handleWindowEvents(sf::RenderWindow &window, sf::Event &event, GameWorld &world) {

	if(event.type == sf::Event::Closed) {

		window.close();
	}

	if(event.type == sf::Event::KeyPressed) {

		if(event.key.code == sf::Keyboard::Escape) {

			window.close();
		}
	}

	if(event.type == sf::Event::Resized) {

		world.camera.setupDefaultProperties(window);
	}
}

void handleObjectEvents(sf::RenderWindow &window, sf::Event &event, GameWorld &world) {

    if(world.worldState != GameWorld::TRANSITIONING_TO_BOSS_FIGHT) {

        for(unsigned i = 0; i < world.players.size(); ++i) {

            world.players[i]->handleInputEvents(event, window);
        }
    }

}

void handleObjectKeystate(sf::RenderWindow &window, GameWorld &world) {

    if(world.worldState != GameWorld::TRANSITIONING_TO_BOSS_FIGHT) {

        for(unsigned i = 0; i < world.players.size(); ++i) {

            world.players[i]->handleKeystate(window);
        }
    }

}

void updateWorld(sf::RenderWindow &window, GameWorld &world) {

    //determine current state of the gameworld
    if(world.worldState == GameWorld::NOT_FIGHTING_BOSS && world.shouldBeginBossFightTransition()) {

        world.beginBossFightTransition();
    }

    if(world.worldState == GameWorld::TRANSITIONING_TO_BOSS_FIGHT && world.shouldBeginBossFight()) {

        world.beginBossFight();
    }

    float deltaTime = world.updateTimer.restart().asSeconds();

	updateWorldPhyics(world, deltaTime);
	updateEnemySpawners(world, world.nonBossEnemySpawners);

	//don't update boss spawners unless player is actulaly fighting bosses
	if(world.worldState == GameWorld::FIGHTING_BOSS) {

        updateEnemySpawners(world, world.bossEnemySpawners, true);
	}

	handleEntityCollisions(world);
	updateWorldRendering(window, world);
}

void drawWorld(sf::RenderWindow &window, GameWorld &world) {

    world.backgrounds.draw(window);
	drawTiles(window, world);
	drawEntities(window, world.players);
	drawEnemyCollection(window, world.nonBossEnemyCollection);
	drawEnemyCollection(window, world.bossEnemyCollection);
	drawEntities(window, world.destructibleBlocks);
	drawEntities(window, world.powerUps);
}

void updateEnemySpawners(GameWorld &world, EnemySpawnerCollection &spawnerCollection, bool applyBossData) {

	spawnerCollection.enemySpawnInfo.currentCameraBounds = world.camera.getCameraBounds();
	spawnerCollection.turretSpawnInfo.currentCameraBounds = world.camera.getCameraBounds();
	spawnerCollection.omnidirectionalTurretSpawnInfo.currentCameraBounds = world.camera.getCameraBounds();

	spawnerCollection.enemySpawnInfo.worldBounds = world.worldBounds;
	spawnerCollection.turretSpawnInfo.worldBounds = world.worldBounds;
	spawnerCollection.omnidirectionalTurretSpawnInfo.worldBounds = world.worldBounds;

	spawnEnemyNearCamera(spawnerCollection.enemySpawnInfo, applyBossData);
	spawnEnemyNearCamera(spawnerCollection.turretSpawnInfo, applyBossData);
	spawnEnemyNearCamera(spawnerCollection.omnidirectionalTurretSpawnInfo, applyBossData);

	//delete any spawner that has no more enemies left to spawn
	///for now don't use this function
//	removeEmptySpawners(spawnerCollection.enemySpawnInfo.spawnPoints);
//	removeEmptySpawners(spawnerCollection.turretSpawnInfo.spawnPoints);
//	removeEmptySpawners(spawnerCollection.omnidirectionalTurretSpawnInfo.spawnPoints);
}

void removeEmptySpawners(vector<shared_ptr<SpawnPoint> > &spawnPoints) {

    for(unsigned i = 0; i < spawnPoints.size();) {

        if(spawnPoints[i]->getEnemiesLeftToSpawn() <= 0) {

            spawnPoints.erase(spawnPoints.begin() + i);
            continue;
        }

        ++i;
    }
}

void updateWorldPhyics(GameWorld &world, const float &deltaTime) {

	vector<glm::vec2> playerPositions;

	//have this loop up here so the player positions vector can be populated for hte camera
	for(auto &it : world.players) {

        if(it->checkIsAlive()) {

            playerPositions.push_back(it->getPosition());
        }
	}

	updateCameraProperties(world, playerPositions, deltaTime);

	if(world.worldState == GameWorld::TRANSITIONING_TO_BOSS_FIGHT) {

        //don't update any objects while transitioning to a boss fight
        return;
	}

	for(auto &it : world.players) {

		it->updatePhysics(deltaTime, world.worldBounds, world.tileMap);

		if(it->checkCanRespawn()) {

			it->respawn(world.camera.getCameraBounds());
		}

		playerPositions.push_back(it->getPosition());
	}

	updateEnemyCollectionPhysics(world.nonBossEnemyCollection, deltaTime, playerPositions, world);
	updateEnemyCollectionPhysics(world.bossEnemyCollection, deltaTime, playerPositions, world);

    removeDeadEntities(world.powerUps);

    removeDeadEntities(world.nonBossEnemyCollection.omnidirectionalTurrets);
	removeDeadEntities(world.nonBossEnemyCollection.enemies);
	removeDeadEntities(world.nonBossEnemyCollection.turrets);

	//kill bosses
	removeDeadEntities(world.bossEnemyCollection.omnidirectionalTurrets);
	removeDeadEntities(world.bossEnemyCollection.enemies);
	removeDeadEntities(world.bossEnemyCollection.turrets);

	removeDeadHashEntries(world.destructibleBlocks, world.destructibleBlockHash);
}

void updateCameraProperties(GameWorld &world, vector<glm::vec2> &playerPositions, const float &deltaTime) {

    ///camera should track a certain position depending on the current state of the gameworld
    ///during normal gameplay camera should track players

    ///if current gameplay state is a normal game play state, then
    if(world.worldState != GameWorld::TRANSITIONING_TO_BOSS_FIGHT) {

        world.camera.calculateProperties(playerPositions);
        world.camera.update(deltaTime, world.worldBounds);

    } else {

        //transitioing to boss fight, make the camera move to default view size
        //and towards the center of the boss world bounds
        //so that when the world size is forcibly set to the boss world bounds
        //the camera doesn't just jump from one position and size to another
        glm::vec2 cameraTargetPosition(world.worldBoundsBossFight.left + world.worldBounds.width / 2,
                                       world.worldBounds.top + world.worldBounds.height / 2);

        world.camera.setTargetSize(world.camera.getDefaultSize());
        world.camera.setTargetPosition(cameraTargetPosition);

        //slow down the speed that the camera transitions at for a cooler effect
        world.camera.update(deltaTime / 2.5f, world.worldBounds);
    }
}

void updateEnemyCollectionPhysics(EnemyCollection &collection, const float &delta, vector<glm::vec2> &playerPositions, GameWorld &world) {

    for(unsigned i = 0; i < collection.enemies.size(); ++i) {

		collection.enemies[i]->updatePhysics(delta, world.worldBounds, world.tileMap);

		if(!collection.enemies[i]->checkIsAlive()) {

            spawnPowerUp(enemyPowerUpDropOccuranceThreshold, powerUpDropPossibleOutcomes, collection.enemies[i]->getPosition(), world);
		}
	}

	for(unsigned i = 0; i < collection.turrets.size(); ++i) {

		collection.turrets[i]->updatePhysics(delta, world.worldBounds, world.tileMap, playerPositions);

		if(!collection.turrets[i]->checkIsAlive()) {

            spawnPowerUp(turretPowerUpDropOccuranceThreshold, powerUpDropPossibleOutcomes, collection.turrets[i]->getPosition(), world);
		}
	}

	for(unsigned i = 0; i < collection.omnidirectionalTurrets.size(); ++i) {

		collection.omnidirectionalTurrets[i]->updatePhysics(delta, world.worldBounds, world.tileMap);

		if(!collection.omnidirectionalTurrets[i]->checkIsAlive()) {

            spawnPowerUp(turretPowerUpDropOccuranceThreshold, powerUpDropPossibleOutcomes, collection.omnidirectionalTurrets[i]->getPosition(), world);
		}
	}
}

void updateWorldRendering(sf::RenderWindow &window, GameWorld &world) {

	world.camera.applyCamera(window);

	sf::Vector2f viewOffset = world.camera.getViewTopLeft() - world.viewPositionLastFrame;
	world.viewPositionLastFrame = world.camera.getViewTopLeft();

    //don't allow any entity to animate while world is transitioing into boss battle
    if(world.worldState == GameWorld::TRANSITIONING_TO_BOSS_FIGHT) {

        return;
    }

	updateObjectRendering(world.players);
	updateObjectRendering(world.destructibleBlocks);
	updateObjectRendering(world.powerUps);
	updateEnemyCollectionRendering(window, world.nonBossEnemyCollection);
	updateEnemyCollectionRendering(window, world.bossEnemyCollection);

	world.backgrounds.updateRendering(viewOffset);
}

void updateEnemyCollectionRendering(sf::RenderWindow &window, EnemyCollection &collection) {

    updateObjectRendering(collection.enemies);
	updateObjectRendering(collection.turrets);
	updateObjectRendering(collection.omnidirectionalTurrets);
}

void drawEnemyCollection(sf::RenderWindow &window, EnemyCollection &collection) {

    drawEntities(window, collection.enemies);
	drawEntities(window, collection.turrets);
	drawEntities(window, collection.omnidirectionalTurrets);
}

void spawnPowerUp(const unsigned &occuranceThreshold, const unsigned &possibleOutcomes, const glm::vec2 &position, GameWorld &world) {

    bool shouldSpawn = determineIfEventOccurs(occuranceThreshold, possibleOutcomes);

    if(!shouldSpawn) {

        return;
    }

    //randomly determine the type of powerup to give
    int powerUpType = getRand(PowerUpType::MACHINE_GUN, PowerUpType::MACHINE_GUN);
    PreloadedPowerUpData *powerUpData = &dataCollection.powerUpData;

    switch(powerUpType) {

        case PowerUpType::MACHINE_GUN: {

            powerUpData = &dataCollection.powerUpData;
            break;
        }

        default: {

            powerUpData = &dataCollection.powerUpData;
        }
    }

    shared_ptr<PowerUp> powerUp = make_shared<PowerUp>(position, (PowerUpType)powerUpType, *powerUpData);
    world.powerUps.push_back(powerUp);
}

void handleEntityCollisions(GameWorld &world) {

	playerWorldCollision(world);
	handleEnemyCollectionWorldCollision(world, world.nonBossEnemyCollection);

	if(world.worldState == GameWorld::FIGHTING_BOSS) {

        handleEnemyCollectionWorldCollision(world, world.bossEnemyCollection);
	}
}

void handleEnemyCollectionWorldCollision(GameWorld &world, EnemyCollection &enemyCollection) {

    enemyWorldCollision(world, enemyCollection);
	turretWorldCollision(world, enemyCollection);
	omnidirectionalTurretWorldCollision(world, enemyCollection);
}

void playerWorldCollision(GameWorld &world) {

	//collision with players against rest of world
	for(auto &it : world.players) {

		collideShootingEntityDynamicObjectHash(it, world.destructibleBlockHash, destructibleBlockEntityCollisionFunction, bulletBlockCollisionFunction);

		collidePlayerEntities(it, world.powerUps, playerPowerUpCollisionFunction);

		playerEnemyCollectionCollision(it, world, world.nonBossEnemyCollection);

		if(world.worldState == GameWorld::FIGHTING_BOSS) {

            playerEnemyCollectionCollision(it, world, world.bossEnemyCollection);
		}
	}
}

void playerEnemyCollectionCollision(shared_ptr<Player> &player, GameWorld &world, EnemyCollection &enemyCollection) {

    collidePlayerEntities(player, enemyCollection.enemies, playerEnemyCollisionFunction);
    collidePlayerShootingEntities(player, enemyCollection.turrets, playerTurretCollisionFunction);
    collidePlayerShootingEntities(player, enemyCollection.omnidirectionalTurrets, playerOmnidirectionalTurretCollisionFunction);

    //handle collision with player's bullets and enemies
    collideBulletsEntities(player->getGun()->getBullets(), enemyCollection.enemies, bulletEnemyCollisionFunction);
    collideBulletsEntities(player->getGun()->getBullets(), enemyCollection.turrets, bulletTurretCollisionFunction);
    collideBulletsEntities(player->getGun()->getBullets(), enemyCollection.omnidirectionalTurrets, bulletOmnidirectionalTurretCollisionFunction);
}

void enemyWorldCollision(GameWorld &world, EnemyCollection &enemyCollection) {

	for(auto &it : enemyCollection.enemies) {

		collideEntityDynamicObjectHash(it, world.destructibleBlockHash, destructibleBlockEntityCollisionFunction);
	}
}

void turretWorldCollision(GameWorld &world, EnemyCollection &enemyCollection) {

	for(auto &it : enemyCollection.turrets) {

		collideShootingEntityDynamicObjectHash(it, world.destructibleBlockHash, destructibleBlockEntityCollisionFunction, bulletBlockCollisionFunction);
	}
}

void omnidirectionalTurretWorldCollision(GameWorld &world, EnemyCollection &enemyCollection) {

    for(auto &it : enemyCollection.omnidirectionalTurrets) {

        collideShootingEntityDynamicObjectHash(it, world.destructibleBlockHash, destructibleBlockEntityCollisionFunction, bulletBlockCollisionFunction);
    }
}

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

void drawTiles(sf::RenderWindow &window, GameWorld &world) {

	sf::FloatRect cameraBounds = world.camera.getCameraBounds();

	glm::vec2 topLeft(cameraBounds.left, cameraBounds.top);
	glm::vec2 bottomRight(cameraBounds.left + cameraBounds.width, cameraBounds.top + cameraBounds.height);

	world.tileMap.draw(window, topLeft, bottomRight);
}

template<class T>
void drawEntities(sf::RenderWindow &window, vector<shared_ptr<T> > &entities) {

	for(unsigned i = 0; i < entities.size(); ++i) {

		entities[i]->draw(window);
	}
}

int main() {

    srand((long)time(0));

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Contra");

    sf::Event event;

    loadDataCollection(dataCollection);

    GameWorld world(window);
    world.worldBounds = sf::FloatRect(0, 0, 2048 + 1024, 768);
    world.worldBoundsBossFight = sf::FloatRect(0, 0, 1028 + 1028, 768);
    world.tileMap.resize(world.worldBounds.width, world.worldBounds.height);
    world.players.push_back(make_shared<Player>());
    PreloadedPlayerData data;
    loadPlayerData(data, "player.txt");
    world.players[0]->load(data);

    loadWorld("world1", world);

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            handleWindowEvents(window, event, world);
            handleObjectEvents(window, event, world);

            if(event.type == sf::Event::MouseButtonPressed) {

                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());

                if(event.mouseButton.button == sf::Mouse::Left) {

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {

                        world.tileMap.setTileDisplayChanges(mousePosition, TileType::UPWARD_RIGHT_1_1, "tileset.png", sf::IntRect(0, 0, 64, 64));

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {

                        world.tileMap.setTileDisplayChanges(mousePosition, TileType::UPWARD_LEFT_1_1, "tileset.png", sf::IntRect(0, 0, 64, 64));

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {

                        world.tileMap.setTileDisplayChanges(mousePosition, TileType::ONE_WAY, "tileset.png", sf::IntRect(0, 0, 64, 64));

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {

                        shared_ptr<SpawnPoint> point = make_shared<SpawnPoint>(mousePosition, sf::seconds(0.6), EnemyType::ENEMY_MUSHROOM, 1);
                        world.bossEnemySpawners.omnidirectionalTurretSpawnInfo.spawnPoints.push_back(point);

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {

                        //snap the block to the grid
                        glm::vec2 position(mousePosition.x, mousePosition.y);
                        position /= static_cast<float>(TILE_SIZE);
                        position.x = glm::floor(position.x);
                        position.y = glm::floor(position.y);

                        position *= static_cast<float>(TILE_SIZE);

                        shared_ptr<DestructibleBlock> block = make_shared<DestructibleBlock>(position, dataCollection.basicDestructibleBlockData);
                        world.destructibleBlocks.push_back(block);
                        world.destructibleBlockHash.insert(block);

                    } else {

                        world.tileMap.setTileDisplayChanges(mousePosition, TileType::SOLID, "tileset.png", sf::IntRect(0, 0, 64, 64));
                    }

                }

                if(event.mouseButton.button == sf::Mouse::Right) {

                    world.tileMap.setTileDisplayChanges(mousePosition, TileType::EMPTY, "tileset.png", sf::IntRect(0, 0, 0, 0));
                }
            }

        }

        handleObjectKeystate(window, world);

        updateWorld(window, world);

        window.clear();

        drawWorld(window, world);

        window.display();

    }

    saveWorld("world1", world);
    return 0;
}
