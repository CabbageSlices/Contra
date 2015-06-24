#ifndef ENEMYLOADERS_H_INCLUDED
#define ENEMYLOADERS_H_INCLUDED

#include <string>

class TurretEnemy;
class Enemy;

bool loadEnemy(TurretEnemy &enemy, const std::string &dataFileName);
bool loadEnemy(Enemy &enemy, const std::string &dataFileName);

#endif // ENEMYLOADERS_H_INCLUDED
