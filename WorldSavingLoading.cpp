#include "WorldSavingLoading.h"
#include "GameWorld.h"
#include "Tile.h"
#include "FileManipulators.h"
#include "StringManipulator.h"
#include "BackgroundManager.h"
#include "SpawnerData.h"
#include "PreloadedData.h"
#include "ObjectTypes.h"
#include "DestructibleBlock.h"

#include <sstream>
#include <vector>
#include <memory>

using std::stringstream;
using std::vector;
using std::shared_ptr;
using std::string;
using std::fstream;

const string savedWorldDataPath("data/levels/");
const string saveFileExtention(".txt");

void saveWorld(const string& worldName, GameWorld &world) {

    fstream file;

    string fileName = savedWorldDataPath + worldName + saveFileExtention;
    file.open(savedWorldDataPath + worldName + saveFileExtention, std::ios_base::out | std::ios_base::trunc);

    if(!file) {

        cout << "Failed to open level \"" << worldName << "\" ";
        cout << "with file path \"" << fileName << "\".";
        return;
    }

    //save each aspect of the world
    saveWorldBoundsData(file, world.worldBoundsDefault, worldBoundsDefaultTag);
    saveWorldBoundsData(file, world.worldBoundsBossFight, worldBoundsBossFightTag);
    saveTileMapData(file, world.tileMap);
    saveBackgroundData(file, world.backgrounds);
    saveEnemySpawnerCollection(file, world.nonBossEnemySpawners, nonBossEnemySpawnerTag);
    saveEnemySpawnerCollection(file, world.bossEnemySpawners, bossEnemySpawnerTag);
    saveDestructibleBlocks(file, world.destructibleBlocks);
    savePlayerSpawnPosition(file, world.initialPlayerSpawnPoint);

    file.close();
}

void saveWorldBoundsData(std::fstream &file, sf::FloatRect &bounds, const DataTagPair &tagPair) {

    file << tagPair.first << endl;

    file << bounds.left << " " << bounds.top << " " << bounds.width << " " << bounds.height << endl;

    file << tagPair.second << endl;
}

void saveTileMapData(fstream &file, TileMap &map) {

    file << tileMapTag.first << endl;

    auto tiles = map.getAllTiles();

    //then property of each tile
    for(auto &tile : tiles) {

        sf::FloatRect boundingBox = tile->getBoundingBox();
        TileType type = tile->getType();
        string tileTextureName = tile->getTextureName();
        sf::IntRect textureRect = tile->getTextureRect();

        //save the tile data required for the tilemap to create a tile
        //only needs position, tyletype, texture, and texture rect
        //save order is: x position, y position, type, texture name,
        //texture rect x, texture rect y, texture rect width, texture rect height
        file << boundingBox.left << " " << boundingBox.top << " ";
        file << (int)type << " " << tileTextureName << " ";
        file << textureRect.left << "  " << textureRect.top << "  " << textureRect.width << "  " << textureRect.height << endl;
    }

    //tile data saved
    file << tileMapTag.second << endl;
}

void saveBackgroundData(std::fstream &file, BackgroundManager &manager) {

    file << backgroundTag.first << endl;

    auto backgroundSaveData = manager.getDataForSaving();

    //save all background filenames and their associated distance from view
    for(auto &data : backgroundSaveData) {

        //first in pair is the background filename, second is the distance from view
        file << data.first << " " << data.second << endl;
    }

    file << backgroundTag.second << endl;
}

void saveEnemySpawnerCollection(std::fstream &file, EnemySpawnerCollection &collection, const DataTagPair &spawnerTag) {

    file << spawnerTag.first << endl;

    //for each spawner collection save each spawner type along with its tags
    //start with the normal enemies
    saveEnemySpawnPoints(file, collection.enemySpawnInfo.spawnPoints, basicEnemySpawnerTag);
    saveEnemySpawnPoints(file, collection.turretSpawnInfo.spawnPoints, turretEnemySpawnerTag);
    saveEnemySpawnPoints(file, collection.omnidirectionalTurretSpawnInfo.spawnPoints, omnidirectionalTurretSpawnerTag);

    file << spawnerTag.second << endl;
}

void saveEnemySpawnPoints(std::fstream &file, vector<shared_ptr<SpawnPoint> > &spawnPoints, const DataTagPair &enemyClassificationTag) {

    file << enemyClassificationTag.first << endl;

    for(auto &spawnPoint : spawnPoints) {

        //save data in following order, each peice of data seperated by a space
        //max # of enemies spawned, spawn position, spawn delay in MILLISECONDS, type of enemy spawned
        file << spawnPoint->getMaxEnemiesSpawned() << " ";
        file << spawnPoint->getSpawnPosition().x << " " << spawnPoint->getSpawnPosition().y << " ";
        file << spawnPoint->getSpawnDelayInMilliseconds() << " ";
        file << (int)spawnPoint->getTypeOfEnemySpawned() << endl;
    }

    file << enemyClassificationTag.second << endl;
}

void saveDestructibleBlocks(fstream &file, vector<shared_ptr<DestructibleBlock> > &destructibleBlocks) {

    file << destructibleBlocksTag.first << endl;

    for(auto &block : destructibleBlocks) {

        glm::vec2 position = block->getPosition();
        DestructibleBlockType type = block->getType();

        file << position.x << " " << position.y << " " << type << endl;
    }

    file << destructibleBlocksTag.second << endl;
}

void savePlayerSpawnPosition(std::fstream &file, const glm::vec2 &spawnPosition) {

    file << playerSpawnTag.first << endl;

    file << spawnPosition.x << " " << spawnPosition.y << endl;

    file << playerSpawnTag.second << endl;
}

void loadWorld(const std::string &worldName, GameWorld &world) {

    fstream file;

    string fileName = savedWorldDataPath + worldName + saveFileExtention;
    file.open(savedWorldDataPath + worldName + saveFileExtention, std::ios_base::in);

    if(!file) {

        cout << "Failed to open level \"" << worldName << "\" ";
        cout << "with file path \"" << fileName << "\".";
        return;
    }

    loadWorldBoundsData(file, world.worldBoundsDefault, worldBoundsDefaultTag);
    loadWorldBoundsData(file, world.worldBoundsBossFight, worldBoundsBossFightTag);
    loadTileMapData(file, world.tileMap, glm::vec2(world.worldBoundsDefault.width, world.worldBoundsDefault.height));
    loadBackgroundData(file, world.backgrounds, world.worldBoundsDefault);
    loadEnemySpawnerCollection(file, world.nonBossEnemySpawners, nonBossEnemySpawnerTag);
    loadEnemySpawnerCollection(file, world.bossEnemySpawners, bossEnemySpawnerTag);
    loadDestructibleBlocks(file, world.destructibleBlocks);
    loadPlayerSpawnPosition(file, world.initialPlayerSpawnPoint);

    //enter all blocks into the hash
    for(unsigned i = 0; i < world.destructibleBlocks.size(); ++i) {

        world.destructibleBlockHash.insert(world.destructibleBlocks[i]);
    }

    //begin using the default world bounds
    world.worldBoundsInUse = world.worldBoundsDefault;

    file.close();
}

void loadWorldBoundsData(std::fstream &file, sf::FloatRect &bounds, const DataTagPair &boundsTag) {

    if(!readAfterLine(file, boundsTag.first)) {

        cout << "Failed to find bounds data" << endl;
        return;
    }

    string extractedData;
    getline(file, extractedData);

    //save format is "xPos yPos width height"
    //extract each aspect of the world bounds
    bounds.left = atof(extractFirstWordInString(extractedData).c_str());
    bounds.top = atof(extractFirstWordInString(extractedData).c_str());
    bounds.width = atof(extractFirstWordInString(extractedData).c_str());
    bounds.height = atof(extractFirstWordInString(extractedData).c_str());

    //data is loaded
}

void loadTileMapData(std::fstream &file, TileMap &map, glm::vec2 worldSize) {

    if(!readAfterLine(file, tileMapTag.first)) {

        cout << "Failed to find tilemap data" << endl;
        return;
    }

    //load data about each tile and set the tile using the tilemap
    map.resize(worldSize.x, worldSize.y);

    string extracted;
    getline(file, extracted);

    while(extracted != tileMapTag.second && file) {

        //extract each peice of data from the line and use it to create a tile
        sf::Vector2f position;

        position.x = atoi(extractFirstWordInString(extracted).c_str());

        position.y = atoi(extractFirstWordInString(extracted).c_str());

        TileType type = (TileType)atoi(extractFirstWordInString(extracted).c_str());

        string textureFilename = extractFirstWordInString(extracted);

        sf::IntRect textureRect;

        textureRect.left = atoi(extractFirstWordInString(extracted).c_str());
        textureRect.top = atoi(extractFirstWordInString(extracted).c_str());
        textureRect.width = atoi(extractFirstWordInString(extracted).c_str());
        textureRect.height = atoi(extractFirstWordInString(extracted).c_str());

        map.setTile(position, type, textureFilename, textureRect);

        extracted = "";

        getline(file, extracted);
    }

    map.displayChangedTiles();

    //data is loaded
}

void loadBackgroundData(fstream &file, BackgroundManager &manager, sf::FloatRect worldSize) {

    if(!readAfterLine(file, backgroundTag.first)) {

        cout << "failed to find background data" << endl;
        return;
    }

    manager.clearBackgrounds();

    string extracted;
    getline(file, extracted);

    while(extracted != backgroundTag.second && file) {

        //load each background file name and insert the background
        string backgroundFilename = extractFirstWordInString(extracted);

        float distanceFromView = atof(extractFirstWordInString(extracted).c_str());

        manager.insertBackground(backgroundFilename, distanceFromView, worldSize);

        extracted = "";
        getline(file, extracted);
    }
}

void loadEnemySpawnerCollection(std::fstream &file, EnemySpawnerCollection &collection, const DataTagPair &spawnerTag) {

    if(!readAfterLine(file, spawnerTag.first)) {

        cout << "failed to find spawner collection data" << endl;
        return;
    }

    //load each type of spawnpoints
    ///LOADING MUST OCCUR IN THE SAME ORDER AS THE DATA WAS SAVED
    ///BECAUSE THE LOADING FUNCTION RELIES ON THE FILE READING TO PLACE THE STREAM READER AT THE CORRECT POSITION AFTER LOADING EACH TYPE OF ENEMY SPAWNER
    ///AFTER THE FIRST TYPE OF ENEMY IS LOADED, THE CURSOR SHOULD BE PLACED AT THE BEGINNING OF THE DATA FOR THE SECOND TYPE OF ENEMY
    ///DO NOT CHANGE LOADING ORDER
    loadEnemySpawnPoints(file, collection.enemySpawnInfo.spawnPoints, basicEnemySpawnerTag);
    loadEnemySpawnPoints(file, collection.turretSpawnInfo.spawnPoints, turretEnemySpawnerTag);
    loadEnemySpawnPoints(file, collection.omnidirectionalTurretSpawnInfo.spawnPoints, omnidirectionalTurretSpawnerTag);
}

void loadEnemySpawnPoints(std::fstream &file, std::vector<std::shared_ptr<SpawnPoint> > &spawnPoints, const DataTagPair &enemyClassificationTag) {

    //if the first line read isn't the beginning of the classification tag, then the file is in the wrong order
    //so you cannot load the spawner data
    string extractedData;
    getline(file, extractedData);

    if(extractedData != enemyClassificationTag.first) {

        cout << "Could not load enemy spawn points" << endl;
        return;
    }

    //now load the data for the given enemy type
    getline(file, extractedData);

    while(extractedData != enemyClassificationTag.second && file) {

        //max # of enemies spawned, spawn position, spawn delay in MILLISECONDS, type of enemy spawned
        unsigned enemyCount = 0;
        glm::vec2 spawnPosition;
        unsigned spawnDelayMilliseconds = 0;
        EnemyType enemyType = EnemyType::ENEMY_GOOMBA;

        enemyCount = atoi(extractFirstWordInString(extractedData).c_str());
        spawnPosition.x = atoi(extractFirstWordInString(extractedData).c_str());
        spawnPosition.y = atoi(extractFirstWordInString(extractedData).c_str());
        spawnDelayMilliseconds = atoi(extractFirstWordInString(extractedData).c_str());
        enemyType = (EnemyType)atoi(extractFirstWordInString(extractedData).c_str());

        shared_ptr<SpawnPoint> spawnPoint = std::make_shared<SpawnPoint>(spawnPosition, sf::milliseconds(spawnDelayMilliseconds), enemyType, enemyCount);
        spawnPoints.push_back(spawnPoint);

        extractedData = "";
        getline(file, extractedData);
    }
}

void loadDestructibleBlocks(std::fstream &file, std::vector<std::shared_ptr<DestructibleBlock> > &destructibleBlocks) {

    if(!readAfterLine(file, destructibleBlocksTag.first)) {

        cout << "failed to find destructible blocks data" << endl;
        return;
    }

    string extractedData;
    getline(file, extractedData);

    while(extractedData != destructibleBlocksTag.second && file) {

        glm::vec2 position;
        DestructibleBlockType blockType;

        position.x = atoi(extractFirstWordInString(extractedData).c_str());
        position.y = atoi(extractFirstWordInString(extractedData).c_str());

        blockType = (DestructibleBlockType)atoi(extractFirstWordInString(extractedData).c_str());
        auto blockData = dataCollection.getDestructibleBlockData(blockType);

        if(blockData) {

            auto block = std::make_shared<DestructibleBlock>(position, blockType, *blockData);
            destructibleBlocks.push_back(block);
        }

        extractedData = "";
        getline(file, extractedData);
    }
}

void loadPlayerSpawnPosition(std::fstream &file, glm::vec2 &spawnPosition) {

    if(!readAfterLine(file, playerSpawnTag.first)) {

        cout << "failed to find player spawn position data" << endl;
        return;
    }

    string extractedData;
    getline(file, extractedData);

    while(extractedData != playerSpawnTag.second && file) {

        spawnPosition.x = atoi(extractFirstWordInString(extractedData).c_str());
        spawnPosition.y = atoi(extractFirstWordInString(extractedData).c_str());

        extractedData = "";
        getline(file, extractedData);
    }
}
