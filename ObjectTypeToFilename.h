#ifndef OBJECTTYPETOFILENAME_H_INCLUDED
#define OBJECTTYPETOFILENAME_H_INCLUDED

#include "ObjectTypes.h"
#include <string>

extern const std::string enemyDataPath;
extern const std::string playerDataPath;
extern const std::string bulletDataPath;
extern const std::string blockDataPath;
extern const std::string gunDataPath;
extern const std::string powerUpDataPath;

std::string getFilenameForData(const EnemyType &enemyType);
std::string getFilenameForData(const int &playerNumber);
std::string getFilenameForData(const BulletType &bulletType);
std::string getFilenameForData(const DestructibleBlockType &blockType);
std::string getFilenameForData(const GunType &gunType);
std::string getFilenameForData(const PowerUpType &powerUpType);

#endif // OBJECTTYPETOFILENAME_H_INCLUDED
