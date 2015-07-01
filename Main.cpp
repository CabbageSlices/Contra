#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
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

#include <functional>
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>
#include <unordered_set>

using std::function;
using std::unordered_set;
using std::remove_if;
using std::forward_iterator_tag;
using std::shared_ptr;
using std::make_shared;
using std::vector;

struct GameWorld;

void loadDataCollection(PreloadedDataCollection &collection);
void handleWindowEvents(sf::RenderWindow &window, sf::Event &event, GameWorld &world);
void handleObjectEvents(sf::RenderWindow &window, sf::Event &event, GameWorld &world);
void handleObjectKeystate(sf::RenderWindow &window, GameWorld &world);
void updateWorld(sf::RenderWindow &window, GameWorld &world);
void drawWorld(sf::RenderWindow &window, GameWorld &world);
void updateEnemySpawners(GameWorld &world);
void updateWorldPhyics(GameWorld &world, const float &deltaTime);
void updateWorldRendering(sf::RenderWindow &window, GameWorld &world);
void handleEntityCollisions(GameWorld &world);
void playerWorldCollision(GameWorld &world);
void enemyWorldCollision(GameWorld &world);
void turretWorldCollision(GameWorld &world);
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
function<void(shared_ptr<Bullet>, shared_ptr<Enemy>)> bulletEnemyCollisionFunction= bulletNonDeflectingEntityCollision;
function<void(shared_ptr<Bullet>, shared_ptr<TurretEnemy>)> bulletTurretCollisionFunction= bulletNonDeflectingEntityCollision;
function<void(shared_ptr<Player>, shared_ptr<Enemy>)> playerEnemyCollisionFunction = playerEnemyEntityCollision;
function<void(shared_ptr<Player>, shared_ptr<PowerUp>)> playerPowerUpCollisionFunction = playerPowerUpCollision;
function<void(shared_ptr<Player>, shared_ptr<TurretEnemy>)> playerTurretCollisionFunction = playerEnemyEntityCollision;
function<void(shared_ptr<DestructibleBlock>, shared_ptr<EntityBase>)> destructibleBlockEntityCollisionFunction = destructibleBlockEntityCollision;

struct GameWorld {

	GameWorld(sf::RenderWindow &window) :
		players(),
		enemies(),
		turrets(),
		powerUps(),
		tileMap(),
		destructibleBlocks(),
		backgrounds(),
		worldBounds(0, 0, 0, 0),
		camera(window),
		viewPositionLastFrame(0, 0),
		enemySpawnInfo(enemies, worldBounds, worldBounds),
		turretSpawnInfo(turrets, worldBounds, worldBounds),
		destructibleBlockHash(256, 256),
		updateTimer()
		{

		}

	//entities
	vector<shared_ptr<Player> > players;
	vector<shared_ptr<Enemy> > enemies;
	vector<shared_ptr<TurretEnemy> > turrets;
	vector<shared_ptr<PowerUp> > powerUps;

	//environment
	TileMap tileMap;
	vector<shared_ptr<DestructibleBlock> > destructibleBlocks;
	BackgroundManager backgrounds;

	//world properties
	sf::FloatRect worldBounds;
	Camera camera;
	sf::Vector2f viewPositionLastFrame;

	//spawner properties
	InformationForSpawner<Enemy> enemySpawnInfo;
	InformationForSpawner<TurretEnemy> turretSpawnInfo;

	//other stuff
	SpatialHash<DestructibleBlock> destructibleBlockHash;
	sf::Clock updateTimer;
};

void loadDataCollection(PreloadedDataCollection &collection) {

    loadEnemyData(collection.goombaData, "goomba.txt");
    loadTurretData(collection.piranhaData, "piranha.txt");
    loadBulletData(collection.slowBulletData, "slow");
    loadBulletData(collection.mediumBulletData, "medium");
    loadBulletData(collection.fastBulletData, "fast");
    loadDestrutibleBlockData(collection.basicDestructibleBlockData, "asdf");
    loadPowerUpData(collection.powerupData, "powerups.txt");
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

	for(unsigned i = 0; i < world.players.size(); ++i) {

		world.players[i]->handleInputEvents(event, window);
	}
}

void handleObjectKeystate(sf::RenderWindow &window, GameWorld &world) {

	for(unsigned i = 0; i < world.players.size(); ++i) {

		world.players[i]->handleKeystate(window);
	}
}

void updateWorld(sf::RenderWindow &window, GameWorld &world) {

	float deltaTime = world.updateTimer.restart().asSeconds();

	updateWorldPhyics(world, deltaTime);
	handleEntityCollisions(world);
	updateWorldRendering(window, world);

	updateEnemySpawners(world);
}

void drawWorld(sf::RenderWindow &window, GameWorld &world) {

    world.backgrounds.draw(window);
	drawTiles(window, world);
	drawEntities(window, world.players);
	drawEntities(window, world.enemies);
	drawEntities(window, world.turrets);
	drawEntities(window, world.destructibleBlocks);
	drawEntities(window, world.powerUps);
}

void updateEnemySpawners(GameWorld &world) {

	world.enemySpawnInfo.currentCameraBounds = world.camera.getCameraBounds();
	world.turretSpawnInfo.currentCameraBounds = world.camera.getCameraBounds();

	world.enemySpawnInfo.worldBounds = world.worldBounds;
	world.turretSpawnInfo.worldBounds = world.worldBounds;

	spawnEnemyNearCamera(world.enemySpawnInfo);
	spawnEnemyNearCamera(world.turretSpawnInfo);
}

void updateWorldPhyics(GameWorld &world, const float &deltaTime) {

	vector<glm::vec2> playerPositions;

	for(auto &it : world.players) {

		it->updatePhysics(deltaTime, world.worldBounds, world.tileMap);

		if(it->checkCanRespawn()) {

			it->respawn(world.camera.getCameraBounds());
		}

		playerPositions.push_back(it->getPosition());
	}

	world.camera.calculateProperties(playerPositions);
	world.camera.update(deltaTime, world.worldBounds);

	for(unsigned i = 0; i < world.enemies.size(); ++i) {

		world.enemies[i]->updatePhysics(deltaTime, world.worldBounds, world.tileMap);
	}

	for(unsigned i = 0; i < world.turrets.size(); ++i) {

		world.turrets[i]->updatePhysics(deltaTime, world.worldBounds, world.tileMap, playerPositions);
	}

    removeDeadEntities(world.powerUps);
	removeDeadEntities(world.enemies);
	removeDeadEntities(world.turrets);
	removeDeadHashEntries(world.destructibleBlocks, world.destructibleBlockHash);
}

void updateWorldRendering(sf::RenderWindow &window, GameWorld &world) {

	updateObjectRendering(world.players);
	updateObjectRendering(world.enemies);
	updateObjectRendering(world.turrets);
	updateObjectRendering(world.destructibleBlocks);
	updateObjectRendering(world.powerUps);

	world.camera.applyCamera(window);

	sf::Vector2f viewOffset = world.camera.getViewTopLeft() - world.viewPositionLastFrame;
	world.viewPositionLastFrame = world.camera.getViewTopLeft();

	world.backgrounds.updateRendering(viewOffset);
}

void handleEntityCollisions(GameWorld &world) {

	playerWorldCollision(world);
	enemyWorldCollision(world);
	turretWorldCollision(world);
}

void playerWorldCollision(GameWorld &world) {

	//collision with players against rest of world
	for(auto &it : world.players) {

		collideShootingEntityDynamicObjectHash(it, world.destructibleBlockHash, destructibleBlockEntityCollisionFunction, bulletBlockCollisionFunction);

		collidePlayerEntities(it, world.powerUps, playerPowerUpCollisionFunction);

		collidePlayerEntities(it, world.enemies, playerEnemyCollisionFunction);
		collidePlayerShootingEntities(it, world.turrets, playerTurretCollisionFunction);

		//handle collision with player's bullets and enemies
		collideBulletsEntities(it->getGun()->getBullets(), world.enemies, bulletEnemyCollisionFunction);
		collideBulletsEntities(it->getGun()->getBullets(), world.turrets, bulletTurretCollisionFunction);
	}
}

template<class Entity>
void collidePlayerEntities(shared_ptr<Player> player, vector<shared_ptr<Entity> > &entities,
                                 function<void(shared_ptr<Player>, shared_ptr<Entity>)> collisionFunction) {

	for(unsigned i = 0; i < entities.size();) {

		collisionFunction(player, entities[i]);

		++i;
	}
}

template<class ShootingEntity>
void collidePlayerShootingEntities(shared_ptr<Player> player, vector<shared_ptr<ShootingEntity> > &entities,
                                 function<void(shared_ptr<Player>, shared_ptr<ShootingEntity>)> collisionFunction) {

	for(unsigned i = 0; i < entities.size();) {

		if(!entities[i]->checkIsAlive()) {

			continue;
		}

		collisionFunction(player, entities[i]);

		auto bullets = entities[i]->getGun()->getBullets();

		for(auto &it : bullets) {

			bulletNonDeflectingEntityCollision(it, player);
		}

		++i;
	}
}

void enemyWorldCollision(GameWorld &world) {

	for(auto &it : world.enemies) {

		collideEntityDynamicObjectHash(it, world.destructibleBlockHash, destructibleBlockEntityCollisionFunction);
	}
}

void turretWorldCollision(GameWorld &world) {

	for(auto &it : world.turrets) {

		collideShootingEntityDynamicObjectHash(it, world.destructibleBlockHash, destructibleBlockEntityCollisionFunction, bulletBlockCollisionFunction);
	}
}

template<class DynamicObject>
void collideEntityDynamicObjectHash(shared_ptr<EntityBase> entity, SpatialHash<DynamicObject> &hash,
                                 function<void(shared_ptr<DynamicObject>, shared_ptr<EntityBase>)> collisionFunction) {

	sf::FloatRect hitbox = entity->getHitbox().getActiveHitboxWorldSpace();
	auto blocks = hash.getSurroundingEntites(hitbox);

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

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Contra");

    sf::Event event;

    loadDataCollection(dataCollection);

    GameWorld world(window);
    world.worldBounds = sf::FloatRect(0, 0, 2048, 768);
    world.tileMap.resize(world.worldBounds.width, world.worldBounds.height);
    world.players.push_back(make_shared<Player>());

    world.powerUps.push_back(make_shared<PowerUp>(glm::vec2(0, 0), PowerUpType::MACHINE_GUN, dataCollection.powerupData));

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            handleWindowEvents(window, event, world);
            handleObjectEvents(window, event, world);

            if(event.type == sf::Event::MouseButtonPressed) {

                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());

                if(event.mouseButton.button == sf::Mouse::Left) {

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {

                        world.tileMap.setTile(mousePosition, TileType::UPWARD_RIGHT_1_1, "tileset.png", sf::IntRect(0, 0, 64, 64));

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {

                        world.tileMap.setTile(mousePosition, TileType::UPWARD_LEFT_1_1, "tileset.png", sf::IntRect(0, 0, 64, 64));

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {

                        world.tileMap.setTile(mousePosition, TileType::ONE_WAY, "tileset.png", sf::IntRect(0, 0, 64, 64));

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {

                        shared_ptr<SpawnPoint> point = make_shared<SpawnPoint>(mousePosition, sf::seconds(0.6));
                        point->setTypeOfEnemySpawned(EnemyType::ENEMY_GOOMBA);
                        world.enemySpawnInfo.spawnPoints.push_back(point);

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

                        world.tileMap.setTile(mousePosition, TileType::SOLID, "tileset.png", sf::IntRect(0, 0, 64, 64));
                    }

                }

                if(event.mouseButton.button == sf::Mouse::Right) {

                    world.tileMap.setTile(mousePosition, TileType::EMPTY, "tileset.png", sf::IntRect(0, 0, 0, 0));
                }
            }

        }

        handleObjectKeystate(window, world);

        updateWorld(window, world);

        window.clear();

        drawWorld(window, world);

        window.display();

    }
    return 0;
}
