#ifndef DATALOADERS_H_INCLUDED
#define DATALOADERS_H_INCLUDED

#include <string>

class PreloadedTurretData;
class PreloadedEnemyData;
class PreloadedBulletData;
class PreloadedDestructibleBlockData;
class PreloadedPowerUpData;
class PreloadedOmniDirectionalTurretData;

bool loadTurretData(PreloadedTurretData &data, const std::string &dataFileName);
bool loadEnemyData(PreloadedEnemyData &daa, const std::string &dataFileName);
bool loadOmniDirectionalTurretData(PreloadedOmniDirectionalTurretData &data, const std::string &dataFileName);

bool loadBulletData(PreloadedBulletData &data, const std::string &dataFileName);

bool loadDestrutibleBlockData(PreloadedDestructibleBlockData &data, const std::string &dataFileName);

bool loadPowerUpData(PreloadedPowerUpData &data, const std::string &dataFileName);

#endif // DATALOADERS_H_INCLUDED
