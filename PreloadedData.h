#ifndef PRELOADEDDATA_H_INCLUDED
#define PRELOADEDDATA_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Direction.h"
#include <string>
#include <map>
#include <vector>

enum EnemyType : int {

    ENEMY_GOOMBA,
    ENEMY_PIRANHA,
    ENEMY_MUSHROOM,
};

enum BulletType : int {

    BULLET_SLOW,
    BULLET_FAST,
    BULLET_MEDIUM,
};

enum GunType {

    GUN_BASIC
};

enum PowerUpType {

    MACHINE_GUN
};

//characteristics of an enemy that is changed when it turns into a boss
struct BossProperties {

    float scale;
    int health;
};

//matches a given enemy type to the scaling factor applied to it if the enemy if upgraded to a boss type
//that is, if this type of enemy were to be used as a boss, how much should it be scaled by
//key is the enemy type, value gives the scaling applied
typedef std::map<EnemyType, BossProperties> BossData;

struct PreloadedData {

    int health;

    std::string textureFileName;
    sf::Time animationNextFrameTime;

    float scale = 1.0f;

    //texture rect for each animation state
    std::map<unsigned, std::vector<sf::IntRect> > animationTextureRects;

    //collisionbox for each animation state
    std::map<unsigned, std::vector<sf::FloatRect> > collisionboxes;
};

struct ShootingEntityPreloadedData : public PreloadedData{

    GunType gunType;
    sf::Time gunfireDelay;
    BulletType bulletType;

    std::map<unsigned, glm::vec2> bulletOriginForState;
};

struct PreloadedPlayerData : public ShootingEntityPreloadedData {

    unsigned STATE_STANDING_LEFT;
    unsigned STATE_STANDING_UP_FACING_LEFT;
    unsigned STATE_STANDING_UP_FACING_RIGHT;
    unsigned STATE_STANDING_RIGHT;

    unsigned STATE_WALKING_DOWN_LEFT;
    unsigned STATE_WALKING_LEFT;
    unsigned STATE_WALKING_UP_LEFT;
    unsigned STATE_WALKING_UP_RIGHT;
    unsigned STATE_WALKING_RIGHT;
    unsigned STATE_WALKING_DOWN_RIGHT;

    unsigned STATE_CROUCHING_LEFT;
    unsigned STATE_CROUCHING_RIGHT;

    unsigned STATE_FALLING_DOWN_FACING_LEFT;
    unsigned STATE_FALLING_DOWN_FACING_RIGHT;
    unsigned STATE_FALLING_DOWN_LEFT;
    unsigned STATE_FALLING_LEFT;
    unsigned STATE_FALLING_UP_LEFT;
    unsigned STATE_FALLING_UP_FACING_LEFT;
    unsigned STATE_FALLING_UP_FACING_RIGHT;
    unsigned STATE_FALLING_UP_RIGHT;
    unsigned STATE_FALLING_RIGHT;
    unsigned STATE_FALLING_DOWN_RIGHT;

    unsigned STATE_JUMPING;

    unsigned STATE_DYING_FACING_LEFT;
    unsigned STATE_DYING_FACING_RIGHT;

    unsigned STATE_DEAD;
};

struct PreloadedTurretData : public ShootingEntityPreloadedData {

    unsigned STATE_HIDING, STATE_COMING_OUT_OF_HIDING, STATE_GOING_INTO_HIDING, STATE_SHOOTING;

    //frames of the shooting animation
    unsigned DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT;

    sf::Time hiddenStateDuration;
    sf::Time exposedStateDuration;
};

struct PreloadedOmniDirectionalTurretData : public ShootingEntityPreloadedData {

    unsigned STATE_HIDING, STATE_COMING_OUT_OF_HIDING, STATE_GOING_INTO_HIDING, STATE_EXPOSED, STATE_SHOOTING;

    sf::Time hiddenStateDuration;
    sf::Time exposedStateDuration;
    sf::Time shootingDelay;
};

//struct to store preloaded data about an enemy so whenever a new enemy is created you don't have to open a file to retrieve info about it
struct PreloadedEnemyData : public PreloadedData{

    unsigned STATE_WALKING_LEFT;
    unsigned STATE_WALKING_RIGHT;
    unsigned STATE_FALLING_LEFT;
    unsigned STATE_FALLING_RIGHT;
};

struct PreloadedDestructibleBlockData : public PreloadedData {

    unsigned STATE_SOLID, STATE_DESTROYING, STATE_DESTROYED;
};

struct PreloadedBulletData : public PreloadedData {

    unsigned STATE_RIGHT, STATE_UP_RIGHT, STATE_UP, STATE_UP_LEFT, STATE_LEFT, STATE_DOWN_LEFT, STATE_DOWN, STATE_DOWN_RIGHT;

    float velocity;
    sf::Time lifetime;
};

struct PreloadedPowerUpData : public PreloadedData {

    unsigned STATE_SPAWNING;
    unsigned STATE_SPAWNED;
    unsigned STATE_DISAPPEARING;
    unsigned STATE_DISAPPEARED;

    //each frame of the spawned animation state refers to a different powerup image
    unsigned FRAME_MACHINEGUN;
};

struct PreloadedDataCollection {

    PreloadedEnemyData goombaData;
    PreloadedTurretData piranhaData;
    PreloadedOmniDirectionalTurretData mushroomData;
    PreloadedBulletData slowBulletData;
    PreloadedBulletData mediumBulletData;
    PreloadedBulletData fastBulletData;
    PreloadedDestructibleBlockData basicDestructibleBlockData;
    PreloadedPowerUpData powerUpData;

    BossData bossData;
};

extern PreloadedDataCollection dataCollection;

#endif // PRELOADEDDATA_H_INCLUDED
