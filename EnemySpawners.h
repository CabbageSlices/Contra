#ifndef ENEMYSPAWNERS_H_INCLUDED
#define ENEMYSPAWNERS_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include "SpatialHashEntry.h"
#include "Direction.h"

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

//the returned spawnpoint needs to be a reference but yo ucan't return local reference
//so instead this function returns the id of the closet spawn point, or -1 if none are available
template<class T>
int findClosestSpawnPointOffscreen(InformationForSpawner<T> &spawnInfo) {

    int closestPointId = -1;

    std::vector<std::shared_ptr<SpawnPoint> > spawnPoints = spawnInfo.spawnPoints;

    if(spawnPoints.size() == 0) {

        return -1;
    }

    glm::vec2 camPos;
    camPos.x = spawnInfo.currentCameraBounds.left + spawnInfo.currentCameraBounds.width / 2.f;
    camPos.y = spawnInfo.currentCameraBounds.top + spawnInfo.currentCameraBounds.height / 2.f;

    for(unsigned i = 0; i < spawnPoints.size(); ++i) {

        glm::vec2 position = spawnPoints[i]->getSpawnPosition();

        if(spawnInfo.currentCameraBounds.contains(sf::Vector2f(position.x, position.y)) || !spawnPoints[i]->checkCanSpawn()) {

            continue;
        }

        if(closestPointId < 0) {

            //closest point hasn't been initialized yet so just set it to the current point
            closestPointId = i;
            continue;
        }

        std::shared_ptr<SpawnPoint> &closestPoint = spawnPoints[closestPointId];

        float distanceCamToClosest = glm::dot(closestPoint->getSpawnPosition() - camPos, closestPoint->getSpawnPosition() - camPos);
        float distanceCamToCurrent = glm::dot(position - camPos, position - camPos);

        bool isCloser = glm::abs(distanceCamToCurrent) < glm::abs(distanceCamToClosest);

        if(isCloser) {

            closestPointId = i;
        }
    }

    return closestPointId;
}

template<class T>
bool spawnEnemyOffscreen(InformationForSpawner<T> &spawnInfo) {

    int idClosestPoint = findClosestSpawnPointOffscreen(spawnInfo);

    if(idClosestPoint < 0 || idClosestPoint > spawnInfo.spawnPoints.size()) {

        //no valid point found so don't spawn anything
        return false;
    }

    std::shared_ptr<SpawnPoint> &closestPoint = spawnInfo.spawnPoints[idClosestPoint];

    Direction direction;

    //enemy spawned to the right of the players so look to left
    if(closestPoint->getSpawnPosition().x > spawnInfo.currentCameraBounds.left) {

        direction.horizontal = HorizontalDirection::LEFT;

    } else {

        direction.horizontal = HorizontalDirection::RIGHT;
    }

    std::shared_ptr<T> enemy = std::make_shared<T>(glm::vec2(closestPoint->getSpawnPosition().x, closestPoint->getSpawnPosition().y), direction);

    if(!enemy) {

        return false;
    }

    closestPoint->resetSpawnTimer();
    closestPoint->increaseSpawnCount();

    spawnInfo.enemies.push_back(enemy);
    return true;
}

#endif // ENEMYSPAWNERS_H_INCLUDED
