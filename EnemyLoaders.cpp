#include "EnemyLoaders.h"
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

    data.gunfireDelay = sf::seconds(1.5);

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
