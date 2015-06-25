#ifndef DATALOADERS_H_INCLUDED
#define DATALOADERS_H_INCLUDED

#include <string>

class PreloadedTurretData;
class PreloadedEnemyData;
class PreloadedBulletData;

bool loadTurretData(PreloadedTurretData &data, const std::string &dataFileName);
bool loadEnemyData(PreloadedEnemyData &daa, const std::string &dataFileName);

bool loadBulletData(PreloadedBulletData &data, const std::string &dataFileName);

#endif // DATALOADERS_H_INCLUDED
