#include "EnemySpawners.h"
#include "Enemy.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;

int findClosestSpawnPointOffscreen(InformationForSpawner &spawnInfo) {

    int closestPointId = -1;

    vector<shared_ptr<SpawnPoint> > spawnPoints = spawnInfo.spawnPoints;

    if(spawnPoints.size() == 0) {

        return -1;
    }

    const float cameraHorizontalCenter = spawnInfo.currentCameraBounds.left + spawnInfo.currentCameraBounds.width / 2;

    for(unsigned i = 1; i < spawnPoints.size(); ++i) {

        sf::Vector2f &position = spawnPoints[i]->spawnPosition;

        if(spawnInfo.currentCameraBounds.contains(position) || !spawnPoints[i]->checkCanSpawn()) {

            //in camera bounds or the spawner isn't active, so we can't use this
            continue;
        }

        if(closestPointId < 0) {

            //closest point hasn't been initialized yet so just set it to the current point
            closestPointId = i;
            continue;
        }

        shared_ptr<SpawnPoint> &closestPoint = spawnPoints[closestPointId];

        bool isCloser = glm::abs(position.x - cameraHorizontalCenter) < glm::abs(closestPoint->spawnPosition.x - cameraHorizontalCenter);

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
    if(closestPoint->spawnPosition.x > spawnInfo.currentCameraBounds.left) {

        direction.horizontal = HorizontalDirection::LEFT;

    } else {

        direction.horizontal = HorizontalDirection::RIGHT;
    }

    shared_ptr<Enemy> enemy = make_shared<Enemy>(glm::vec2(closestPoint->spawnPosition.x, closestPoint->spawnPosition.y), direction);

    if(!enemy) {

        return false;
    }

    closestPoint->resetSpawnTimer();
    spawnInfo.enemies.push_back(enemy);
    return true;
}
