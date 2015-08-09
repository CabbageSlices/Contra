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

bool loadBossData(std::map<EnemyType, BossProperties> &bossData, const std::string &dataFileName);

bool loadPlayerData(PreloadedPlayerData &data, const std::string &dataFileName);

bool loadTurretData(PreloadedTurretData &data, const std::string &dataFileName);
bool loadEnemyData(PreloadedEnemyData &daa, const std::string &dataFileName);
bool loadOmniDirectionalTurretData(PreloadedOmniDirectionalTurretData &data, const std::string &dataFileName);

bool loadBulletData(PreloadedBulletData &data, const std::string &dataFileName);

bool loadDestrutibleBlockData(PreloadedDestructibleBlockData &data, const std::string &dataFileName);

bool loadPowerUpData(PreloadedPowerUpData &data, const std::string &dataFileName);

#endif // DATALOADERS_H_INCLUDED
