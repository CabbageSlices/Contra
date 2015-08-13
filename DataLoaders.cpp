#include "DataLoaders.h"
#include "PreloadedData.h"
#include "Gun.h"

using std::map;

bool loadBossData(map<EnemyType, BossProperties> &bossData, const std::string &dataFileName) {

    bossData[EnemyType::ENEMY_GOOMBA].scale = 2.f;
    bossData[EnemyType::ENEMY_GOOMBA].health = 5;

    bossData[EnemyType::ENEMY_PIRANHA].scale = 2.f;
    bossData[EnemyType::ENEMY_PIRANHA].health = 13.f;

    bossData[EnemyType::ENEMY_MUSHROOM].scale = 2.f;
    bossData[EnemyType::ENEMY_MUSHROOM].health = 20.f;

    return true;
}

bool loadPlayerData(PreloadedPlayerData &data, const std::string &dataFileName) {

    data.health = 3;
    data.textureFileName = "player.png";
    data.animationNextFrameTime = sf::milliseconds(100);

    data.gunType = GunType::GUN_BASIC;
    data.gunfireDelay = FIRE_DELAY_MEDIUM;
    data.bulletType = BulletType::BULLET_MEDIUM;

    data.STATE_STANDING_LEFT = 0;
    data.STATE_STANDING_UP_FACING_LEFT = 1;
    data.STATE_STANDING_UP_FACING_RIGHT = 2;
    data.STATE_STANDING_RIGHT = 3;

    data.STATE_WALKING_DOWN_LEFT = 4;
    data.STATE_WALKING_LEFT = 5;
    data.STATE_WALKING_UP_LEFT = 6;
    data.STATE_WALKING_UP_RIGHT = 7;
    data.STATE_WALKING_RIGHT = 8;
    data.STATE_WALKING_DOWN_RIGHT = 9;

    data.STATE_CROUCHING_LEFT = 10;
    data.STATE_CROUCHING_RIGHT = 11;

    data.STATE_FALLING_DOWN_FACING_LEFT = 12;
    data.STATE_FALLING_DOWN_FACING_RIGHT = 13;
    data.STATE_FALLING_DOWN_LEFT = 14;
    data.STATE_FALLING_LEFT = 15;
    data.STATE_FALLING_UP_LEFT = 16;
    data.STATE_FALLING_UP_FACING_LEFT = 17;
    data.STATE_FALLING_UP_FACING_RIGHT = 18;
    data.STATE_FALLING_UP_RIGHT = 19;
    data.STATE_FALLING_RIGHT = 20;
    data.STATE_FALLING_DOWN_RIGHT = 21;

    data.STATE_JUMPING = 22;

    data.STATE_DYING_FACING_LEFT = 23;
    data.STATE_DYING_FACING_RIGHT = 24;

    data.STATE_DEAD = 25;

    data.bulletOriginForState[data.STATE_STANDING_LEFT] = glm::vec2(0, 57);
    data.bulletOriginForState[data.STATE_STANDING_RIGHT] = glm::vec2(126, 62);
    data.bulletOriginForState[data.STATE_STANDING_UP_FACING_LEFT] = glm::vec2(73, 0);
    data.bulletOriginForState[data.STATE_STANDING_UP_FACING_RIGHT] = glm::vec2(48, 0);

    data.bulletOriginForState[data.STATE_WALKING_DOWN_LEFT] = glm::vec2(15, 128);
    data.bulletOriginForState[data.STATE_WALKING_LEFT] = glm::vec2(0, 56);
    data.bulletOriginForState[data.STATE_WALKING_UP_LEFT] = glm::vec2(32, 14);
    data.bulletOriginForState[data.STATE_WALKING_UP_RIGHT] = glm::vec2(109, 10);
    data.bulletOriginForState[data.STATE_WALKING_RIGHT] = glm::vec2(128, 61);
    data.bulletOriginForState[data.STATE_WALKING_DOWN_RIGHT] = glm::vec2(114, 128);

    data.bulletOriginForState[data.STATE_CROUCHING_LEFT] = glm::vec2(-5, 102);
    data.bulletOriginForState[data.STATE_CROUCHING_RIGHT] = glm::vec2(128, 104);

    data.bulletOriginForState[data.STATE_FALLING_DOWN_FACING_LEFT] = glm::vec2(37, 112);
    data.bulletOriginForState[data.STATE_FALLING_DOWN_FACING_RIGHT] = glm::vec2(51, 114);
    data.bulletOriginForState[data.STATE_FALLING_RIGHT] = glm::vec2(128, 51);
    data.bulletOriginForState[data.STATE_FALLING_DOWN_RIGHT] = glm::vec2(128, 110);
    data.bulletOriginForState[data.STATE_FALLING_UP_RIGHT] = glm::vec2(120, 9);
    data.bulletOriginForState[data.STATE_FALLING_UP_LEFT] = glm::vec2(74, 0);
    data.bulletOriginForState[data.STATE_FALLING_UP_LEFT] = glm::vec2(0, 0);
    data.bulletOriginForState[data.STATE_FALLING_DOWN_LEFT] = glm::vec2(13, 108);
    data.bulletOriginForState[data.STATE_FALLING_LEFT] = glm::vec2(0, 47);

    //bullet origin for each direction the player could face
    //this is only usedw hen player is jumping since the jumping state can shoot in 8 directions without changing the animation state
    data.bulletOriginForState[CombinedAxis::UP] = glm::vec2(50, 0);
    data.bulletOriginForState[CombinedAxis::UP_RIGHT] = glm::vec2(100, 0);
    data.bulletOriginForState[CombinedAxis::RIGHT] = glm::vec2(100, 64);
    data.bulletOriginForState[CombinedAxis::DOWN_RIGHT] = glm::vec2(100, 100);
    data.bulletOriginForState[CombinedAxis::DOWN] = glm::vec2(64, 100);
    data.bulletOriginForState[CombinedAxis::DOWN_LEFT] = glm::vec2(0, 100);
    data.bulletOriginForState[CombinedAxis::LEFT] = glm::vec2(0, 64);
    data.bulletOriginForState[CombinedAxis::UP_LEFT] = glm::vec2(0, 0);

    for(unsigned i = 0; i < 3; ++i) {

        data.animationTextureRects[data.STATE_WALKING_RIGHT].push_back(sf::IntRect(0, 128 * i, 128, 128));
        data.hurtboxes[data.STATE_WALKING_RIGHT].push_back(sf::FloatRect(51, 31, 37, 97));
    }

    for(unsigned i = 0; i < 3; ++i) {

        data.animationTextureRects[data.STATE_WALKING_DOWN_RIGHT].push_back(sf::IntRect(128, 128 * i, 128, 128));
        data.hurtboxes[data.STATE_WALKING_DOWN_RIGHT].push_back(sf::FloatRect(51, 31, 37, 97));
    }

    for(unsigned i = 0; i < 3; ++i) {

        data.animationTextureRects[data.STATE_WALKING_UP_RIGHT].push_back(sf::IntRect(256, 128 * i, 128, 128));
        data.hurtboxes[data.STATE_WALKING_UP_RIGHT].push_back(sf::FloatRect(51, 31, 37, 97));
    }

    for(unsigned i = 0; i < 3; ++i) {

        data.animationTextureRects[data.STATE_WALKING_UP_LEFT].push_back(sf::IntRect(640, 128 * i, 128, 128));
        data.hurtboxes[data.STATE_WALKING_UP_LEFT].push_back(sf::FloatRect(51, 31, 37, 97));
    }

    for(unsigned i = 0; i < 3; ++i) {

        data.animationTextureRects[data.STATE_WALKING_DOWN_LEFT].push_back(sf::IntRect(768, 128 * i, 128, 128));
        data.hurtboxes[data.STATE_WALKING_DOWN_LEFT].push_back(sf::FloatRect(51, 31, 37, 97));
    }

    for(unsigned i = 0; i < 3; ++i) {

        data.animationTextureRects[data.STATE_WALKING_LEFT].push_back(sf::IntRect(896, 128 * i, 128, 128));
        data.hurtboxes[data.STATE_WALKING_LEFT].push_back(sf::FloatRect(51, 31, 37, 97));
    }

    for(unsigned i = 0; i < 3; ++i) {

        data.animationTextureRects[data.STATE_JUMPING].push_back(sf::IntRect(384, 512 + 128 * i, 128, 128));
        data.hurtboxes[data.STATE_JUMPING].push_back(sf::FloatRect(40, 79, 55, 55));
    }

    for(unsigned i = 0; i < 3; ++i) {

        data.animationTextureRects[data.STATE_DYING_FACING_LEFT].push_back(sf::IntRect(512 + 128 * i, 640, 128, 128));
        data.hurtboxes[data.STATE_DYING_FACING_LEFT].push_back(sf::FloatRect(2, 2, 2, 2));
    }

    for(unsigned i = 0; i < 3; ++i) {

        data.animationTextureRects[data.STATE_DYING_FACING_RIGHT].push_back(sf::IntRect(128 * i, 640, 128, 128));
        data.hurtboxes[data.STATE_DYING_FACING_RIGHT].push_back(sf::FloatRect(2, 2, 2, 2));
    }

    data.animationTextureRects[data.STATE_STANDING_RIGHT].push_back(sf::IntRect(0, 0, 128, 128));
    data.animationTextureRects[data.STATE_STANDING_LEFT].push_back(sf::IntRect(896, 0, 128, 128));
    data.animationTextureRects[data.STATE_STANDING_UP_FACING_LEFT].push_back(sf::IntRect(512, 0, 128, 128));
    data.animationTextureRects[data.STATE_STANDING_UP_FACING_RIGHT].push_back(sf::IntRect(384, 0, 128, 128));

    data.animationTextureRects[data.STATE_CROUCHING_RIGHT].push_back(sf::IntRect(384, 128, 128, 128));
    data.animationTextureRects[data.STATE_CROUCHING_LEFT].push_back(sf::IntRect(512, 128, 128, 128));

    data.animationTextureRects[data.STATE_FALLING_RIGHT].push_back(sf::IntRect(0, 384, 128, 128));
    data.animationTextureRects[data.STATE_FALLING_DOWN_RIGHT].push_back(sf::IntRect(128, 384, 128, 128));
    data.animationTextureRects[data.STATE_FALLING_UP_RIGHT].push_back(sf::IntRect(256, 384, 128, 128));
    data.animationTextureRects[data.STATE_FALLING_UP_FACING_RIGHT].push_back(sf::IntRect(384, 384, 128, 128));
    data.animationTextureRects[data.STATE_FALLING_UP_FACING_LEFT].push_back(sf::IntRect(512, 384, 128, 128));
    data.animationTextureRects[data.STATE_FALLING_UP_LEFT].push_back(sf::IntRect(640, 384, 128, 128));
    data.animationTextureRects[data.STATE_FALLING_DOWN_LEFT].push_back(sf::IntRect(768, 384, 128, 128));
    data.animationTextureRects[data.STATE_FALLING_LEFT].push_back(sf::IntRect(896, 384, 128, 128));
    data.animationTextureRects[data.STATE_FALLING_DOWN_FACING_RIGHT].push_back(sf::IntRect(128, 512, 128, 128));
    data.animationTextureRects[data.STATE_FALLING_DOWN_FACING_LEFT].push_back(sf::IntRect(256, 512, 128, 128));

    data.animationTextureRects[data.STATE_DEAD].push_back(sf::IntRect(1, 1, 1, 1));



    data.hurtboxes[data.STATE_STANDING_LEFT].push_back(sf::FloatRect(51, 31, 37, 97));
    data.hurtboxes[data.STATE_STANDING_RIGHT].push_back(sf::FloatRect(51, 31, 37, 97));
    data.hurtboxes[data.STATE_STANDING_UP_FACING_LEFT].push_back(sf::FloatRect(51, 31, 37, 97));
    data.hurtboxes[data.STATE_STANDING_UP_FACING_RIGHT].push_back(sf::FloatRect(51, 31, 37, 97));

    data.hurtboxes[data.STATE_CROUCHING_RIGHT].push_back(sf::FloatRect(10, 79, 118, 49));
    data.hurtboxes[data.STATE_CROUCHING_LEFT].push_back(sf::FloatRect(0, 81, 128, 47));

    data.hurtboxes[data.STATE_FALLING_RIGHT].push_back(sf::FloatRect(36, 25, 60, 89));
    data.hurtboxes[data.STATE_FALLING_DOWN_RIGHT].push_back(sf::FloatRect(36, 25, 60, 89));
    data.hurtboxes[data.STATE_FALLING_UP_RIGHT].push_back(sf::FloatRect(36, 25, 60, 89));
    data.hurtboxes[data.STATE_FALLING_UP_FACING_RIGHT].push_back(sf::FloatRect(36, 25, 60, 89));
    data.hurtboxes[data.STATE_FALLING_UP_FACING_LEFT].push_back(sf::FloatRect(36, 25, 60, 89));
    data.hurtboxes[data.STATE_FALLING_UP_LEFT].push_back(sf::FloatRect(36, 25, 60, 89));
    data.hurtboxes[data.STATE_FALLING_DOWN_LEFT].push_back(sf::FloatRect(36, 25, 60, 89));
    data.hurtboxes[data.STATE_FALLING_LEFT].push_back(sf::FloatRect(36, 25, 60, 89));
    data.hurtboxes[data.STATE_FALLING_DOWN_FACING_RIGHT].push_back(sf::FloatRect(36, 25, 60, 89));
    data.hurtboxes[data.STATE_FALLING_DOWN_FACING_LEFT].push_back(sf::FloatRect(36, 25, 60, 89));

    data.hurtboxes[data.STATE_DEAD].push_back(sf::FloatRect(1, 1, 1, 1));

    ///aside from the default hitbox, if an object
    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(47, 13, 47, 115));
    data.hitboxes[data.jumpingHitboxState].push_back(sf::FloatRect(40, 13 + 115 - 49, 49, 49));

    return true;
}

bool loadTurretData(PreloadedTurretData &data, const std::string &dataFileName) {

    ///for now ignore the data file
    //load the different states
    data.STATE_HIDING = 0;
    data.STATE_COMING_OUT_OF_HIDING = 1;
    data.STATE_GOING_INTO_HIDING = 2;
    data.STATE_SHOOTING = 3;

    //set the frames in the order that the texturerect data for this shooting direction is inserted into the container
    data.DOWN = 0;
    data.DOWN_LEFT = 1;
    data.LEFT = 2;
    data.UP_LEFT = 3;
    data.UP = 4;
    data.UP_RIGHT = 5;
    data.RIGHT = 6;
    data.DOWN_RIGHT = 7;

    data.gunType = GunType::GUN_BASIC;
    data.gunfireDelay = sf::seconds(1.8);
    data.bulletType = BulletType::BULLET_SLOW;

    //load the animation properties
    data.hiddenStateDuration = sf::seconds(2.5);
    data.exposedStateDuration = sf::seconds(5.0);
    data.health = 8;

    //load the sprite
    data.textureFileName = "pirana.png";

    data.animationNextFrameTime = sf::milliseconds(75);

    //create the texture rects
    data.animationTextureRects[data.STATE_HIDING].push_back(sf::IntRect(0, 0, 0, 0));

    data.animationTextureRects[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(91, 0, 28, 100));
    data.animationTextureRects[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(162, 0, 38, 100));
    data.animationTextureRects[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(243, 0, 35, 100));
    data.animationTextureRects[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(328, 0, 31, 100));
    data.animationTextureRects[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(426, 0, 32, 100));

    data.animationTextureRects[data.STATE_GOING_INTO_HIDING].push_back(sf::IntRect(426, 0, 32, 100));
    data.animationTextureRects[data.STATE_GOING_INTO_HIDING].push_back(sf::IntRect(328, 0, 31, 100));
    data.animationTextureRects[data.STATE_GOING_INTO_HIDING].push_back(sf::IntRect(243, 0, 35, 100));
    data.animationTextureRects[data.STATE_GOING_INTO_HIDING].push_back(sf::IntRect(162, 0, 38, 100));
    data.animationTextureRects[data.STATE_GOING_INTO_HIDING].push_back(sf::IntRect(91, 0, 28, 100));

    data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(156, 100, 44, 100)); //dow
    data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(156, 100, 44, 100));//down_left
    data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(80, 100, 33, 100));//left
    data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(237, 100, 40, 100));//up_left
    data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(321, 100, 41, 100));//up
    data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(246, 200, 40, 100));//up_right
    data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(71, 200, 33, 100));//right
    data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(161, 200, 44, 100));//down_right

    //create hitboxes for each state and frame of animation
    ///hitbox positions are relative to origin of the data
    data.hurtboxes[data.STATE_HIDING].push_back(sf::FloatRect(1, 1, 1, 1));

    data.hurtboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 88, 28, 12));
    data.hurtboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 75, 38, 25));
    data.hurtboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 58, 35, 44));
    data.hurtboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 35, 31, 62));
    data.hurtboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 15, 32, 87));

    data.hurtboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 15, 32, 87));
    data.hurtboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 35, 31, 62));
    data.hurtboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 58, 35, 44));
    data.hurtboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 75, 38, 25));
    data.hurtboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 88, 28, 12));

    data.hurtboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(0, 2, 46, 100));

    data.bulletOriginForState[CombinedAxis::UP_LEFT] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::UP] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::UP_RIGHT] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::RIGHT] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::DOWN_RIGHT] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::DOWN] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::DOWN_LEFT] = glm::vec2(140, 51);
    data.bulletOriginForState[CombinedAxis::LEFT] = glm::vec2(140, 51);

    //these objects don't collide with environment
    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(1, 1, 1, 1));

    return true;
}

bool loadEnemyData(PreloadedEnemyData &data, const std::string &dataFileName) {

    data.STATE_WALKING_LEFT = 0;
    data.STATE_WALKING_RIGHT = 1;
    data.STATE_FALLING_LEFT = 2;
    data.STATE_FALLING_RIGHT = 3;

    data.health = 1;

    data.textureFileName = "enemy.png";
    data.animationNextFrameTime = sf::milliseconds(100);

    int textureRectWidth = 128;
    int textureRectHeight = 128;

    for(unsigned frames = 0; frames < 5; ++frames) {

        data.animationTextureRects[data.STATE_WALKING_LEFT].push_back(sf::IntRect(textureRectWidth * frames, 0, textureRectWidth, textureRectHeight));
    }

    for(unsigned frames = 0; frames < 5; ++frames) {

       data.animationTextureRects[data.STATE_WALKING_RIGHT].push_back(sf::IntRect(textureRectWidth * frames, textureRectHeight, textureRectWidth, textureRectHeight));
    }

    data.animationTextureRects[data.STATE_FALLING_LEFT].push_back(sf::IntRect(0, textureRectHeight * 2, textureRectWidth, textureRectHeight));
    data.animationTextureRects[data.STATE_FALLING_RIGHT].push_back(sf::IntRect(textureRectWidth, textureRectHeight * 2, textureRectWidth, textureRectHeight));

    data.hurtboxes[data.STATE_WALKING_LEFT].push_back(sf::FloatRect(24, 50, 82, 78));
    data.hurtboxes[data.STATE_WALKING_RIGHT].push_back(sf::FloatRect(24, 50, 82, 78));
    data.hurtboxes[data.STATE_FALLING_LEFT].push_back(sf::FloatRect(24, 59, 74, 69));
    data.hurtboxes[data.STATE_FALLING_RIGHT].push_back(sf::FloatRect(32, 59, 74, 69));

    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(24, 50, 82, 78));

    return true;
}

bool loadOmniDirectionalTurretData(PreloadedOmniDirectionalTurretData &data, const std::string &dataFileName) {

    data.scale = 1;
    data.STATE_HIDING = 0;
    data.STATE_COMING_OUT_OF_HIDING = 1;
    data.STATE_EXPOSED = 2;
    data.STATE_SHOOTING = 3;
    data.STATE_GOING_INTO_HIDING = 4;

    data.gunType = GunType::GUN_BASIC;
    data.gunfireDelay = sf::seconds(0);
    data.bulletType = BulletType::BULLET_MEDIUM;

    data.hiddenStateDuration = sf::seconds(3.5);
    data.exposedStateDuration = sf::seconds(6);
    data.shootingDelay = sf::seconds(2.f);
    data.health = 10;

    data.textureFileName = "mushroom.png";

    data.animationNextFrameTime = sf::milliseconds(90);

    data.animationTextureRects[data.STATE_HIDING].push_back(sf::IntRect(1, 1, 1, 1));

    for(unsigned i = 0; i < 4; ++i) {

        data.animationTextureRects[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::IntRect(256 * i, 0, 256, 256));
    }

    for(unsigned i = 0; i < 4; ++i) {

        data.animationTextureRects[data.STATE_GOING_INTO_HIDING].push_back(sf::IntRect(769 - 256 * i, 0, 256, 256));
    }

    for(unsigned i = 0; i < 4; ++i) {

        data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(256 * i, 256, 256, 256));
    }

    data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(769, 256, 256, 256));
    data.animationTextureRects[data.STATE_SHOOTING].push_back(sf::IntRect(769, 256, 256, 256));

    data.animationTextureRects[data.STATE_EXPOSED].push_back(sf::IntRect(769, 0, 256, 256));


    data.hurtboxes[data.STATE_HIDING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hurtboxes[data.STATE_EXPOSED].push_back(sf::FloatRect(52, 10, 175, 245));

    data.hurtboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(44, 217, 126, 38));
    data.hurtboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(68, 178, 130, 78));
    data.hurtboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(79, 105, 123, 151));
    data.hurtboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(60, 15, 160, 241));

    data.hurtboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(60, 15, 160, 241));
    data.hurtboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(79, 105, 123, 151));
    data.hurtboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(68, 178, 130, 78));
    data.hurtboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(44, 217, 126, 38));

    data.hurtboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(27, 47, 154, 209));
    data.hurtboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(47, 293 - 256, 143, 219));
    data.hurtboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(38, 312 - 256, 153, 200));
    data.hurtboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(26, 306 - 256, 204, 206));
    data.hurtboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(26, 306 - 256, 204, 206));
    data.hurtboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(26, 306 - 256, 204, 206));

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

bool loadBulletData(PreloadedBulletData &data, const std::string &dataFileName) {

    data.STATE_RIGHT = 0;
    data.STATE_UP_RIGHT = 1;
    data.STATE_UP = 2;
    data.STATE_UP_LEFT = 3;
    data.STATE_LEFT = 4;
    data.STATE_DOWN_LEFT = 5;
    data.STATE_DOWN = 6;
    data.STATE_DOWN_RIGHT = 7;

    data.velocity = 7.f;

    if(dataFileName == "slow")
    data.velocity = 3.5f;

    if(dataFileName == "fast")
    data.velocity = 10.5f;

    data.lifetime = sf::seconds(3.f);

    data.health = 1;
    data.textureFileName = "bullet.png";
    data.animationNextFrameTime = sf::seconds(10);

    data.animationTextureRects[data.STATE_RIGHT].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[data.STATE_UP_RIGHT].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[data.STATE_UP].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[data.STATE_UP_LEFT].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[data.STATE_LEFT].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[data.STATE_DOWN_LEFT].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[data.STATE_DOWN].push_back(sf::IntRect(0, 0, 21, 21));
    data.animationTextureRects[data.STATE_DOWN_RIGHT].push_back(sf::IntRect(0, 0, 21, 21));

    data.hurtboxes[data.STATE_RIGHT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[data.STATE_UP_RIGHT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[data.STATE_UP].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[data.STATE_UP_LEFT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[data.STATE_LEFT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[data.STATE_DOWN_LEFT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[data.STATE_DOWN].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hurtboxes[data.STATE_DOWN_RIGHT].push_back(sf::FloatRect(0, 0, 21, 21));

    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(0, 0, 21, 21));

    return true;
}

bool loadDestrutibleBlockData(PreloadedDestructibleBlockData &data, const std::string &dataFileName) {

    data.STATE_SOLID = 0;
    data.STATE_DESTROYING = 1;
    data.STATE_DESTROYED = 2;

    data.textureFileName = "BrickBreak.png";
    data.animationNextFrameTime = sf::milliseconds(40);

    data.animationTextureRects[data.STATE_SOLID].push_back(sf::IntRect(0, 0, 64, 64));

    data.animationTextureRects[data.STATE_DESTROYING].push_back(sf::IntRect(64, 0, 64, 64));
    data.animationTextureRects[data.STATE_DESTROYING].push_back(sf::IntRect(128, 0, 64, 64));
    data.animationTextureRects[data.STATE_DESTROYING].push_back(sf::IntRect(0, 64, 64, 64));
    data.animationTextureRects[data.STATE_DESTROYING].push_back(sf::IntRect(64, 64, 64, 64));
    data.animationTextureRects[data.STATE_DESTROYING].push_back(sf::IntRect(128, 64, 64, 64));
    data.animationTextureRects[data.STATE_DESTROYING].push_back(sf::IntRect(0, 128, 64, 64));
    data.animationTextureRects[data.STATE_DESTROYING].push_back(sf::IntRect(64, 128, 64, 64));
    data.animationTextureRects[data.STATE_DESTROYING].push_back(sf::IntRect(128, 128, 64, 64));

    data.hurtboxes[data.STATE_SOLID].push_back(sf::FloatRect(0, 0, 64, 64));
    data.hurtboxes[data.STATE_DESTROYING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hurtboxes[data.STATE_DESTROYED].push_back(sf::FloatRect(1, 1, 1, 1));

    data.health = 1;

    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(0, 0, 64, 64));

    return true;
}

bool loadPowerUpData(PreloadedPowerUpData &data, const std::string &dataFileName) {

    data.health = 1;

    data.STATE_SPAWNING = 0;
    data.STATE_SPAWNED = 1;
    data.STATE_DISAPPEARING = 2;
    data.STATE_DISAPPEARED = 3;

    data.textureFileName = "powerups.png";
    data.animationNextFrameTime = sf::milliseconds(200);

    data.FRAME_MACHINEGUN = 0;

    for(unsigned i = 0; i < 5; ++i) {

        data.animationTextureRects[data.STATE_SPAWNING].push_back(sf::IntRect(128 * i, 0, 128, 128));
        data.animationTextureRects[data.STATE_DISAPPEARING].push_back(sf::IntRect(128 * i, 256, 128, 128));
    }

    //machinegun frame
    data.animationTextureRects[data.STATE_SPAWNED].push_back(sf::IntRect(0, 128, 128, 128));
    data.animationTextureRects[data.STATE_DISAPPEARED].push_back(sf::IntRect(1, 1, 1, 1));

    data.hurtboxes[data.STATE_SPAWNING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hurtboxes[data.STATE_SPAWNED].push_back(sf::FloatRect(25, 17, 87, 87));//machinegun
    data.hurtboxes[data.STATE_DISAPPEARING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hurtboxes[data.STATE_DISAPPEARED].push_back(sf::FloatRect(1, 1, 1, 1));

    data.hitboxes[data.defaultHitboxState].push_back(sf::FloatRect(25, 17, 87, 87));

    return true;
}
