#include "DataLoaders.h"
#include "PreloadedData.h"

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
    data.hitboxes[data.STATE_HIDING].push_back(sf::FloatRect(1, 1, 1, 1));

    data.hitboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 88, 28, 12));
    data.hitboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 75, 38, 25));
    data.hitboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 58, 35, 44));
    data.hitboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 35, 31, 62));
    data.hitboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(0, 15, 32, 87));

    data.hitboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 15, 32, 87));
    data.hitboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 35, 31, 62));
    data.hitboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 58, 35, 44));
    data.hitboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 75, 38, 25));
    data.hitboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(0, 88, 28, 12));

    data.hitboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(0, 2, 46, 100));

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

    data.hitboxes[data.STATE_WALKING_LEFT].push_back(sf::FloatRect(24, 50, 82, 78));
    data.hitboxes[data.STATE_WALKING_RIGHT].push_back(sf::FloatRect(24, 50, 82, 78));
    data.hitboxes[data.STATE_FALLING_LEFT].push_back(sf::FloatRect(24, 59, 74, 69));
    data.hitboxes[data.STATE_FALLING_RIGHT].push_back(sf::FloatRect(32, 59, 74, 69));

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
    data.bulletType = BulletType::BULLET_FAST;

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


    data.hitboxes[data.STATE_HIDING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hitboxes[data.STATE_EXPOSED].push_back(sf::FloatRect(52, 10, 175, 245));

    data.hitboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(44, 217, 126, 38));
    data.hitboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(68, 178, 130, 78));
    data.hitboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(79, 105, 123, 151));
    data.hitboxes[data.STATE_COMING_OUT_OF_HIDING].push_back(sf::FloatRect(60, 15, 160, 241));

    data.hitboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(60, 15, 160, 241));
    data.hitboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(79, 105, 123, 151));
    data.hitboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(68, 178, 130, 78));
    data.hitboxes[data.STATE_GOING_INTO_HIDING].push_back(sf::FloatRect(44, 217, 126, 38));

    data.hitboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(27, 47, 154, 209));
    data.hitboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(47, 293 - 256, 143, 219));
    data.hitboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(38, 312 - 256, 153, 200));
    data.hitboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(26, 306 - 256, 204, 206));
    data.hitboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(26, 306 - 256, 204, 206));
    data.hitboxes[data.STATE_SHOOTING].push_back(sf::FloatRect(26, 306 - 256, 204, 206));

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

    data.hitboxes[data.STATE_RIGHT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hitboxes[data.STATE_UP_RIGHT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hitboxes[data.STATE_UP].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hitboxes[data.STATE_UP_LEFT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hitboxes[data.STATE_LEFT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hitboxes[data.STATE_DOWN_LEFT].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hitboxes[data.STATE_DOWN].push_back(sf::FloatRect(0, 0, 21, 21));
    data.hitboxes[data.STATE_DOWN_RIGHT].push_back(sf::FloatRect(0, 0, 21, 21));

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

    data.hitboxes[data.STATE_SOLID].push_back(sf::FloatRect(0, 0, 64, 64));
    data.hitboxes[data.STATE_DESTROYING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hitboxes[data.STATE_DESTROYED].push_back(sf::FloatRect(1, 1, 1, 1));

    data.health = 1;

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

    data.hitboxes[data.STATE_SPAWNING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hitboxes[data.STATE_SPAWNED].push_back(sf::FloatRect(25, 17, 87, 87));//machinegun
    data.hitboxes[data.STATE_DISAPPEARING].push_back(sf::FloatRect(1, 1, 1, 1));
    data.hitboxes[data.STATE_DISAPPEARED].push_back(sf::FloatRect(1, 1, 1, 1));

    return true;
}
