#include <iostream>
#include "DataLoaders.h"
#include "PreloadedData.h"
#include "EntityAnimationStates.h"
#include "Gun.h"
#include "EntitySavedataTags.h"
#include "FileManipulators.h"
#include "StringManipulator.h"
#include "cstdlib"

using std::cout;
using std::endl;
using std::fstream;
using std::map;
using std::string;

bool loadBossData(map<EnemyType, BossProperties> &bossData, const std::string &dataFilename) {

    bossData[EnemyType::ENEMY_GOOMBA].scale = 2.f;
    bossData[EnemyType::ENEMY_GOOMBA].health = 5;

    bossData[EnemyType::ENEMY_PIRANHA].scale = 2.f;
    bossData[EnemyType::ENEMY_PIRANHA].health = 13.f;

    bossData[EnemyType::ENEMY_MUSHROOM].scale = 2.f;
    bossData[EnemyType::ENEMY_MUSHROOM].health = 20.f;

    return true;
}

bool loadIntegerParameter(std::fstream &file, int &loadedData, const DataTagPair &tagPair) {

    if(!readAfterLine(file, tagPair.first)) {

        return false;
    }

    string extractedData;
    getline(file, extractedData);

    if(extractedData == tagPair.second) {

        return false;
    }

    loadedData = atoi(extractFirstWordInString(extractedData).c_str());
    return true;
}

bool loadEntityHealth(fstream &file, int &health) {

    //find the health data
    return loadIntegerParameter(file, health, entityHealthTag);
}

bool loadTextureFilename(fstream &file, string &textureFilename) {

    if(!readAfterLine(file, entityTextureFilenameTag.first)) {

        return false;
    }

    string extractedData;
    getline(file, extractedData);

    //no data loaded
    if(extractedData == entityTextureFilenameTag.second) {

        return false;
    }

    textureFilename = extractedData;
    return true;
}

bool loadAnimationNextFrameTime(fstream &file, sf::Time &animationNextFrameTime) {

    int timeInMilliseconds = 0;

    if(!loadIntegerParameter(file, timeInMilliseconds, entityAnimationNextFrameTimeTag)) {

        return false;
    }

    animationNextFrameTime = sf::milliseconds(timeInMilliseconds);
    return true;
}

bool loadEntityScale(std::fstream &file, float &scale, const DataTagPair &tagPair) {

    if(!readAfterLine(file, tagPair.first)) {

        return false;
    }

    string extractedData;
    getline(file, extractedData);

    if(extractedData == tagPair.second) {

        return false;
    }

    scale = atof(extractFirstWordInString(extractedData).c_str());
    return true;
}

bool loadEntityGunType(std::fstream &file, GunType &gunType) {

    int type = -1;

    if(!loadIntegerParameter(file, type, entityGunTypeTag)) {

        return false;
    }

    gunType = static_cast<GunType>(type);

    return true;
}

bool loadEntityGunfireDelay(std::fstream &file, sf::Time &gunfireDelay) {

    int delay = 0;

    if(!loadIntegerParameter(file, delay, entityGunfireDelayTag)) {

        return false;
    }

    gunfireDelay = sf::milliseconds(delay);

    return true;
}

bool loadEntityBulletType(std::fstream &file, BulletType &bulletType) {

    int type = 0;

    if(!loadIntegerParameter(file, type, entityGunBulletTypeTag)) {

        return false;
    }

    bulletType = static_cast<BulletType>(bulletType);

    return true;
}

bool loadEntityBulletOrigin(std::fstream &file, std::map<unsigned, glm::vec2> &bulletOriginForState) {

    if(!readAfterLine(file, entityBulletOriginTag.first)) {

        return false;
    }

    string extractedData;
    getline(file, extractedData);

    while(extractedData != entityBulletOriginTag.second) {

        //first number is the state for the bulllet
        //next 2 numbers are the positions
        unsigned state = 0;
        glm::vec2 position;

        state = atoi(extractFirstWordInString(extractedData).c_str());
        position.x = atof(extractFirstWordInString(extractedData).c_str() );
        position.y = atof(extractFirstWordInString(extractedData).c_str() );

        bulletOriginForState[state] = position;

        extractedData = "";
        getline(file, extractedData);
    }

    return true;
}

bool loadEntityTextureRects(std::fstream &file, std::map<unsigned, std::vector<sf::IntRect> > &textureRects) {

    if(!readAfterLine(file, entityAnimationTextureRectsTag.first)) {

        return false;
    }

    string extractedData;
    getline(file, extractedData);

    while(extractedData != entityAnimationTextureRectsTag.second) {

        //first number is the state
        //next 4 numbers are the properties of the texture rect
        unsigned state = 0;
        sf::IntRect textureRect;

        state = atoi(extractFirstWordInString(extractedData).c_str());
        textureRect.left = atof(extractFirstWordInString(extractedData).c_str() );
        textureRect.top = atof(extractFirstWordInString(extractedData).c_str() );
        textureRect.width = atof(extractFirstWordInString(extractedData).c_str() );
        textureRect.height = atof(extractFirstWordInString(extractedData).c_str() );

        textureRects[state].push_back(textureRect);

        extractedData = "";
        getline(file, extractedData);
    }

    return true;
}

bool loadEntityCollisionBoxes(std::fstream &file, std::map<unsigned, std::vector<sf::FloatRect> > &collisionBoxes, const DataTagPair &tagPair) {

    if(!readAfterLine(file, tagPair.first)) {

        return false;
    }

    string extractedData;
    getline(file, extractedData);

    while(extractedData != tagPair.second) {

        //first number is the state
        //next 4 numbers are the properties of the texture rect
        unsigned state = 0;
        sf::FloatRect collisionRect;

        state = atoi(extractFirstWordInString(extractedData).c_str());
        collisionRect.left = atof(extractFirstWordInString(extractedData).c_str() );
        collisionRect.top = atof(extractFirstWordInString(extractedData).c_str() );
        collisionRect.width = atof(extractFirstWordInString(extractedData).c_str() );
        collisionRect.height = atof(extractFirstWordInString(extractedData).c_str() );

        collisionBoxes[state].push_back(collisionRect);

        extractedData = "";
        getline(file, extractedData);
    }

    return true;
}

bool loadPlayerData(PreloadedPlayerData &data, const std::string &dataFilename) {

    fstream file;
    file.open(dataFilename, std::ios_base::in);

    if(!file) {

        cout << "Failed to find file: " << dataFilename << endl;
        return false;
    }

    if(!loadEntityHealth(file, data.health)) {

        cout << "Failed to load health" << endl;
        return false;
    }

    if(!loadEntityScale(file, data.scale, entityNormalFormScaleTag)) {

        cout << "Failed to load scaling data" << endl;
        return false;
    }

    if(!loadTextureFilename(file, data.textureFilename)) {

        cout << "Failed to load texture file name." << endl;
        return false;
    }

    if(!loadAnimationNextFrameTime(file, data.animationNextFrameTime)) {

        cout << "Failed to load animation next frame time" << endl;
        return false;
    }

    if(!loadEntityGunType(file, data.gunType)) {

        cout << "Failed to load entity gun type" << endl;
        return false;
    }

    if(!loadEntityGunfireDelay(file, data.gunfireDelay)) {

        cout << "Failed to load entity gunfire delay" << endl;
        return false;
    }

    if(!loadEntityBulletType(file, data.bulletType)) {

        cout << "Failed to load entity bullet type" << endl;
        return false;
    }

    if(!loadEntityBulletOrigin(file, data.bulletOriginForState)) {

        cout << "Failed to load entity bullet origin" << endl;
        return false;
    }

    if(!loadEntityTextureRects(file, data.animationTextureRects)) {

        cout << "Failed to load entity texture rects" << endl;
        return false;
    }

    if(!loadEntityCollisionBoxes(file, data.hitboxes, entityHitboxTag)) {

        cout << "Failed to load hitboxes" << endl;
        return false;
    }

    if(!loadEntityCollisionBoxes(file, data.hurtboxes, entityHurtboxTag)) {

        cout << "Failed to load hurtboxes" << endl;
        return false;
    }

    return true;

//    data.health = 3;
//    data.textureFilename = "player.png";
//    data.animationNextFrameTime = sf::milliseconds(100);
//
//    data.gunType = GunType::GUN_BASIC;
//    data.gunfireDelay = FIRE_DELAY_MEDIUM;
//    data.bulletType = BulletType::BULLET_MEDIUM;
//
//    data.bulletOriginForState[PlayerEnums::STATE_STANDING_LEFT] = glm::vec2(0, 57);
//    data.bulletOriginForState[PlayerEnums::STATE_STANDING_RIGHT] = glm::vec2(126, 62);
//    data.bulletOriginForState[PlayerEnums::STATE_STANDING_UP_FACING_LEFT] = glm::vec2(73, 0);
//    data.bulletOriginForState[PlayerEnums::STATE_STANDING_UP_FACING_RIGHT] = glm::vec2(48, 0);
//
//    data.bulletOriginForState[PlayerEnums::STATE_WALKING_DOWN_LEFT] = glm::vec2(15, 128);
//    data.bulletOriginForState[PlayerEnums::STATE_WALKING_LEFT] = glm::vec2(0, 56);
//    data.bulletOriginForState[PlayerEnums::STATE_WALKING_UP_LEFT] = glm::vec2(32, 14);
//    data.bulletOriginForState[PlayerEnums::STATE_WALKING_UP_RIGHT] = glm::vec2(109, 10);
//    data.bulletOriginForState[PlayerEnums::STATE_WALKING_RIGHT] = glm::vec2(128, 61);
//    data.bulletOriginForState[PlayerEnums::STATE_WALKING_DOWN_RIGHT] = glm::vec2(114, 128);
//
//    data.bulletOriginForState[PlayerEnums::STATE_CROUCHING_LEFT] = glm::vec2(-5, 102);
//    data.bulletOriginForState[PlayerEnums::STATE_CROUCHING_RIGHT] = glm::vec2(128, 104);
//
//    data.bulletOriginForState[PlayerEnums::STATE_FALLING_DOWN_FACING_LEFT] = glm::vec2(37, 112);
//    data.bulletOriginForState[PlayerEnums::STATE_FALLING_DOWN_FACING_RIGHT] = glm::vec2(51, 114);
//    data.bulletOriginForState[PlayerEnums::STATE_FALLING_RIGHT] = glm::vec2(128, 51);
//    data.bulletOriginForState[PlayerEnums::STATE_FALLING_DOWN_RIGHT] = glm::vec2(128, 110);
//    data.bulletOriginForState[PlayerEnums::STATE_FALLING_UP_RIGHT] = glm::vec2(120, 9);
//    data.bulletOriginForState[PlayerEnums::STATE_FALLING_UP_FACING_RIGHT] = glm::vec2(60, 0);
//    data.bulletOriginForState[PlayerEnums::STATE_FALLING_UP_FACING_LEFT] = glm::vec2(74, 0);
//    data.bulletOriginForState[PlayerEnums::STATE_FALLING_UP_LEFT] = glm::vec2(0, 0);
//    data.bulletOriginForState[PlayerEnums::STATE_FALLING_DOWN_LEFT] = glm::vec2(13, 108);
//    data.bulletOriginForState[PlayerEnums::STATE_FALLING_LEFT] = glm::vec2(0, 47);
//
//    //bullet origin for each direction the player could face
//    //this is only usedw hen player is jumping since the jumping state can shoot in 8 directions without changing the animation state
//    data.bulletOriginForState[CombinedAxis::UP] = glm::vec2(50, 0);
//    data.bulletOriginForState[CombinedAxis::UP_RIGHT] = glm::vec2(100, 0);
//    data.bulletOriginForState[CombinedAxis::RIGHT] = glm::vec2(100, 64);
//    data.bulletOriginForState[CombinedAxis::DOWN_RIGHT] = glm::vec2(100, 100);
//    data.bulletOriginForState[CombinedAxis::DOWN] = glm::vec2(64, 100);
//    data.bulletOriginForState[CombinedAxis::DOWN_LEFT] = glm::vec2(0, 100);
//    data.bulletOriginForState[CombinedAxis::LEFT] = glm::vec2(0, 64);
//    data.bulletOriginForState[CombinedAxis::UP_LEFT] = glm::vec2(0, 0);
//
//    for(unsigned i = 0; i < 3; ++i) {
//
//        data.animationTextureRects[PlayerEnums::STATE_WALKING_RIGHT].push_back(sf::IntRect(0, 128 * i, 128, 128));
//        data.hurtboxes[PlayerEnums::STATE_WALKING_RIGHT].push_back(sf::FloatRect(51, 31, 37, 97));
//    }
//
//    for(unsigned i = 0; i < 3; ++i) {
//
//        data.animationTextureRects[PlayerEnums::STATE_WALKING_DOWN_RIGHT].push_back(sf::IntRect(128, 128 * i, 128, 128));
//        data.hurtboxes[PlayerEnums::STATE_WALKING_DOWN_RIGHT].push_back(sf::FloatRect(51, 31, 37, 97));
//    }
//
//    for(unsigned i = 0; i < 3; ++i) {
//
//        data.animationTextureRects[PlayerEnums::STATE_WALKING_UP_RIGHT].push_back(sf::IntRect(256, 128 * i, 128, 128));
//        data.hurtboxes[PlayerEnums::STATE_WALKING_UP_RIGHT].push_back(sf::FloatRect(51, 31, 37, 97));
//    }
//
//    for(unsigned i = 0; i < 3; ++i) {
//
//        data.animationTextureRects[PlayerEnums::STATE_WALKING_UP_LEFT].push_back(sf::IntRect(640, 128 * i, 128, 128));
//        data.hurtboxes[PlayerEnums::STATE_WALKING_UP_LEFT].push_back(sf::FloatRect(51, 31, 37, 97));
//    }
//
//    for(unsigned i = 0; i < 3; ++i) {
//
//        data.animationTextureRects[PlayerEnums::STATE_WALKING_DOWN_LEFT].push_back(sf::IntRect(768, 128 * i, 128, 128));
//        data.hurtboxes[PlayerEnums::STATE_WALKING_DOWN_LEFT].push_back(sf::FloatRect(51, 31, 37, 97));
//    }
//
//    for(unsigned i = 0; i < 3; ++i) {
//
//        data.animationTextureRects[PlayerEnums::STATE_WALKING_LEFT].push_back(sf::IntRect(896, 128 * i, 128, 128));
//        data.hurtboxes[PlayerEnums::STATE_WALKING_LEFT].push_back(sf::FloatRect(51, 31, 37, 97));
//    }
//
//    for(unsigned i = 0; i < 3; ++i) {
//
//        data.animationTextureRects[PlayerEnums::STATE_JUMPING].push_back(sf::IntRect(384, 512 + 128 * i, 128, 128));
//        data.hurtboxes[PlayerEnums::STATE_JUMPING].push_back(sf::FloatRect(40, 79, 55, 55));
//    }
//
//    for(unsigned i = 0; i < 3; ++i) {
//
//        data.animationTextureRects[PlayerEnums::STATE_DYING_FACING_LEFT].push_back(sf::IntRect(512 + 128 * i, 640, 128, 128));
//        data.hurtboxes[PlayerEnums::STATE_DYING_FACING_LEFT].push_back(sf::FloatRect(2, 2, 2, 2));
//    }
//
//    for(unsigned i = 0; i < 3; ++i) {
//
//        data.animationTextureRects[PlayerEnums::STATE_DYING_FACING_RIGHT].push_back(sf::IntRect(128 * i, 640, 128, 128));
//        data.hurtboxes[PlayerEnums::STATE_DYING_FACING_RIGHT].push_back(sf::FloatRect(2, 2, 2, 2));
//    }
//
//    data.animationTextureRects[PlayerEnums::STATE_STANDING_RIGHT].push_back(sf::IntRect(0, 0, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_STANDING_LEFT].push_back(sf::IntRect(896, 0, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_STANDING_UP_FACING_LEFT].push_back(sf::IntRect(512, 0, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_STANDING_UP_FACING_RIGHT].push_back(sf::IntRect(384, 0, 128, 128));
//
//    data.animationTextureRects[PlayerEnums::STATE_CROUCHING_RIGHT].push_back(sf::IntRect(384, 128, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_CROUCHING_LEFT].push_back(sf::IntRect(512, 128, 128, 128));
//
//    data.animationTextureRects[PlayerEnums::STATE_FALLING_RIGHT].push_back(sf::IntRect(0, 384, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_FALLING_DOWN_RIGHT].push_back(sf::IntRect(128, 384, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_FALLING_UP_RIGHT].push_back(sf::IntRect(256, 384, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_FALLING_UP_FACING_RIGHT].push_back(sf::IntRect(384, 384, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_FALLING_UP_FACING_LEFT].push_back(sf::IntRect(512, 384, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_FALLING_UP_LEFT].push_back(sf::IntRect(640, 384, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_FALLING_DOWN_LEFT].push_back(sf::IntRect(768, 384, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_FALLING_LEFT].push_back(sf::IntRect(896, 384, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_FALLING_DOWN_FACING_RIGHT].push_back(sf::IntRect(128, 512, 128, 128));
//    data.animationTextureRects[PlayerEnums::STATE_FALLING_DOWN_FACING_LEFT].push_back(sf::IntRect(256, 512, 128, 128));
//
//    data.animationTextureRects[PlayerEnums::STATE_DEAD].push_back(sf::IntRect(1, 1, 1, 1));
//
//
//
//    data.hurtboxes[PlayerEnums::STATE_STANDING_LEFT].push_back(sf::FloatRect(51, 31, 37, 97));
//    data.hurtboxes[PlayerEnums::STATE_STANDING_RIGHT].push_back(sf::FloatRect(51, 31, 37, 97));
//    data.hurtboxes[PlayerEnums::STATE_STANDING_UP_FACING_LEFT].push_back(sf::FloatRect(51, 31, 37, 97));
//    data.hurtboxes[PlayerEnums::STATE_STANDING_UP_FACING_RIGHT].push_back(sf::FloatRect(51, 31, 37, 97));
//
//    data.hurtboxes[PlayerEnums::STATE_CROUCHING_RIGHT].push_back(sf::FloatRect(10, 79, 118, 49));
//    data.hurtboxes[PlayerEnums::STATE_CROUCHING_LEFT].push_back(sf::FloatRect(0, 81, 128, 47));
//
//    data.hurtboxes[PlayerEnums::STATE_FALLING_RIGHT].push_back(sf::FloatRect(36, 25, 60, 89));
//    data.hurtboxes[PlayerEnums::STATE_FALLING_DOWN_RIGHT].push_back(sf::FloatRect(36, 25, 60, 89));
//    data.hurtboxes[PlayerEnums::STATE_FALLING_UP_RIGHT].push_back(sf::FloatRect(36, 25, 60, 89));
//    data.hurtboxes[PlayerEnums::STATE_FALLING_UP_FACING_RIGHT].push_back(sf::FloatRect(36, 25, 60, 89));
//    data.hurtboxes[PlayerEnums::STATE_FALLING_UP_FACING_LEFT].push_back(sf::FloatRect(36, 25, 60, 89));
//    data.hurtboxes[PlayerEnums::STATE_FALLING_UP_LEFT].push_back(sf::FloatRect(36, 25, 60, 89));
//    data.hurtboxes[PlayerEnums::STATE_FALLING_DOWN_LEFT].push_back(sf::FloatRect(36, 25, 60, 89));
//    data.hurtboxes[PlayerEnums::STATE_FALLING_LEFT].push_back(sf::FloatRect(36, 25, 60, 89));
//    data.hurtboxes[PlayerEnums::STATE_FALLING_DOWN_FACING_RIGHT].push_back(sf::FloatRect(36, 25, 60, 89));
//    data.hurtboxes[PlayerEnums::STATE_FALLING_DOWN_FACING_LEFT].push_back(sf::FloatRect(36, 25, 60, 89));
//
//    data.hurtboxes[PlayerEnums::STATE_DEAD].push_back(sf::FloatRect(1, 1, 1, 1));
//
//    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(47, 13, 47, 115));
//    data.hitboxes[data.jumpingHitboxState].push_back(sf::FloatRect(40, 13 + 115 - 49, 49, 49));

    return true;
}

bool loadTurretData(PreloadedTurretData &data, const std::string &dataFilename) {

    ///for now ignore the data file
    data.gunType = GunType::GUN_BASIC;
    data.gunfireDelay = sf::seconds(1.8);
    data.bulletType = BulletType::BULLET_SLOW;

    //load the animation properties
    data.hiddenStateDuration = sf::seconds(2.5);
    data.exposedStateDuration = sf::seconds(5.0);
    data.health = 8;

    //load the sprite
    data.textureFilename = "pirana.png";

    data.animationNextFrameTime = sf::milliseconds(75);

    //create the texture rects
    data.animationTextureRects[TurretEnemyEnums::STATE_HIDING].push_back(sf::IntRect(0, 0, 0, 0));

    data.animationTextureRects[TurretEnemyEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(91, 0, 28, 100));
    data.animationTextureRects[TurretEnemyEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(162, 0, 38, 100));
    data.animationTextureRects[TurretEnemyEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(243, 0, 35, 100));
    data.animationTextureRects[TurretEnemyEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(328, 0, 31, 100));
    data.animationTextureRects[TurretEnemyEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(426, 0, 32, 100));

    data.animationTextureRects[TurretEnemyEnums::STATE_GOING_INTO_HIDING].push_back(sf::IntRect(426, 0, 32, 100));
    data.animationTextureRects[TurretEnemyEnums::STATE_GOING_INTO_HIDING].push_back(sf::IntRect(328, 0, 31, 100));
    data.animationTextureRects[TurretEnemyEnums::STATE_GOING_INTO_HIDING].push_back(sf::IntRect(243, 0, 35, 100));
    data.animationTextureRects[TurretEnemyEnums::STATE_GOING_INTO_HIDING].push_back(sf::IntRect(162, 0, 38, 100));
    data.animationTextureRects[TurretEnemyEnums::STATE_GOING_INTO_HIDING].push_back(sf::IntRect(91, 0, 28, 100));

    data.animationTextureRects[TurretEnemyEnums::STATE_SHOOTING].push_back(sf::IntRect(156, 100, 44, 100)); //dow
    data.animationTextureRects[TurretEnemyEnums::STATE_SHOOTING].push_back(sf::IntRect(156, 100, 44, 100));//down_left
    data.animationTextureRects[TurretEnemyEnums::STATE_SHOOTING].push_back(sf::IntRect(80, 100, 33, 100));//left
    data.animationTextureRects[TurretEnemyEnums::STATE_SHOOTING].push_back(sf::IntRect(237, 100, 40, 100));//up_left
    data.animationTextureRects[TurretEnemyEnums::STATE_SHOOTING].push_back(sf::IntRect(321, 100, 41, 100));//up
    data.animationTextureRects[TurretEnemyEnums::STATE_SHOOTING].push_back(sf::IntRect(246, 200, 40, 100));//up_right
    data.animationTextureRects[TurretEnemyEnums::STATE_SHOOTING].push_back(sf::IntRect(71, 200, 33, 100));//right
    data.animationTextureRects[TurretEnemyEnums::STATE_SHOOTING].push_back(sf::IntRect(161, 200, 44, 100));//down_right

    //create hitboxes for each state and frame of animation
    ///hitbox positions are relative to origin of the data
    data.hurtboxes[TurretEnemyEnums::STATE_HIDING].push_back(sf::FloatRect(1, 1, 1, 1));

    data.hurtboxes[TurretEnemyEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 88, 28, 12));
    data.hurtboxes[TurretEnemyEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 75, 38, 25));
    data.hurtboxes[TurretEnemyEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 58, 35, 44));
    data.hurtboxes[TurretEnemyEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 35, 31, 62));
    data.hurtboxes[TurretEnemyEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 15, 32, 87));

    data.hurtboxes[TurretEnemyEnums::STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 15, 32, 87));
    data.hurtboxes[TurretEnemyEnums::STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 35, 31, 62));
    data.hurtboxes[TurretEnemyEnums::STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 58, 35, 44));
    data.hurtboxes[TurretEnemyEnums::STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 75, 38, 25));
    data.hurtboxes[TurretEnemyEnums::STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 88, 28, 12));

    data.hurtboxes[TurretEnemyEnums::STATE_SHOOTING].push_back(sf::FloatRect(0, 2, 46, 100));

    data.bulletOriginForState[CombinedAxis::UP_LEFT] = glm::vec2(14, 51);
    data.bulletOriginForState[CombinedAxis::UP] = glm::vec2(14, 51);
    data.bulletOriginForState[CombinedAxis::UP_RIGHT] = glm::vec2(14, 51);
    data.bulletOriginForState[CombinedAxis::RIGHT] = glm::vec2(14, 51);
    data.bulletOriginForState[CombinedAxis::DOWN_RIGHT] = glm::vec2(14, 51);
    data.bulletOriginForState[CombinedAxis::DOWN] = glm::vec2(14, 51);
    data.bulletOriginForState[CombinedAxis::DOWN_LEFT] = glm::vec2(14, 51);
    data.bulletOriginForState[CombinedAxis::LEFT] = glm::vec2(14, 51);

    //these objects don't collide with environment
    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(1, 1, 1, 1));

    return true;
}

bool loadEnemyData(PreloadedEnemyData &data, const std::string &dataFilename) {

    data.health = 1;

    data.textureFilename = "enemy.png";
    data.animationNextFrameTime = sf::milliseconds(100);

    int textureRectWidth = 128;
    int textureRectHeight = 128;

    for(unsigned frames = 0; frames < 5; ++frames) {

        data.animationTextureRects[EnemyEnums::STATE_WALKING_LEFT].push_back(sf::IntRect(textureRectWidth * frames, 0, textureRectWidth, textureRectHeight));
    }

    for(unsigned frames = 0; frames < 5; ++frames) {

       data.animationTextureRects[EnemyEnums::STATE_WALKING_RIGHT].push_back(sf::IntRect(textureRectWidth * frames, textureRectHeight, textureRectWidth, textureRectHeight));
    }

    data.animationTextureRects[EnemyEnums::STATE_FALLING_LEFT].push_back(sf::IntRect(0, textureRectHeight * 2, textureRectWidth, textureRectHeight));
    data.animationTextureRects[EnemyEnums::STATE_FALLING_RIGHT].push_back(sf::IntRect(textureRectWidth, textureRectHeight * 2, textureRectWidth, textureRectHeight));

    data.hurtboxes[EnemyEnums::STATE_WALKING_LEFT].push_back(sf::FloatRect(24, 50, 82, 78));
    data.hurtboxes[EnemyEnums::STATE_WALKING_RIGHT].push_back(sf::FloatRect(24, 50, 82, 78));
    data.hurtboxes[EnemyEnums::STATE_FALLING_LEFT].push_back(sf::FloatRect(24, 59, 74, 69));
    data.hurtboxes[EnemyEnums::STATE_FALLING_RIGHT].push_back(sf::FloatRect(32, 59, 74, 69));

    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(24, 50, 82, 78));

    return true;
}

bool loadOmniDirectionalTurretData(PreloadedOmniDirectionalTurretData &data, const std::string &dataFilename) {

    data.scale = 1;

    data.gunType = GunType::GUN_BASIC;
    data.gunfireDelay = sf::seconds(0);
    data.bulletType = BulletType::BULLET_MEDIUM;

    data.hiddenStateDuration = sf::seconds(3.5);
    data.exposedStateDuration = sf::seconds(6);
    data.shootingDelay = sf::seconds(2.f);
    data.health = 10;

    data.textureFilename = "mushroom.png";

    data.animationNextFrameTime = sf::milliseconds(90);

    data.animationTextureRects[OmniDirectionalTurretEnums::STATE_HIDING].push_back(sf::IntRect(1, 1, 1, 1));

    for(unsigned i = 0; i < 4; ++i) {

        data.animationTextureRects[OmniDirectionalTurretEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(256 * i, 0, 256, 256));
    }

    for(unsigned i = 0; i < 4; ++i) {

        data.animationTextureRects[OmniDirectionalTurretEnums::STATE_GOING_INTO_HIDING].push_back(sf::IntRect(769 - 256 * i, 0, 256, 256));
    }

    for(unsigned i = 0; i < 4; ++i) {

        data.animationTextureRects[OmniDirectionalTurretEnums::STATE_SHOOTING].push_back(sf::IntRect(256 * i, 256, 256, 256));
    }

    data.animationTextureRects[OmniDirectionalTurretEnums::STATE_SHOOTING].push_back(sf::IntRect(769, 256, 256, 256));
    data.animationTextureRects[OmniDirectionalTurretEnums::STATE_SHOOTING].push_back(sf::IntRect(769, 256, 256, 256));

    data.animationTextureRects[OmniDirectionalTurretEnums::STATE_EXPOSED].push_back(sf::IntRect(769, 0, 256, 256));


    data.hurtboxes[OmniDirectionalTurretEnums::STATE_HIDING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_EXPOSED].push_back(sf::FloatRect(52, 10, 175, 245));

    data.hurtboxes[OmniDirectionalTurretEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(44, 217, 126, 38));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(68, 178, 130, 78));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(79, 105, 123, 151));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(60, 15, 160, 241));

    data.hurtboxes[OmniDirectionalTurretEnums::STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(60, 15, 160, 241));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(79, 105, 123, 151));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(68, 178, 130, 78));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(44, 217, 126, 38));

    data.hurtboxes[OmniDirectionalTurretEnums::STATE_SHOOTING].push_back(sf::FloatRect(27, 47, 154, 209));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_SHOOTING].push_back(sf::FloatRect(47, 293 - 256, 143, 219));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_SHOOTING].push_back(sf::FloatRect(38, 312 - 256, 153, 200));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_SHOOTING].push_back(sf::FloatRect(26, 306 - 256, 204, 206));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_SHOOTING].push_back(sf::FloatRect(26, 306 - 256, 204, 206));
    data.hurtboxes[OmniDirectionalTurretEnums::STATE_SHOOTING].push_back(sf::FloatRect(26, 306 - 256, 204, 206));

    data.bulletOriginForState[CombinedAxis::UP_LEFT] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::UP] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::UP_RIGHT] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::RIGHT] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::DOWN_RIGHT] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::DOWN] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::DOWN_LEFT] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::LEFT] = glm::vec2(140, 51);

    //these turrets don't collide with blocks
    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(1, 1, 1, 1));

    return true;
}

bool loadBulletData(PreloadedBulletData &data, const std::string &dataFilename) {

    data.velocity = 7.f;

    if(dataFilename == "slow")
    data.velocity = 3.5f;

    if(dataFilename == "fast")
    data.velocity = 10.5f;

    data.lifetime = sf::seconds(3.f);

    data.health = 1;
    data.textureFilename = "bullet.png";
    data.animationNextFrameTime = sf::seconds(10);

    data.animationTextureRects[BulletEnums::STATE_RIGHT].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[BulletEnums::STATE_UP_RIGHT].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[BulletEnums::STATE_UP].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[BulletEnums::STATE_UP_LEFT].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[BulletEnums::STATE_LEFT].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[BulletEnums::STATE_DOWN_LEFT].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[BulletEnums::STATE_DOWN].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[BulletEnums::STATE_DOWN_RIGHT].push_back(sf::IntRect(0, 0, 21, 21));

    data.hurtboxes[BulletEnums::STATE_RIGHT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[BulletEnums::STATE_UP_RIGHT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[BulletEnums::STATE_UP].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[BulletEnums::STATE_UP_LEFT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[BulletEnums::STATE_LEFT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[BulletEnums::STATE_DOWN_LEFT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[BulletEnums::STATE_DOWN].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[BulletEnums::STATE_DOWN_RIGHT].push_back(sf::FloatRect(0, 0, 21, 21));

    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(0, 0, 21, 21));

    return true;
}

bool loadDestrutibleBlockData(PreloadedDestructibleBlockData &data, const std::string &dataFilename) {

    data.textureFilename = "BrickBreak.png";
    data.animationNextFrameTime = sf::milliseconds(40);

    data.animationTextureRects[DestructibleBlockEnums::STATE_SOLID].push_back(sf::IntRect(0, 0, 64, 64));

    data.animationTextureRects[DestructibleBlockEnums::STATE_DESTROYING].push_back(sf::IntRect(64, 0, 64, 64));
    data.animationTextureRects[DestructibleBlockEnums::STATE_DESTROYING].push_back(sf::IntRect(128, 0, 64, 64));
    data.animationTextureRects[DestructibleBlockEnums::STATE_DESTROYING].push_back(sf::IntRect(0, 64, 64, 64));
    data.animationTextureRects[DestructibleBlockEnums::STATE_DESTROYING].push_back(sf::IntRect(64, 64, 64, 64));
    data.animationTextureRects[DestructibleBlockEnums::STATE_DESTROYING].push_back(sf::IntRect(128, 64, 64, 64));
    data.animationTextureRects[DestructibleBlockEnums::STATE_DESTROYING].push_back(sf::IntRect(0, 128, 64, 64));
    data.animationTextureRects[DestructibleBlockEnums::STATE_DESTROYING].push_back(sf::IntRect(64, 128, 64, 64));
    data.animationTextureRects[DestructibleBlockEnums::STATE_DESTROYING].push_back(sf::IntRect(128, 128, 64, 64));

    data.animationTextureRects[DestructibleBlockEnums::STATE_DESTROYED].push_back(sf::IntRect(1, 1, 1, 1));

    data.hurtboxes[DestructibleBlockEnums::STATE_SOLID].push_back(sf::FloatRect(0, 0, 64, 64));
    data.hurtboxes[DestructibleBlockEnums::STATE_DESTROYING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hurtboxes[DestructibleBlockEnums::STATE_DESTROYED].push_back(sf::FloatRect(1, 1, 1, 1));

    data.health = 1;

    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(0, 0, 64, 64));

    return true;
}

bool loadPowerUpData(PreloadedPowerUpData &data, const std::string &dataFilename) {

    data.health = 1;

    data.textureFilename = "powerups.png";
    data.animationNextFrameTime = sf::milliseconds(200);

    for(unsigned i = 0; i < 5; ++i) {

        data.animationTextureRects[PowerUpEnums::STATE_SPAWNING].push_back(sf::IntRect(128 * i, 0, 128, 128));
        data.animationTextureRects[PowerUpEnums::STATE_DISAPPEARING].push_back(sf::IntRect(128 * i, 256, 128, 128));
    }

    //machinegun frame
    data.animationTextureRects[PowerUpEnums::STATE_SPAWNED].push_back(sf::IntRect(0, 128, 128, 128));
    data.animationTextureRects[PowerUpEnums::STATE_DISAPPEARED].push_back(sf::IntRect(1, 1, 1, 1));

    data.hurtboxes[PowerUpEnums::STATE_SPAWNING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hurtboxes[PowerUpEnums::STATE_SPAWNED].push_back(sf::FloatRect(25, 17, 87, 87));//machinegun
    data.hurtboxes[PowerUpEnums::STATE_DISAPPEARING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hurtboxes[PowerUpEnums::STATE_DISAPPEARED].push_back(sf::FloatRect(1, 1, 1, 1));

    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(25, 17, 87, 87));

    return true;
}
