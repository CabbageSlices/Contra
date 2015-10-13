#ifndef DATALOADERS_H_INCLUDED
#define DATALOADERS_H_INCLUDED

#include <string>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include "ObjectTypes.h"
#include "DataTagPair.h"

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

bool loadIntegerParameter(std::fstream &file, int &loadedData, const DataTagPair &tagPair);

bool loadEntityHealth(std::fstream &file, int &health);
bool loadTextureFilename(std::fstream &file, std::string &textureFilename);
bool loadAnimationNextFrameTime(std::fstream &file, sf::Time &animationNextFrameTime);
bool loadEntityScale(std::fstream &file, float &scale, const DataTagPair &tagPair);
bool loadEntityGunType(std::fstream &file, GunType &gunType);
bool loadEntityGunfireDelay(std::fstream &file, sf::Time &gunfireDelay);
bool loadEntityBulletType(std::fstream &file, BulletType &bulletType);
bool loadEntityBulletOrigin(std::fstream &file, std::map<unsigned, glm::vec2> &bulletOriginForState);
bool loadEntityTextureRects(std::fstream &file, std::map<unsigned, std::vector<sf::IntRect> > &textureRects);
bool loadEntityCollisionBoxes(std::fstream &file, std::map<unsigned, std::vector<sf::FloatRect> > &hurtboxes, const DataTagPair &tagPair);

bool loadPlayerData(PreloadedPlayerData &data, const std::string &dataFilename);

bool loadTurretData(PreloadedTurretData &data, const std::string &dataFilename);
bool loadEnemyData(PreloadedEnemyData &data, const std::string &dataFilename);
bool loadOmniDirectionalTurretData(PreloadedOmniDirectionalTurretData &data, const std::string &dataFilename);

bool loadBulletData(PreloadedBulletData &data, const std::string &dataFilename);

bool loadDestrutibleBlockData(PreloadedDestructibleBlockData &data, const std::string &dataFilename);

bool loadPowerUpData(PreloadedPowerUpData &data, const std::string &dataFilename);

#endif // DATALOADERS_H_INCLUDED
