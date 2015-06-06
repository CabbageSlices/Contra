#include "EnemySpawners.h"
#include "Enemy.h"
#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;

int findClosestSpawnPointOffscreen(InformationForSpawner &spawnInfo) {

    int closestPointId = -1;

    vector<shared_ptr<SpawnPoint> > spawnPoints = spawnInfo.spawnPoints;

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

        shared_ptr<SpawnPoint> &closestPoint = spawnPoints[closestPointId];

        float distanceCamToClosest = glm::dot(closestPoint->getSpawnPosition() - camPos, closestPoint->getSpawnPosition() - camPos);
        float distanceCamToCurrent = glm::dot(position - camPos, position - camPos);

        bool isCloser = glm::abs(distanceCamToCurrent) < glm::abs(distanceCamToClosest);

        if(isCloser) {

            closestPointId = i;
        }
    }

    return closestPointId;
}

bool spawnEnemyOffscreen(InformationForSpawner &spawnInfo) {

    int idClosestPoint = findClosestSpawnPointOffscreen(spawnInfo);

    if(idClosestPoint < 0 || idClosestPoint > spawnInfo.spawnPoints.size()) {

        //no valid point found so don't spawn anything
        return false;
    }

    shared_ptr<SpawnPoint> &closestPoint = spawnInfo.spawnPoints[idClosestPoint];

    Direction direction;

    //enemy spawned to the right of the players so look to left
    if(closestPoint->getSpawnPosition().x > spawnInfo.currentCameraBounds.left) {

        direction.horizontal = HorizontalDirection::LEFT;

    } else {

        direction.horizontal = HorizontalDirection::RIGHT;
    }

    shared_ptr<Enemy> enemy = make_shared<Enemy>(glm::vec2(closestPoint->getSpawnPosition().x, closestPoint->getSpawnPosition().y), direction);

    if(!enemy) {

        return false;
    }

    closestPoint->resetSpawnTimer();
    closestPoint->increaseSpawnCount();

    shared_ptr<SpatialHashEntry<Enemy> > hashEntry = make_shared<SpatialHashEntry<Enemy> >(enemy);
    spawnInfo.enemies.push_back(hashEntry);
    return true;
}
