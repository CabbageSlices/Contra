#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Player.h"
#include "Tile.h"
#include "TileMap.h"
#include "Gun.h"
#include "Camera.h"
#include "Enemy.h"
#include "EnemySpawners.h"

#include <vector>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::vector;

int main() {

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Contra");

    sf::Event event;

    Player player;

    sf::Clock timer;

    sf::FloatRect worldBounds(0, 0, 3072, 1920);

    TileMap tileMap(worldBounds.width, worldBounds.height);

    Camera camera(window);

    Enemy enemy(glm::vec2(0, 0), Direction());
    enemy.setInitialVelocity(glm::vec2(-TERMINAL_VELOCITY / 5, 0));

    vector<shared_ptr<Enemy> > enemies;
    vector<shared_ptr<SpawnPoint> > spawnPoints;

    InformationForSpawner spawnInfo(enemies, spawnPoints, camera.getCameraBounds(), worldBounds);

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

                        shared_ptr<SpawnPoint> point = make_shared<SpawnPoint>(mousePosition, sf::seconds(1.5));
                        spawnPoints.push_back(point);

                    } else {

                        tileMap.setTile(mousePosition, TileType::SOLID);
                    }

                }

                if(event.mouseButton.button == sf::Mouse::Right) {

                    tileMap.setTile(mousePosition, TileType::EMPTY);
                }
            }

            player.handleInputEvents(event, window);
        }

        player.handleKeystate(window);

        sf::Time deltaTime = timer.restart();

        if(slowed) {

            deltaTime /= 10.f;
        }

        vector<glm::vec2> playerPositions;

        enemy.update(deltaTime.asSeconds(), worldBounds, tileMap);


        for(unsigned i = 0; i < enemies.size(); ++i) {

            enemies[i]->update(deltaTime.asSeconds(), worldBounds, tileMap);
        }

        player.update(deltaTime.asSeconds(), worldBounds, tileMap);

        playerPositions.push_back(player.getPosition());
        playerPositions.push_back(glm::vec2(0, 1920));

        camera.calculateProperties(playerPositions);
        camera.update(deltaTime.asSeconds(), worldBounds);

        camera.applyCamera(window);

        spawnInfo.currentCameraBounds = camera.getCameraBounds();

        if(spawnEnemyOffscreen(spawnInfo)) {

            cout << "spawned" << endl;
        }

        window.clear();

        sf::FloatRect cameraBounds = camera.getCameraBounds();
        glm::vec2 topLeft(cameraBounds.left, cameraBounds.top);
        glm::vec2 bottomRight(cameraBounds.left + cameraBounds.width, cameraBounds.top + cameraBounds.height);

        tileMap.drawDebug(window, topLeft, bottomRight);
        player.draw(window);

        for(unsigned i = 0; i < enemies.size(); ++i) {

            enemies[i]->draw(window);
        }

        window.display();
    }
    return 0;
}
