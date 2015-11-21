#include "GameWorld.h"
#include "GameConfiguration.h"

std::shared_ptr<Player> createPlayer(const glm::vec2 &spawnPosition, const PlayerNumber &number, GameConfiguration &gameConfig) {

    const PreloadedPlayerData &data = *dataCollection.getPlayerData(number);
    const PlayerKeys& keys = gameConfig.getKeysForPlayer(number);

    std::shared_ptr<Player> player(new Player(spawnPosition, keys));
    player->load(data);

    return player;
}

bool canSpawnEnemy(EnemySpawnerCollection &spawnerCollection) {

    if(activeSpawnPointsRemaining(spawnerCollection.enemySpawnInfo.spawnPoints)) {

        return true;
    }

    if(activeSpawnPointsRemaining(spawnerCollection.turretSpawnInfo.spawnPoints)) {

        return true;
    }

    if(activeSpawnPointsRemaining(spawnerCollection.omnidirectionalTurretSpawnInfo.spawnPoints)) {

        return true;
    }

    return false;
}

bool enemiesRemaining(EnemyCollection &enemyCollection) {

    if(enemyCollection.enemies.size() > 0) {

        return true;
    }

    if(enemyCollection.omnidirectionalTurrets.size() > 0) {

        return true;
    }

    if(enemyCollection.turrets.size() > 0) {

        return true;
    }

    return false;
}
