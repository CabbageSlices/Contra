#ifndef ENEMYSPAWNERS_H_INCLUDED
#define ENEMYSPAWNERS_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <memory>

class Enemy;

struct SpawnPoint {

    SpawnPoint(const sf::Vector2f &position, const sf::Time &delay) :
        spawnPosition(position),
        spawnTimer(),
        spawnDelay(delay)
    {

    }

    bool checkCanSpawn() {

        return spawnTimer.getElapsedTime() > spawnDelay;
    }

    void resetSpawnTimer() {

        spawnTimer.restart();
    }

    sf::Vector2f spawnPosition;
    sf::Clock spawnTimer;
    sf::Time spawnDelay;
};

//information that a spawner can use for spawning
struct InformationForSpawner {

    InformationForSpawner(std::vector<std::shared_ptr<Enemy> > &enemyContainer, std::vector<std::shared_ptr<SpawnPoint> > &spawnPointContainer, const sf::FloatRect &camBounds, const sf::FloatRect &levelBounds) :
        enemies(enemyContainer),
        spawnPoints(spawnPointContainer),
        currentCameraBounds(camBounds),
        worldBounds(levelBounds)
        {

        }

    std::vector<std::shared_ptr<Enemy> > &enemies;
    std::vector<std::shared_ptr<SpawnPoint> > &spawnPoints;
    sf::FloatRect currentCameraBounds;
    sf::FloatRect worldBounds;
};

//the returned spawnpoint needs to be a reference but yo ucan't return local reference
//so instead this function returns the id of the closet spawn point, or -1 if none are available
int findClosestSpawnPointOffscreen(InformationForSpawner &spawnInfo);

bool spawnEnemyOffscreen(InformationForSpawner &spawnInfo);

#endif // ENEMYSPAWNERS_H_INCLUDED
