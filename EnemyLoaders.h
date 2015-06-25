#ifndef DataLoaders_H_INCLUDED
#define DataLoaders_H_INCLUDED

#include <string>

class PreloadedTurretData;
class PreloadedEnemyData;

bool loadTurretData(PreloadedTurretData &data, const std::string &dataFileName);
bool loadEnemyData(PreloadedEnemyData &daa, const std::string &dataFileName);

#endif // DataLoaders_H_INCLUDED
