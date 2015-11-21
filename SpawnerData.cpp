#include "SpawnerData.h"


bool activeSpawnPointsRemaining(std::vector<std::shared_ptr<SpawnPoint> > &spawnPoints) {

    //check if there are any spawn points that can spawn an enemy
    for(auto &spawnPoint : spawnPoints) {

        if(spawnPoint->getEnemiesLeftToSpawn() > 0) {

            return true;
        }
    }

    //none of the points can spawn an enemy
    return false;
}
