#ifndef SPAWNERDATA_H_INCLUDED
#define SPAWNERDATA_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <memory>

struct SpawnPoint {

    SpawnPoint(const sf::Vector2f &position, const sf::Time &delay, const EnemyType &type, const unsigned &maxSpawns = 3) :
        enemiesSpawned(0),
        maxSpawnCount(maxSpawns),
        spawnPosition(position.x, position.y),
        spawnTimer(),
        spawnDelay(delay),
        typeOfEnemySpawned(type)
    {

    }

    SpawnPoint(const glm::vec2 &position, const sf::Time &delay, const EnemyType &type, const unsigned &maxSpawns = 3) :
        enemiesSpawned(0),
        maxSpawnCount(maxSpawns),
        spawnPosition(position),
        spawnTimer(),
        spawnDelay(delay),
        typeOfEnemySpawned(type)
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

    EnemyType getTypeOfEnemySpawned() const {

        return typeOfEnemySpawned;
    }

    unsigned getEnemiesLeftToSpawn() const {

        return maxSpawnCount - enemiesSpawned;
    }

    unsigned getMaxEnemiesSpawned() const {

        return maxSpawnCount;
    }

    unsigned getSpawnDelayInMilliseconds() const {

        return spawnDelay.asMilliseconds();
    }

    void setTypeOfEnemySpawned(const EnemyType &type) {

        typeOfEnemySpawned = type;
    }

    void stopSpawning() {

        //spawner stops working once max sapwn limit is reached
        enemiesSpawned = maxSpawnCount + 1;
    }

    private:

        unsigned enemiesSpawned;
        unsigned maxSpawnCount;//if this is set to 0 then the spawner has no limit

        glm::vec2 spawnPosition;
        sf::Clock spawnTimer;
        sf::Time spawnDelay;

        EnemyType typeOfEnemySpawned;
};

//information that a spawner can use for spawning
template<class T>
struct InformationForSpawner {

    InformationForSpawner(std::vector<std::shared_ptr<T> > &enemyContainer, const sf::FloatRect &camBounds, const sf::FloatRect &levelBounds) :
        enemies(enemyContainer),
        spawnPoints(),
        currentCameraBounds(camBounds),
        worldBounds(levelBounds)
        {

        }

    std::vector<std::shared_ptr<T> > &enemies;
    std::vector<std::shared_ptr<SpawnPoint> > spawnPoints;
    sf::FloatRect currentCameraBounds;
    sf::FloatRect worldBounds;
};

#endif // SPAWNERDATA_H_INCLUDED
