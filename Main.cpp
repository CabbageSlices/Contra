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

    sf::FloatRect worldBounds(0, 0, 1024 * 5, 768 * 2);

    TileMap tileMap(worldBounds.width, worldBounds.height);

    Camera camera(window);

    Enemy enemy(glm::vec2(0, 0), Direction());
    enemy.setInitialVelocity(glm::vec2(-TERMINAL_VELOCITY / 5, 0));

    typedef SpatialHashEntry<Enemy> EnemyHash;
    vector<shared_ptr<EnemyHash> > enemies;
    vector<shared_ptr<SpawnPoint> > spawnPoints;

    InformationForSpawner spawnInfo(enemies, spawnPoints, camera.getCameraBounds(), worldBounds);

    bool slowed = false;

    TurretEnemy enem(glm::vec2(256, 768 * 2 - 256));

    DestructibleBlock block(glm::vec2(512, 768 * 2 - 256));

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
                        ///spawnPoints.push_back(point);

                        shared_ptr<Enemy> enemy = make_shared<Enemy>(glm::vec2(mousePosition.x, mousePosition.y), Direction());
                        shared_ptr<EnemyHash> entry= make_shared<EnemyHash>(enemy);
                        enemies.push_back(entry);

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

        if(slowed) {

            deltaTime /= 10.f;
        }

        enemy.updatePhysics(deltaTime.asSeconds(), worldBounds, tileMap);
        block.updatePhysics(deltaTime.asSeconds(), worldBounds, tileMap);
        player->updatePhysics(deltaTime.asSeconds(), worldBounds, tileMap);

        if(player->checkCanRespawn()) {

            player->respawn(camera.getCameraBounds());
        }

        vector<shared_ptr<Bullet> > &playerBullets = player->getGun()->getBullets();

        if(block.checkIsAlive())
        player->respondToCollision(block.handleCollision(player));

        for(unsigned i = 0; i < enemies.size();) {

            enemies[i]->getObject()->updatePhysics(deltaTime.asSeconds(), worldBounds, tileMap);

            if(!enemies[i]->getObject()->checkIsAlive()) {

                enemies.erase(enemies.begin() + i);
                continue;
            }

            for(unsigned j = 0; j < playerBullets.size(); ++j) {

                if(playerBullets[j]->getHitbox().getActiveHitboxWorldSpace().intersects(enemies[i]->getObject()->getHitbox().getActiveHitboxWorldSpace()) && playerBullets[j]->checkIsAlive()) {

                    playerBullets[j]->handleCollision(enemies[i]->getObject());
                }
            }

            sf::FloatRect enemyHitbox = enemies[i]->getObject()->getHitbox().getActiveHitboxWorldSpace();
            sf::FloatRect playerHitbox = player->getHitbox().getActiveHitboxWorldSpace();

            if(enemyHitbox.intersects(playerHitbox) && player->checkCanGetHit()) {

                player->getHit();
            }

            ++i;
        }

        vector<glm::vec2> playerPositions;
        playerPositions.push_back(player->getPosition());
        //playerPositions.push_back(glm::vec2(0, 1920));

        enem.updatePhysics(deltaTime.asSeconds(), worldBounds, tileMap, playerPositions);

        camera.calculateProperties(playerPositions);
        camera.update(deltaTime.asSeconds(), worldBounds);

        camera.applyCamera(window);

        spawnInfo.currentCameraBounds = camera.getCameraBounds();

        if(enemies.size() < 100)
        spawnEnemyOffscreen(spawnInfo);

        sf::FloatRect collidingRect = player->getHitbox().getActiveHitboxWorldSpace();

        player->updateRendering();
        enem.updateRendering();
        for(unsigned i = 0; i < enemies.size(); ++i) {

            enemies[i]->getObject()->updateRendering();
        }

        block.updateRendering();

        window.clear();

        sf::FloatRect cameraBounds = camera.getCameraBounds();
        glm::vec2 topLeft(cameraBounds.left, cameraBounds.top);
        glm::vec2 bottomRight(cameraBounds.left + cameraBounds.width, cameraBounds.top + cameraBounds.height);

        tileMap.draw(window, topLeft, bottomRight);
        player->draw(window);

        for(unsigned i = 0; i < enemies.size(); ++i) {

            enemies[i]->getObject()->draw(window);
        }

        block.draw(window);
        enem.draw(window);

        window.display();
    }
    return 0;
}
