#include "GameWorld.h"
#include "GameConfiguration.h"

std::shared_ptr<Player> createPlayer(const glm::vec2 &spawnPosition, const PlayerNumber &number, GameConfiguration &gameConfig) {

    const PreloadedPlayerData &data = *dataCollection.getPlayerData(number);
    const PlayerKeys& keys = gameConfig.getKeysForPlayer(number);

    std::shared_ptr<Player> player(new Player(spawnPosition, keys));
    player->load(data);

    return player;
}
