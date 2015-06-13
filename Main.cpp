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

#include <vector>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::vector;

int main() {

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Contra");

    sf::Event event;

    shared_ptr<Player> player = make_shared<Player>();

    sf::Clock timer;

    sf::FloatRect worldBounds(0, 0, 1024 * 10, 768 * 2);

    TileMap tileMap(worldBounds.width, worldBounds.height);

    Camera camera(window);

    vector<shared_ptr<Enemy> > enemies;
    vector<shared_ptr<SpawnPoint> > spawnPoints;
    InformationForSpawner<Enemy> spawnInfo(enemies, spawnPoints, camera.getCameraBounds(), worldBounds);

    vector<shared_ptr<DestructibleBlock> > destructibleBlocks;
    SpatialHash<DestructibleBlock> blockHash(TILE_SIZE, TILE_SIZE);

    bool slowed = false;

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
                        spawnPoints.push_back(point);

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

        std::unordered_set<shared_ptr<DestructibleBlock> > collidingWithPlayer = blockHash.getSurroundingEntites(player->getHitbox().getActiveHitboxWorldSpace());

        for(auto it = collidingWithPlayer.begin(); it != collidingWithPlayer.end(); ++it) {

            if((*it)->getHitbox().getActiveHitboxWorldSpace().intersects(player->getHitbox().getActiveHitboxWorldSpace() )) {

                CollisionResponse blockResponse = (*it)->handleCollision(player);
                player->respondToCollision(blockResponse);
            }
        }

        for(unsigned i = 0; i < enemies.size();) {

            sf::FloatRect previousBounds = enemies[i]->getHitbox().getActiveHitboxWorldSpace();
            ///enemies[i]->updatePhysics(deltaTime.asSeconds(), worldBounds, tileMap);

            if(!enemies[i]->checkIsAlive()) {

                enemies.erase(enemies.begin() + i);
                continue;
            }

            sf::FloatRect currentBounds = enemies[i]->getHitbox().getActiveHitboxWorldSpace();

            for(unsigned j = 0; j < playerBullets.size(); ++j) {

                if(playerBullets[j]->getHitbox().getActiveHitboxWorldSpace().intersects(enemies[i]->getHitbox().getActiveHitboxWorldSpace()) && playerBullets[j]->checkIsAlive()) {

                    playerBullets[j]->handleCollision(enemies[i]);
                }
            }

            sf::FloatRect enemyHitbox = enemies[i]->getHitbox().getActiveHitboxWorldSpace();
            sf::FloatRect playerHitbox = player->getHitbox().getActiveHitboxWorldSpace();

            if(enemyHitbox.intersects(playerHitbox) && player->checkCanGetHit()) {

                enemies[i]->handleCollision(player);
            }

            ++i;
        }

        for(unsigned i = 0; i < destructibleBlocks.size();) {

            destructibleBlocks[i]->updatePhysics(deltaTime.asSeconds(), worldBounds, tileMap);
            destructibleBlocks[i]->updateRendering();

            if(!destructibleBlocks[i]->checkIsAlive()) {

                blockHash.remove(destructibleBlocks[i]);
                destructibleBlocks.erase(destructibleBlocks.begin() + i);

            } else {

                ++i;
            }
        }

        vector<glm::vec2> playerPositions;
        playerPositions.push_back(player->getPosition());

        camera.calculateProperties(playerPositions);
        camera.update(deltaTime.asSeconds(), worldBounds);

        camera.applyCamera(window);

        spawnInfo.currentCameraBounds = camera.getCameraBounds();

        player->updateRendering();
        for(unsigned i = 0; i < enemies.size(); ++i) {

            enemies[i]->updateRendering();
        }

        window.clear();

        sf::FloatRect cameraBounds = camera.getCameraBounds();
        glm::vec2 topLeft(cameraBounds.left, cameraBounds.top);
        glm::vec2 bottomRight(cameraBounds.left + cameraBounds.width, cameraBounds.top + cameraBounds.height);

        tileMap.draw(window, topLeft, bottomRight);
        player->draw(window);

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
