#ifndef GAMEWORLD_H_INCLUDED
#define GAMEWORLD_H_INCLUDED

#include "Enemy.h"
#include "TurretEnemy.h"
#include "OmniDirectionalTurret.h"
#include "Player.h"
#include "PowerUp.h"
#include "DestructibleBlock.h"
#include "Camera.h"
#include "SpatialHash.h"
#include "BackgroundManager.h"
#include "SpawnerData.h"
#include "PreloadedData.h"

#include <iostream>
#include <vector>
#include <memory>
#include <string>

using std::cout;
using std::endl;

class GameConfiguration;

struct EnemyCollection {

    EnemyCollection() :
        enemies(),
        turrets(),
        omnidirectionalTurrets()
        {

        }

    std::vector<std::shared_ptr<Enemy> > enemies;
	std::vector<std::shared_ptr<TurretEnemy> > turrets;
	std::vector<std::shared_ptr<OmniDirectionalTurret> > omnidirectionalTurrets;

	void clear() {

        enemies.clear();
        turrets.clear();
        omnidirectionalTurrets.clear();
	}
};

struct EnemySpawnerCollection {

    EnemySpawnerCollection(EnemyCollection &collection, sf::FloatRect &worldBounds) :
        enemySpawnInfo(collection.enemies, worldBounds, worldBounds),
        turretSpawnInfo(collection.turrets, worldBounds, worldBounds),
        omnidirectionalTurretSpawnInfo(collection.omnidirectionalTurrets, worldBounds, worldBounds)
        {

        }

    void clearSpawnPoints() {

        enemySpawnInfo.spawnPoints.clear();
        turretSpawnInfo.spawnPoints.clear();
        omnidirectionalTurretSpawnInfo.spawnPoints.clear();
    }

    InformationForSpawner<Enemy> enemySpawnInfo;
	InformationForSpawner<TurretEnemy> turretSpawnInfo;
	InformationForSpawner<OmniDirectionalTurret> omnidirectionalTurretSpawnInfo;
};

struct GameWorld {

	GameWorld(sf::RenderWindow &window) :
	    worldState(NOT_FIGHTING_BOSS),
	    nameOfPrecedingLevel(),
	    initialPlayerSpawnPoint(0, 0),
		players(),
		powerUps(),
		nonBossEnemyCollection(),
		bossEnemyCollection(),
		tileMap(),
		destructibleBlocks(),
		backgrounds(),
		camera(window),
		viewPositionLastFrame(0, 0),
		worldBoundsDefault(0, 0, 0, 0),
		worldBoundsBossFight(0, 0, 0, 0),
		worldBoundsInUse(0, 0, 0, 0),
		nonBossEnemySpawners(nonBossEnemyCollection, worldBoundsInUse),
		bossEnemySpawners(bossEnemyCollection, worldBoundsInUse),
		destructibleBlockHash(256, 256),
		updateTimer()
		{

		}

    //state of the current gameworld in terms of whether or not playeri s fighting bosses now
    enum WorldBossBattleState {

        NOT_FIGHTING_BOSS,
        TRANSITIONING_TO_BOSS_FIGHT,
        FIGHTING_BOSS,

    } worldState;

    std::string nameOfPrecedingLevel;

    glm::vec2 initialPlayerSpawnPoint;

	//entities
	std::vector<std::shared_ptr<Player> > players;
	std::vector<std::shared_ptr<PowerUp> > powerUps;
	EnemyCollection nonBossEnemyCollection; //collection of non boss type enemies
	EnemyCollection bossEnemyCollection; //collection of boss type enemies

	//environment
	TileMap tileMap;
	std::vector<std::shared_ptr<DestructibleBlock> > destructibleBlocks;
	BackgroundManager backgrounds;

	//camera properties
	Camera camera;
	sf::Vector2f viewPositionLastFrame;

	//world bounds for normal gameplay
	sf::FloatRect worldBoundsDefault;

	//world bounds for when the player is fighting a boss
	//this is different from normal world bounds so that the player can't go offscreen while the boss is there
	sf::FloatRect worldBoundsBossFight;

	//world bounds currently being used
	//this is the bounds used during gameplay, the other saved world bounds are only for saving and loading
	sf::FloatRect worldBoundsInUse;

	//spawner properties for non boss enemy types
    EnemySpawnerCollection nonBossEnemySpawners;

    //spawners for boss
    EnemySpawnerCollection bossEnemySpawners;

	//other stuff
	SpatialHash<DestructibleBlock> destructibleBlockHash;
	sf::Clock updateTimer;

	void clearEverythingButPlayers() {

        powerUps.clear();
        tileMap.resize(0, 0);
        destructibleBlocks.clear();
        backgrounds.clearBackgrounds();
        nonBossEnemySpawners.clearSpawnPoints();
        bossEnemySpawners.clearSpawnPoints();
        nonBossEnemyCollection.clear();
        bossEnemyCollection.clear();
        destructibleBlockHash.clear();
        destructibleBlocks.clear();
	}

	void clearEverything() {

        players.clear();
        clearEverythingButPlayers();
	}

	bool shouldBeginBossFightTransition() {

        //if any player isn't isn't in the boss world bounds then don't transition
        for(auto &player : players) {

            sf::FloatRect playerBounds = player->getHurtbox().getActiveHitboxWorldSpace();

            if(!(playerBounds.left >= worldBoundsBossFight.left && playerBounds.left + playerBounds.width <= worldBoundsBossFight.left + worldBoundsBossFight.width &&
               playerBounds.top >= worldBoundsBossFight.top && playerBounds.top + playerBounds.height <= worldBoundsBossFight.top + worldBoundsBossFight.height)) {

                    return false;
            }

        }

        //don't return true if there are no players
        return players.size() > 0;
	}

	void beginBossFightTransition() {

        worldState = TRANSITIONING_TO_BOSS_FIGHT;
	}

    //boss fight begins once the camera is positioned so that it is entierely inside the boss fight region
	bool shouldBeginBossFight() {

        bool cameraWithinBossFightBounds = false;

        sf::FloatRect cameraBounds = camera.getCameraBounds();

        if(cameraBounds.left >= worldBoundsBossFight.left && cameraBounds.left + cameraBounds.width <= worldBoundsBossFight.left + worldBoundsBossFight.width &&
           cameraBounds.top >= worldBoundsBossFight.top && cameraBounds.top + cameraBounds.height <= worldBoundsBossFight.height) {

            cameraWithinBossFightBounds = true;
        }

        return cameraWithinBossFightBounds;
	}

	void beginBossFight() {

        worldState = FIGHTING_BOSS;
        worldBoundsInUse = worldBoundsBossFight;
	}
};

std::shared_ptr<Player> createPlayer(const glm::vec2 &spawnPosition, const PlayerNumber &number, GameConfiguration &gameConfig);
bool canSpawnEnemy(EnemySpawnerCollection &spawnerCollection);
bool enemiesRemaining(EnemyCollection &enemyCollection);

#endif // GAMEWORLD_H_INCLUDED
