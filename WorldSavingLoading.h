#ifndef WORLDSAVINGLOADING_H_INCLUDED
#define WORLDSAVINGLOADING_H_INCLUDED

#include <fstream>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <memory>

class GameWorld;
class TileMap;
class BackgroundManager;
class EnemySpawnerCollection;
class DestructibleBlock;

//every piece of data saved has a tag
//this tagpair stores the begin and end tag for some type of data
//first in the pair is the tag at the beginning of the data block, and the second item is the ending tag.
typedef std::pair<std::string, std::string> DataTagPair;

extern const std::string savedDataPath;
extern const std::string saveFileExtention;

extern const DataTagPair playerSpawnTag;
extern const DataTagPair tileMapTag;
extern const DataTagPair backgroundTag;
extern const DataTagPair worldBoundsTag;
extern const DataTagPair worldBoundsBossFightTag;
extern const DataTagPair nonBossEnemySpawnerTag;
extern const DataTagPair bossEnemySpawnerTag;
extern const DataTagPair basicEnemySpawnerTag;
extern const DataTagPair turretEnemySpawnerTag;
extern const DataTagPair omnidirectionalTurretSpawnerTag;
extern const DataTagPair destructibleBlocksTag;

void saveWorld(const std::string &worldName, GameWorld &world);

void saveTileMapData(std::fstream &file, TileMap &map);
void saveBackgroundData(std::fstream &file, BackgroundManager &manager);
void saveWorldBoundsData(std::fstream &file, sf::FloatRect &bounds, DataTagPair &boundsTag);
void saveSpawnerCollection(std::fstream &file, EnemySpawnerCollection &collection, DataTagPair &spawnerTag);//used for both boss and non boss spawners so specify what tag should be usd
void saveDestructibleBlocks(std::fstream &file, std::vector<std::shared_ptr<DestructibleBlock> > &destructibleBlocks);


void loadWorld(const std::string &worldName, GameWorld &world);

void loadTileMapData(std::fstream &file, TileMap &map, glm::vec2 worldSize);
void loadBackgroundData(std::fstream &file, BackgroundManager &manager, sf::FloatRect worldSize);
#endif // WORLDSAVINGLOADING_H_INCLUDED
