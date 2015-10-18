#ifndef PRELOADEDDATA_H_INCLUDED
#define PRELOADEDDATA_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Direction.h"
#include "ObjectTypes.h"
#include "ObjectTypeToFilename.h"
#include <string>
#include <map>
#include <vector>
#include <functional>

//characteristics of an enemy that is changed when it turns into a boss
struct BossProperties {

    float scale;
    int health;
};

struct PreloadedData {

    int health;

    std::string textureFilename;
    sf::Time animationNextFrameTime;

    float scale = 1.0f;

    //all entities have a default hitbox state, which is the hitbox they use for almost all animation states when colliding with the environment
    unsigned defaultHitboxState = 0;

    //texture rect for each animation state
    std::map<unsigned, std::vector<sf::IntRect> > animationTextureRects;

    //hurtbox for each animation state
    std::map<unsigned, std::vector<sf::FloatRect> > hurtboxes;

    //hitboxes for the different states of the entity
    std::map<unsigned, std::vector<sf::FloatRect> > hitboxes;
};

struct ShootingEntityPreloadedData : public PreloadedData{

    GunType gunType;
    sf::Time gunfireDelay;
    BulletType bulletType;

    std::map<unsigned, glm::vec2> bulletOriginForState;
};

struct PreloadedPlayerData : public ShootingEntityPreloadedData {

    unsigned jumpingHitboxState = 1;
};

struct PreloadedTurretData : public ShootingEntityPreloadedData {

    sf::Time hiddenStateDuration;
    sf::Time exposedStateDuration;
};

struct PreloadedOmniDirectionalTurretData : public ShootingEntityPreloadedData {

    sf::Time hiddenStateDuration;
    sf::Time exposedStateDuration;
    sf::Time shootingDelay;//shooting delay is different from fire delay, firedelay is delay to fire a second bullet
    //shooting delay is delay to shoot next SET of bullets
};

//struct to store preloaded data about an enemy so whenever a new enemy is created you don't have to open a file to retrieve info about it
struct PreloadedEnemyData : public PreloadedData{

    //empty for now
};

struct PreloadedDestructibleBlockData : public PreloadedData {

    //empty for now
};

struct PreloadedBulletData : public PreloadedData {

    float velocity;
    sf::Time lifetime;
};

struct PreloadedPowerUpData : public PreloadedData {

    //empty for now
};

struct PreloadedDataCollection {

    public:

        //returns cached data
        //returns null pointer on failure
        const PreloadedEnemyData* getBasicEnemyData(const EnemyType &enemyType);
        const PreloadedTurretData* getTurretEnemyData(const EnemyType &enemyType);
        const PreloadedOmniDirectionalTurretData* getOmnidirectionalTurretData(const EnemyType &enemyType);

        const PreloadedBulletData* getBulletData(const BulletType &bulletType);

        const PreloadedDestructibleBlockData* getDestructibleBlockData(const DestructibleBlockType &blockType);

        const PreloadedPowerUpData* getPowerUpData(const PowerUpType &powerUpType);

        const PreloadedPlayerData* getPlayerData(const int &playerNumber);

        const BossProperties* getBossData(const EnemyType &enemyType);

    private:

        //collection of preloaded data paired with the entity whose data they represent
        //since enemy types are all stored in one enum even though there are different preloadeddata types for different classes of enemies
        //you have to make sure to store enemy types and preloaded data of a specific class of enemy in the correct container
        std::map<EnemyType, PreloadedEnemyData> basicEnemyData;
        std::map<EnemyType, PreloadedTurretData> turretEnemyData;
        std::map<EnemyType, PreloadedOmniDirectionalTurretData> omnidirectionalTurretData;

        std::map<BulletType, PreloadedBulletData> bulletData;

        std::map<DestructibleBlockType, PreloadedDestructibleBlockData> destructibleBlockData;

        std::map<PowerUpType, PreloadedPowerUpData> powerUpData;

        std::map<int, PreloadedPlayerData> playerData;

        //matches a given enemy type to the scaling factor applied to it if the enemy if upgraded to a boss type
        //that is, if this type of enemy were to be used as a boss, how much should it be scaled by
        //key is the enemy type, value gives the scaling applied
        std::map<EnemyType, BossProperties> bossData;
};

//since all of the get data functions in the preloadedDataCollection follow the same format
//create a template function that generalizes the procedure for any given type
template<class DataType, class PreloadedData>
const PreloadedData* getData(std::map<DataType, PreloadedData> &data, const DataType &dataType,
                             std::function<bool(PreloadedData&, const std::string&)> dataLoader) {

    if(data.count(dataType) != 0) {

        return &data[dataType];
    }

    //create an empty cache for the given type
    data[dataType];
    std::string dataFilename = getFilenameForData(dataType);

    if(!dataLoader(data[dataType], dataFilename)) {

        data.erase(dataType);
        return nullptr;
    }

    return &data[dataType];
}

extern PreloadedDataCollection dataCollection;

#endif // PRELOADEDDATA_H_INCLUDED
