#include "PreloadedData.h"
#include "DataLoaders.h"

using std::string;
using std::function;

PreloadedDataCollection dataCollection;

const PreloadedEnemyData *PreloadedDataCollection::getBasicEnemyData(const EnemyType &enemyType) {

    function<bool(PreloadedEnemyData&, const string&)> loadingFunction = loadEnemyData;
    return getData(basicEnemyData, enemyType, loadingFunction);
}

const PreloadedTurretData *PreloadedDataCollection::getTurretEnemyData(const EnemyType &enemyType) {

    function<bool(PreloadedTurretData&, const string&)> loadingFunction = loadTurretData;
    return getData(turretEnemyData, enemyType, loadingFunction);
}

const PreloadedOmniDirectionalTurretData *PreloadedDataCollection::getOmnidirectionalTurretData(const EnemyType &enemyType) {

    function<bool(PreloadedOmniDirectionalTurretData&, const string&)> loadingFunction = loadOmniDirectionalTurretData;
    return getData(omnidirectionalTurretData, enemyType, loadingFunction);
}

const PreloadedBulletData *PreloadedDataCollection::getBulletData(const BulletType &bulletType) {

    function<bool(PreloadedBulletData&, const string&)> loadingFunction = loadBulletData;
    return getData(bulletData, bulletType, loadingFunction);
}

const PreloadedDestructibleBlockData *PreloadedDataCollection::getDestructibleBlockData(const DestructibleBlockType &blockType) {

    function<bool(PreloadedDestructibleBlockData&, const string&)> loadingFunction = loadDestrutibleBlockData;
    return getData(destructibleBlockData, blockType, loadingFunction);
}

const PreloadedPowerUpData *PreloadedDataCollection::getPowerUpData(const PowerUpType &powerUpType) {

    function<bool(PreloadedPowerUpData&, const string&)> loadingFunction = loadPowerUpData;
    return getData(powerUpData, powerUpType, loadingFunction);
}

const BossProperties *PreloadedDataCollection::getBossData(const EnemyType &enemyType) {

    ///function<bool(&, const string&)> loadingFunction = loadBossData;
    ///return getData(BossData, enemyType, loadingFunction);
    ///for now just let the load boss data function do all the work
    ///later on, change the load boss data function so that it loads the boss data for only a specific enemy, rather than all of them
    if(bossData.count(enemyType) == 0) {

        loadBossData(bossData, "asdf");
    }

    return &bossData[enemyType];
}
