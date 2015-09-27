#ifndef WORLDSAVINGLOADING_H_INCLUDED
#define WORLDSAVINGLOADING_H_INCLUDED

#include <fstream>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include "WorldSavedataTags.h"
#include <vector>
#include <memory>

class GameWorld;
class TileMap;
class BackgroundManager;
class EnemySpawnerCollection;
class DestructibleBlock;
class SpawnPoint;

extern const std::string savedWorldDataPath;
extern const std::string saveFileExtention;

void saveWorld(const std::string &worldName, GameWorld &world);

void saveWorldBoundsData(std::fstream &file, sf::FloatRect &bounds, const DataTagPair &boundsTag);
void saveTileMapData(std::fstream &file, TileMap &map);
void saveBackgroundData(std::fstream &file, BackgroundManager &manager);
void saveEnemySpawnerCollection(std::fstream &file, EnemySpawnerCollection &collection, const DataTagPair &spawnerTag);//used for both boss and non boss spawners so specify what tag should be usd
void saveEnemySpawnPoints(std::fstream &file, std::vector<std::shared_ptr<SpawnPoint> > &spawnPoints, const DataTagPair &enemyClassificationTag);
void saveDestructibleBlocks(std::fstream &file, std::vector<std::shared_ptr<DestructibleBlock> > &destructibleBlocks);
void savePlayerSpawnPosition(std::fstream &file, const glm::vec2 &spawnPosition);

void loadWorld(const std::string &worldName, GameWorld &world);

void loadWorldBoundsData(std::fstream &file, sf::FloatRect &bounds, const DataTagPair &boundsTag);
void loadTileMapData(std::fstream &file, TileMap &map, glm::vec2 worldSize);
void loadBackgroundData(std::fstream &file, BackgroundManager &manager, sf::FloatRect worldSize);
void loadEnemySpawnerCollection(std::fstream &file, EnemySpawnerCollection &collection, const DataTagPair &spawnerTag);//used for both boss and non boss spawners so specify what tag should be usd
void loadEnemySpawnPoints(std::fstream &file, std::vector<std::shared_ptr<SpawnPoint> > &spawnPoints, const DataTagPair &enemyClassificationTag);
void loadDestructibleBlocks(std::fstream &file, std::vector<std::shared_ptr<DestructibleBlock> > &destructibleBlocks);
void loadPlayerSpawnPosition(std::fstream &file, glm::vec2 &spawnPosition);
#endif // WORLDSAVINGLOADING_H_INCLUDED
