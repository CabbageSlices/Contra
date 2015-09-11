#ifndef DATALOADERS_H_INCLUDED
#define DATALOADERS_H_INCLUDED

#include <string>
#include <map>

class PreloadedPlayerData;
class PreloadedTurretData;
class PreloadedEnemyData;
class PreloadedBulletData;
class PreloadedDestructibleBlockData;
class PreloadedPowerUpData;
class PreloadedOmniDirectionalTurretData;

enum EnemyType : int;
struct BossProperties;

bool loadBossData(std::map<EnemyType, BossProperties> &bossData, const std::string &dataFilename);

bool loadPlayerData(PreloadedPlayerData &data, const std::string &dataFilename);

bool loadTurretData(PreloadedTurretData &data, const std::string &dataFilename);
bool loadEnemyData(PreloadedEnemyData &data, const std::string &dataFilename);
bool loadOmniDirectionalTurretData(PreloadedOmniDirectionalTurretData &data, const std::string &dataFilename);

bool loadBulletData(PreloadedBulletData &data, const std::string &dataFilename);

bool loadDestrutibleBlockData(PreloadedDestructibleBlockData &data, const std::string &dataFilename);

bool loadPowerUpData(PreloadedPowerUpData &data, const std::string &dataFilename);

#endif // DATALOADERS_H_INCLUDED
