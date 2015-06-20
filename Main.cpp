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
#include "EnemyLoaders.h"

#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>
#include <unordered_set>

using std::unordered_set;
using std::remove_if;
using std::forward_iterator_tag;
using std::shared_ptr;
using std::make_shared;
using std::vector;

struct GameWorld {

	GameWorld(sf::RenderWindow &window) :
		players(),
		enemies(),
		turrets(),
		tileMap(),
		destructibleBlocks(),
		worldBounds(0, 0, 0, 0),
		camera(window),
		enemySpawnPoints(),
		turretSpawnPoints(),
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

	//environment
	TileMap tileMap;
	vector<shared_ptr<DestructibleBlock> > destructibleBlocks;

	//world properties
	sf::FloatRect worldBounds;
	Camera camera;

	//spawner properties
	vector<shared_ptr<SpawnPoint> > enemySpawnPoints;
	vector<shared_ptr<SpawnPoint> > turretSpawnPoints;
	InformationForSpawner<Enemy> enemySpawnInfo;
	InformationForSpawner<TurretEnemy> turretSpawnInfo;

	//other stuff
	SpatialHash<DestructibleBlock> destructibleBlockHash;
	sf::Clock updateTimer;
};

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
void collideEntityPair(shared_ptr<EntityBase> collisionHandler, shared_ptr<EntityBase> responder);
void drawTiles(sf::RenderWindow &window, GameWorld &world);

template<class Entity>
void collidePlayerEntities(shared_ptr<Player> player, vector<shared_ptr<Entity> > &entities);

template<class ShootingEntity>
void collidePlayerShootingEntities(shared_ptr<Player> player, vector<shared_ptr<ShootingEntity> > &entities);

template<class DynamicObject>
void collideEntityDynamicObjectHash(shared_ptr<EntityBase> entity, SpatialHash<DynamicObject> &hash);

template<class DynamicObject>
void collideShootingEntityDynamicObjectHash(shared_ptr<ShootingEntity> shooter, SpatialHash<DynamicObject> &hash);

template<class T>
void removeDeadEntities(vector<shared_ptr<T> > &entities);

template<class T>
void removeDeadHashEntries(vector<shared_ptr<T> > &entities, SpatialHash<T> &hash);

template<class T>
typename vector<shared_ptr<T> >::iterator placeDeadEntitiesAtEnd(vector<shared_ptr<T> > &entities);

template<class B, class T>
void collideBulletsEntities(vector<shared_ptr<B> > &bullets, vector<shared_ptr<T> > &entities);

template<class DynamicObject>
void collideEntityDynamicObjects(shared_ptr<EntityBase> entity, unordered_set<shared_ptr<DynamicObject> > &objects);

template<class DynamicObject>
void collideBulletDynamicObjects(shared_ptr<Bullet> bullet, unordered_set<shared_ptr<DynamicObject> > &objects);

template<class T>
void collideBulletEntities(shared_ptr<Bullet> bullet, vector<shared_ptr<T> > &entities);

template<class T>
void updateObjectRendering(vector<shared_ptr<T> > &entities);

template<class T>
void drawEntities(sf::RenderWindow &window, vector<shared_ptr<T> > &entities);

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

	drawTiles(window, world);
	drawEntities(window, world.players);
	drawEntities(window, world.enemies);
	drawEntities(window, world.turrets);
	drawEntities(window, world.destructibleBlocks);
}

void updateEnemySpawners(GameWorld &world) {

	world.enemySpawnInfo.currentCameraBounds = world.camera.getCameraBounds();
	world.turretSpawnInfo.currentCameraBounds = world.camera.getCameraBounds();

	spawnEnemyOffscreen(world.enemySpawnInfo);
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

	for(unsigned i = 0; i < world.enemies.size(); ++i) {

		world.turrets[i]->updatePhysics(deltaTime, world.worldBounds, world.tileMap, playerPositions);
	}

	removeDeadEntities(world.enemies);
	removeDeadEntities(world.turrets);
	removeDeadHashEntries(world.destructibleBlocks, world.destructibleBlockHash);
}

void updateWorldRendering(sf::RenderWindow &window, GameWorld &world) {

	updateObjectRendering(world.players);
	updateObjectRendering(world.enemies);
	updateObjectRendering(world.turrets);
	updateObjectRendering(world.destructibleBlocks);

	world.camera.applyCamera(window);
}

void handleEntityCollisions(GameWorld &world) {

	playerWorldCollision(world);
	enemyWorldCollision(world);
	turretWorldCollision(world);
}

void playerWorldCollision(GameWorld &world) {

	//collision with players against rest of world
	for(auto &it : world.players) {

		collideShootingEntityDynamicObjectHash(it, world.destructibleBlockHash);
		collidePlayerEntities(it, world.enemies);
		collidePlayerShootingEntities(it, world.turrets);

		//handle collision with player's bullets and enemies
		collideBulletsEntities(it->getGun()->getBullets(), world.enemies);
		collideBulletsEntities(it->getGun()->getBullets(), world.turrets);
	}
}

template<class Entity>
void collidePlayerEntities(shared_ptr<Player> player, vector<shared_ptr<Entity> > &entities) {

	for(unsigned i = 0; i < entities.size();) {

		if(!entities[i]->checkIsAlive()) {

			continue;
		}

		collideEntityPairCheckBoth(entities[i], player);

		++i;
	}
}

template<class ShootingEntity>
void collidePlayerShootingEntities(shared_ptr<Player> player, vector<shared_ptr<ShootingEntity> > &entities) {

	for(unsigned i = 0; i < entities.size();) {

		if(!entities[i]->checkIsAlive()) {

			continue;
		}

		collideEntityPairCheckBoth(entities[i], player);

		auto bullets = entities[i]->getGun()->getBullets();

		for(auto &it : bullets) {

			collideEntityPairCheckBoth(it, player);
		}

		++i;
	}
}

void enemyWorldCollision(GameWorld &world) {

	for(auto &it : world.players) {

		collideEntityDynamicObjectHash(it, world.destructibleBlockHash);
	}
}

void turretWorldCollision(GameWorld &world) {

	for(auto &it : world.turrets) {

		collideShootingEntityDynamicObjectHash(it, world.destructibleBlockHash);
	}
}

template<class DynamicObject>
void collideEntityDynamicObjectHash(shared_ptr<EntityBase> entity, SpatialHash<DynamicObject> &hash) {

	sf::FloatRect hitbox = entity->getHitbox().getActiveHitboxWorldSpace();
	auto blocks = hash.getSurroundingEntites(hitbox);

	collideEntityDynamicObjects(entity, blocks);
}

template<class DynamicObject>
void collideShootingEntityDynamicObjectHash(shared_ptr<ShootingEntity> shooter, SpatialHash<DynamicObject> &hash) {

	collideEntityDynamicObjectHash(shooter, hash);

	auto bullets = shooter->getGun()->getBullets();

	for(auto &it : bullets) {

		sf::FloatRect bulletHitbox = it->getHitbox().getActiveHitboxWorldSpace();
		auto collidingWithBullet = hash.getSurroundingEntites(bulletHitbox);

		collideBulletDynamicObjects(it, collidingWithBullet);
	}
}

template<class T>
void removeDeadEntities(vector<shared_ptr<T> > &entities) {

	auto iter = placeDeadEntitiesAtEnd<T>(entities);

	entities.erase(iter, entities.end());
}

template<class T>
void removeDeadHashEntries(vector<shared_ptr<T> > &entities, SpatialHash<T> &hash) {

	auto iter = placeDeadEntitiesAtEnd(entities);

	while(iter != entities.end()) {

		hash.remove(*iter);
		iter = entities.erase(iter);
	}
}

template<class T>
typename vector<shared_ptr<T> >::iterator placeDeadEntitiesAtEnd(vector<shared_ptr<T> > &entities) {

	return remove_if(entities.begin(), entities.end(),
		[](shared_ptr<T> &entity) {

			return !entity->checkIsAlive();
		});
}

template<class B, class T>
void collideBulletsEntities(vector<shared_ptr<B> > &bullets, vector<shared_ptr<T> > &entities) {

    for(auto &bullet : bullets) {

        collideBulletEntities(bullet, entities);
    }
}

template<class B, class DynamicObject>
void collideBulletsDynamicObjects(vector<shared_ptr<B> > &bullets, unordered_set<shared_ptr<DynamicObject> > &objects) {

    for(auto &bullet : bullets) {

        collideBulletDynamicObjects(bullet, objects);
    }
}

template<class DynamicObject>
void collideEntityDynamicObjects(shared_ptr<EntityBase> entity, unordered_set<shared_ptr<DynamicObject> > &objects) {

	for(auto &it : objects) {

		collideEntityPairCheckHandler(it, entity);
	}
}

template<class DynamicObject>
void collideBulletDynamicObjects(shared_ptr<Bullet> bullet, unordered_set<shared_ptr<DynamicObject> > &objects) {

	for(auto &it : objects) {

		collideEntityPairCheckBoth(bullet, it);
	}
}

template<class T>
void collideBulletEntities(shared_ptr<Bullet> bullet, vector<shared_ptr<T> > &entities) {

	for(auto &it : entities) {

		collideEntityPairCheckBoth(bullet, it);
	}
}

void collideEntityPair(shared_ptr<EntityBase> collisionHandler, shared_ptr<EntityBase> responder) {

	sf::FloatRect handlerHitbox = collisionHandler->getHitbox().getActiveHitboxWorldSpace();
	sf::FloatRect responderHitbox = responder->getHitbox().getActiveHitboxWorldSpace();

	if(handlerHitbox.intersects(responderHitbox)) {

		CollisionResponse collisionResponse = collisionHandler->handleCollision(responder);
		responder->respondToCollision(collisionResponse);
	}
}

void collideEntityPairCheckHandler(shared_ptr<EntityBase> collisionHandler, shared_ptr<EntityBase> responder) {

	if(!collisionHandler->checkCanGetHit()) {

		return;
	}

	collideEntityPair(collisionHandler, responder);
}

void collideEntityPairCheckResponder(shared_ptr<EntityBase> collisionHandler, shared_ptr<EntityBase> responder) {

	if(!responder->checkCanGetHit()) {

		return;
	}

	collideEntityPair(collisionHandler, responder);
}

void collideEntityPairCheckBoth(shared_ptr<EntityBase> collisionHandler, shared_ptr<EntityBase> responder) {

	if(!collisionHandler->checkCanGetHit() || !responder->checkCanGetHit()) {

		return;
	}

	collideEntityPair(collisionHandler, responder);
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

    shared_ptr<Player> player = make_shared<Player>();

    sf::Clock timer;

    sf::FloatRect worldBounds(0, 0, 1024 + 64, 768);

    TileMap tileMap(worldBounds.width, worldBounds.height);

    Camera camera(window);

    vector<shared_ptr<Enemy> > enemies;
    InformationForSpawner<Enemy> spawnInfo(enemies, camera.getCameraBounds(), worldBounds);

    vector<shared_ptr<DestructibleBlock> > destructibleBlocks;
    SpatialHash<DestructibleBlock> blockHash(TILE_SIZE, TILE_SIZE);

    bool slowed = false;

    shared_ptr<TurretEnemy> enem = make_shared<TurretEnemy>(glm::vec2(100, 500), 8);
    loadEnemy(*enem, "asdf");

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }

            if(event.type == sf::Event::KeyPressed) {

                if(event.key.code == sf::Keyboard::Escape) {

                    window.close();
                }

                if(event.key.code == sf::Keyboard::Z) {

                    slowed = !slowed;
                }
            }

            if(event.type == sf::Event::Resized) {

                camera.setupDefaultProperties(window);
            }

            if(event.type == sf::Event::MouseButtonPressed) {

                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());

                if(event.mouseButton.button == sf::Mouse::Left) {

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {

                        tileMap.setTile(mousePosition, TileType::UPWARD_RIGHT_1_1);

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {

                        tileMap.setTile(mousePosition, TileType::UPWARD_LEFT_1_1);

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {

                        tileMap.setTile(mousePosition, TileType::ONE_WAY);

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {

                        shared_ptr<SpawnPoint> point = make_shared<SpawnPoint>(mousePosition, sf::seconds(0.6));
                        spawnInfo.spawnPoints.push_back(point);

                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {

                        //snap the block to the grid
                        glm::vec2 position(mousePosition.x, mousePosition.y);
                        position /= static_cast<float>(TILE_SIZE);
                        position.x = glm::floor(position.x);
                        position.y = glm::floor(position.y);

                        position *= static_cast<float>(TILE_SIZE);

                        shared_ptr<DestructibleBlock> block = make_shared<DestructibleBlock>(position);
                        destructibleBlocks.push_back(block);
                        blockHash.insert(block);

                    } else {

                        tileMap.setTile(mousePosition, TileType::SOLID);
                    }

                }

                if(event.mouseButton.button == sf::Mouse::Right) {

                    tileMap.setTile(mousePosition, TileType::EMPTY);
                }
            }

            player->handleInputEvents(event, window);
        }

        player->handleKeystate(window);

        sf::Time deltaTime = timer.restart();

        player->updatePhysics(deltaTime.asSeconds(), worldBounds, tileMap);

        if(player->checkCanRespawn()) {

            player->respawn(camera.getCameraBounds());
        }

        vector<shared_ptr<Bullet> > &playerBullets = player->getGun()->getBullets();

        collideEntityDynamicObjectHash(player, blockHash);
        collidePlayerEntities(player, enemies);
        collideBulletsEntities(playerBullets, enemies);

        for(unsigned i = 0; i < enemies.size();) {

            enemies[i]->updatePhysics(deltaTime.asSeconds(), worldBounds, tileMap);

            if(!enemies[i]->checkIsAlive()) {

                enemies.erase(enemies.begin() + i);
                continue;
            }


            collideEntityDynamicObjectHash(enemies[i], blockHash);

            ++i;
        }

        for(unsigned i = 0; i < destructibleBlocks.size();) {

            destructibleBlocks[i]->updateRendering();

            ++i;
        }

        removeDeadHashEntries(destructibleBlocks, blockHash);

        vector<glm::vec2> playerPositions;
        playerPositions.push_back(player->getPosition());

        ///enem->updatePhysics(deltaTime.asSeconds(), worldBounds, tileMap, playerPositions);

        camera.calculateProperties(playerPositions);
        camera.update(deltaTime.asSeconds(), worldBounds);

        camera.applyCamera(window);

        spawnInfo.currentCameraBounds = camera.getCameraBounds();

        spawnEnemyOffscreen(spawnInfo);

        player->updateRendering();
        for(unsigned i = 0; i < enemies.size(); ++i) {

            enemies[i]->updateRendering();
        }

        ///enem->updateRendering();

        window.clear();

        sf::FloatRect cameraBounds = camera.getCameraBounds();
        glm::vec2 topLeft(cameraBounds.left, cameraBounds.top);
        glm::vec2 bottomRight(cameraBounds.left + cameraBounds.width, cameraBounds.top + cameraBounds.height);

        tileMap.draw(window, topLeft, bottomRight);

        player->draw(window);

        enem->draw(window);

        for(unsigned i = 0; i < enemies.size(); ++i) {

            enemies[i]->draw(window);
        }

        for(unsigned i = 0; i < destructibleBlocks.size(); ++i) {

            destructibleBlocks[i]->draw(window);
        }

        window.display();
    }
    return 0;
}
