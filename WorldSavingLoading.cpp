#include "WorldSavingLoading.h"
#include "GameWorld.h"

using std::string;
using std::fstream;

const string savedDataPath("levels/");
const string saveFileExtention(".txt");

const DataTagPair playerSpawnTag("PlayerSpawnBegin", "PlayerSpawnBegin");
const DataTagPair tileMapTag("TileMapBegin", "TileMapEnd");
const DataTagPair backgroundTag("BackgroundBegin", "BackgroundEnd");
const DataTagPair worldBoundsTag("worldBoundsBegin", "worldBoundsEnd");
const DataTagPair worldBoundsBossFightTag("bossWorldBoundsBegin", "bossWorldBoundsEnd");
const DataTagPair nonBossEnemySpawnerTag("nonBossEnemySpawnersBegin", "nonBossEnemySpawnersEnd");
const DataTagPair bossEnemySpawnerTag("bossEnemySpawnersBegin", "bossEnemySpawnersEnd");
const DataTagPair basicEnemySpawnerTag("basicEnemySpawnerBegin", "basicEnemySpawnerEnd");
const DataTagPair turretEnemySpawnerTag("turretSpawnerBegin", "turretSpawnerEnd");
const DataTagPair omnidirectionalTurretSpawnerTag("omnidirectionalTurretSpawnerBegin", "omnidirectionalTurretSpawnerEnd");
const DataTagPair destructibleBlockTag("destructibleBlockBegin", "destructibleBlockEnd");

void saveWorld(const string& worldName, GameWorld &world) {


}
