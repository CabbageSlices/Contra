#ifndef ENEMYSPAWNERS_H_INCLUDED
#define ENEMYSPAWNERS_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include "SpatialHashEntry.h"
#include "Direction.h"
#include "PreloadedData.h"
#include "Enemy.h"
#include "TurretEnemy.h"

struct SpawnPoint {

    SpawnPoint(const sf::Vector2f &position, const sf::Time &delay, const unsigned &maxSpawns = 3) :
        enemiesSpawned(0),
        maxSpawnCount(maxSpawns),
        spawnPosition(position.x, position.y),
        spawnTimer(),
        spawnDelay(delay)
    {

    }

    SpawnPoint(const glm::vec2 &position, const sf::Time &delay, const unsigned &maxSpawns = 3) :
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

    EnemyType getTypeOfEnemySpawned() const {

        return typeOfEnemySpawned;
    }

    void setTypeOfEnemySpawned(const EnemyType &type) {

        typeOfEnemySpawned = type;
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

    InformationForSpawner(std::vector<std::shared_ptr<T> > &enemyContainer, PreloadedDataCollection &collection, const sf::FloatRect &camBounds, const sf::FloatRect &levelBounds) :
        enemies(enemyContainer),
        dataCollection(collection),
        spawnPoints(),
        currentCameraBounds(camBounds),
        worldBounds(levelBounds)
        {

        }

    std::vector<std::shared_ptr<T> > &enemies;
    PreloadedDataCollection &dataCollection;
    std::vector<std::shared_ptr<SpawnPoint> > spawnPoints;
    sf::FloatRect currentCameraBounds;
    sf::FloatRect worldBounds;
};

void applyLoadedData(Enemy &enemy, EnemyType enemyType, PreloadedDataCollection &dataCollection) {

    if(enemyType == EnemyType::ENEMY_GOOMBA) {

        enemy.load(dataCollection.goombaData);

    }
}

void applyLoadedData(TurretEnemy &enemy, EnemyType enemyType, PreloadedDataCollection &dataCollection) {

    if(enemyType == EnemyType::ENEMY_PIRANHA) {

        enemy.load(dataCollection.piranhaData);
    }
}

void spawnEntity(std::shared_ptr<Enemy> &enemy, InformationForSpawner<Enemy> &spawnInfo, std::shared_ptr<SpawnPoint> &closestPoint) {

    Direction direction;

    //enemy spawned to the right of the players so look to left
    if(closestPoint->getSpawnPosition().x > spawnInfo.currentCameraBounds.left) {

        direction.horizontal = HorizontalDirection::LEFT;

    } else {

        direction.horizontal = HorizontalDirection::RIGHT;
    }

    enemy = std::make_shared<Enemy>(glm::vec2(closestPoint->getSpawnPosition().x, closestPoint->getSpawnPosition().y), direction);
}

void spawnEntity(std::shared_ptr<TurretEnemy> &enemy, InformationForSpawner<TurretEnemy> &spawnInfo, std::shared_ptr<SpawnPoint> &closestPoint) {

    enemy = std::make_shared<TurretEnemy>(glm::vec2(closestPoint->getSpawnPosition().x, closestPoint->getSpawnPosition().y));
}

//the returned spawnpoint needs to be a reference but yo ucan't return local reference
//so instead this function returns the id of the closet spawn point, or -1 if none are available
template<class T>
int findClosestSpawnPointOffscreen(InformationForSpawner<T> &spawnInfo) {

    int closestPointId = -1;

    std::vector<std::shared_ptr<SpawnPoint> > &spawnPoints = spawnInfo.spawnPoints;

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
int findPointNearCamera(InformationForSpawner<T> &spawnInfo) {

    std::vector<std::shared_ptr<SpawnPoint> > &spawnPoints = spawnInfo.spawnPoints;

    glm::vec2 camCenter;
    camCenter.x = spawnInfo.currentCameraBounds.left + spawnInfo.currentCameraBounds.width / 2.f;
    camCenter.y = spawnInfo.currentCameraBounds.top + spawnInfo.currentCameraBounds.height / 2.f;

    //find a valid point within a certain distance from the camera center
    //this distance is determined by the camera size scaled by some arbritrary constant
    float sizeScale = 1.25f;

    glm::vec2 camSizeScaled;
    camSizeScaled.x = spawnInfo.currentCameraBounds.width;
    camSizeScaled.y = spawnInfo.currentCameraBounds.height;
    camSizeScaled *= sizeScale;

    sf::FloatRect validSpawnpointArea(camCenter.x - camSizeScaled.x / 2, camCenter.y - camSizeScaled.y / 2, camSizeScaled.x, camSizeScaled.y);

    for(unsigned i = 0; i < spawnPoints.size(); ++i) {

        if(!spawnPoints[i]->checkCanSpawn()) {

            continue;
        }

        glm::vec2 position = spawnPoints[i]->getSpawnPosition();
        sf::Vector2f pos(position.x, position.y);

        if(validSpawnpointArea.contains(pos)) {

            return i;
        }
    }

    return -1;
}

template<class T>
bool spawnEnemy(InformationForSpawner<T> &spawnInfo, int(*findSpawnPoint)(InformationForSpawner<T> &spawnInfo)) {

    int spawnPointId = findSpawnPoint(spawnInfo);

    if(spawnPointId < 0 || spawnPointId > spawnInfo.spawnPoints.size()) {

        //no valid point found so don't spawn anything
        return false;
    }

    std::shared_ptr<SpawnPoint> &closestPoint = spawnInfo.spawnPoints[spawnPointId];

    std::shared_ptr<T> enemy;

    spawnEntity(enemy, spawnInfo, closestPoint);

    if(!enemy) {

        return false;
    }

    applyLoadedData(*enemy, closestPoint->getTypeOfEnemySpawned(), spawnInfo.dataCollection);

    closestPoint->resetSpawnTimer();
    closestPoint->increaseSpawnCount();

    spawnInfo.enemies.push_back(enemy);
    return true;
}

template<class T>
bool spawnEnemyOffscreen(InformationForSpawner<T> &spawnInfo) {

    return spawnEnemy(spawnInfo, findClosestSpawnPointOffscreen);
}

template<class T>
bool spawnEnemyNearCamera(InformationForSpawner<T> &spawnInfo) {

    return spawnEnemy(spawnInfo, findPointNearCamera);
}

#endif // ENEMYSPAWNERS_H_INCLUDED
