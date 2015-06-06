#ifndef ENEMYSPAWNERS_H_INCLUDED
#define ENEMYSPAWNERS_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include "SpatialHashEntry.h"

class Enemy;

struct SpawnPoint {

    SpawnPoint(const sf::Vector2f &position, const sf::Time &delay, const unsigned &maxSpawns = 3) :
        enemiesSpawned(0),
        maxSpawnCount(maxSpawns),
        spawnPosition(position.x, position.y),
        spawnTimer(),
        spawnDelay(delay)
    {

    }

    SpawnPoint(const glm::vec2 &position, const sf::Time &delay, const unsigned &maxSpawns = 0) :
        enemiesSpawned(0),
        maxSpawnCount(maxSpawns),
        spawnPosition(position),
        spawnTimer(),
        spawnDelay(delay)
    {

    }

    bool checkCanSpawn() const {

        return spawnTimer.getElapsedTime() > spawnDelay && (enemiesSpawned < maxSpawnCount || maxSpawnCount == 0);
    }

    void resetSpawnTimer() {

        spawnTimer.restart();
    }

    void increaseSpawnCount() {

        enemiesSpawned++;
    }

    glm::vec2 getSpawnPosition() const {

        return spawnPosition;
    }

    private:

        unsigned enemiesSpawned;
        unsigned maxSpawnCount;//if this is set to 0 then the spawner has no limit

        glm::vec2 spawnPosition;
        sf::Clock spawnTimer;
        sf::Time spawnDelay;
};

//information that a spawner can use for spawning
struct InformationForSpawner {

    typedef SpatialHashEntry<Enemy> EnemyHash;
    InformationForSpawner(std::vector<std::shared_ptr<EnemyHash> > &enemyContainer, std::vector<std::shared_ptr<SpawnPoint> > &spawnPointContainer, const sf::FloatRect &camBounds, const sf::FloatRect &levelBounds) :
        enemies(enemyContainer),
        spawnPoints(spawnPointContainer),
        currentCameraBounds(camBounds),
        worldBounds(levelBounds)
        {

        }

    std::vector<std::shared_ptr<EnemyHash> > &enemies;
    std::vector<std::shared_ptr<SpawnPoint> > &spawnPoints;
    sf::FloatRect currentCameraBounds;
    sf::FloatRect worldBounds;
};

//the returned spawnpoint needs to be a reference but yo ucan't return local reference
//so instead this function returns the id of the closet spawn point, or -1 if none are available
int findClosestSpawnPointOffscreen(InformationForSpawner &spawnInfo);

bool spawnEnemyOffscreen(InformationForSpawner &spawnInfo);

#endif // ENEMYSPAWNERS_H_INCLUDED
