#include "PlayingLevelFunctions.h"
#include "CollisionResolution.h"
#include "Player.h"
#include "EntityBase.h"
#include "OmniDirectionalTurret.h"
#include "Bullet.h"
#include "DestructibleBlock.h"
#include "TurretEnemy.h"
#include "EnemySpawners.h"
#include "Random.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;

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
unsigned enemyPowerUpDropOccuranceThreshold = 100;
unsigned turretPowerUpDropOccuranceThreshold = 10;

void updateEnemySpawners(GameWorld &world, EnemySpawnerCollection &spawnerCollection, bool applyBossData) {

	spawnerCollection.enemySpawnInfo.currentCameraBounds = world.camera.getCameraBounds();
	spawnerCollection.turretSpawnInfo.currentCameraBounds = world.camera.getCameraBounds();
	spawnerCollection.omnidirectionalTurretSpawnInfo.currentCameraBounds = world.camera.getCameraBounds();

	spawnerCollection.enemySpawnInfo.worldBounds = world.worldBoundsInUse;
	spawnerCollection.turretSpawnInfo.worldBounds = world.worldBoundsInUse;
	spawnerCollection.omnidirectionalTurretSpawnInfo.worldBounds = world.worldBoundsInUse;

	spawnEnemyNearCamera(spawnerCollection.enemySpawnInfo, applyBossData);
	spawnEnemyNearCamera(spawnerCollection.turretSpawnInfo, applyBossData);
	spawnEnemyNearCamera(spawnerCollection.omnidirectionalTurretSpawnInfo, applyBossData);

	//delete any spawner that has no more enemies left to spawn
	removeEmptySpawners(spawnerCollection.enemySpawnInfo.spawnPoints);
	removeEmptySpawners(spawnerCollection.turretSpawnInfo.spawnPoints);
	removeEmptySpawners(spawnerCollection.omnidirectionalTurretSpawnInfo.spawnPoints);
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

		it->updatePhysics(deltaTime, world.worldBoundsInUse, world.tileMap);

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

    ///for saving and loading testing don't remove destructible blocks
	///removeDeadHashEntries(world.destructibleBlocks, world.destructibleBlockHash);
}

void updateCameraProperties(GameWorld &world, vector<glm::vec2> &playerPositions, const float &deltaTime) {

    ///camera should track a certain position depending on the current state of the gameworld
    ///during normal gameplay camera should track players

    ///if current gameplay state is a normal game play state, then
    if(world.worldState != GameWorld::TRANSITIONING_TO_BOSS_FIGHT) {

        world.camera.calculateProperties(playerPositions);
        world.camera.update(deltaTime, world.worldBoundsInUse);

    } else {

        //transitioing to boss fight, make the camera move to default view size
        //and towards the center of the boss world bounds
        //so that when the world size is forcibly set to the boss world bounds
        //the camera doesn't just jump from one position and size to another
        glm::vec2 cameraTargetPosition(world.worldBoundsBossFight.left + world.worldBoundsBossFight.width / 2,
                                       world.worldBoundsBossFight.top + world.worldBoundsBossFight.height / 2);

        world.camera.setTargetSize(world.camera.getDefaultSize());
        world.camera.setTargetPosition(cameraTargetPosition);

        //slow down the speed that the camera transitions at for a cooler effect
        //use use the default world bounds for the camera's bounds because while the camera is transitioing to boss fight world bounds, the camera isn't inside the bossfight bounds
        //so if you use the boss fight bounds, camera will snap to the inside of the boss fight
        world.camera.update(deltaTime / 2.5f, world.worldBoundsDefault);
    }
}

void updateEnemyCollectionPhysics(EnemyCollection &collection, const float &delta, vector<glm::vec2> &playerPositions, GameWorld &world) {

    for(unsigned i = 0; i < collection.enemies.size(); ++i) {

		collection.enemies[i]->updatePhysics(delta, world.worldBoundsInUse, world.tileMap);

		if(!collection.enemies[i]->checkIsAlive()) {

            spawnPowerUp(enemyPowerUpDropOccuranceThreshold, powerUpDropPossibleOutcomes, collection.enemies[i]->getPosition(), world);
		}
	}

	for(unsigned i = 0; i < collection.turrets.size(); ++i) {

		collection.turrets[i]->updatePhysics(delta, world.worldBoundsInUse, world.tileMap, playerPositions);

		if(!collection.turrets[i]->checkIsAlive()) {

            spawnPowerUp(turretPowerUpDropOccuranceThreshold, powerUpDropPossibleOutcomes, collection.turrets[i]->getPosition(), world);
		}
	}

	for(unsigned i = 0; i < collection.omnidirectionalTurrets.size(); ++i) {

		collection.omnidirectionalTurrets[i]->updatePhysics(delta, world.worldBoundsInUse, world.tileMap);

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
    auto powerUpData = dataCollection.getPowerUpData((PowerUpType)powerUpType);

    if(!powerUpData) {

        return;
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

void drawTiles(sf::RenderWindow &window, GameWorld &world) {

	sf::FloatRect cameraBounds = world.camera.getCameraBounds();

	glm::vec2 topLeft(cameraBounds.left, cameraBounds.top);
	glm::vec2 bottomRight(cameraBounds.left + cameraBounds.width, cameraBounds.top + cameraBounds.height);

	world.tileMap.drawTilesDebug(window, topLeft, bottomRight);
}
