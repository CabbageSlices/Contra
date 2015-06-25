#ifndef PRELOADEDDATA_H_INCLUDED
#define PRELOADEDDATA_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include <string>
#include <map>
#include <vector>

enum EnemyType {

    ENEMY_GOOMBA,
    ENEMY_PIRANHA,
};

struct PreloadedData {

    int health;

    std::string textureFileName;
    sf::Time animationNextFrameTime;

    //texture rect for each animation state
    std::map<unsigned, std::vector<sf::IntRect> > animationTextureRects;

    //hitbox for each animation state
    std::map<unsigned, std::vector<sf::FloatRect> > hitboxes;
};

struct PreloadedTurretData : public PreloadedData {

    unsigned STATE_HIDING, STATE_COMING_OUT_OF_HIDING, STATE_GOING_INTO_HIDING, STATE_SHOOTING;

    //frames of the shooting animation
    unsigned DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT;

    sf::Time gunfireDelay;

    sf::Time hiddenStateDuration;
    sf::Time exposedStateDuration;
};

//struct to store preloaded data about an enemy so whenever a new enemy is created you don't have to open a file to retrieve info about it
struct PreloadedEnemyData : public PreloadedData{

    unsigned STATE_WALKING_LEFT;
    unsigned STATE_WALKING_RIGHT;
    unsigned STATE_FALLING_LEFT;
    unsigned STATE_FALLING_RIGHT;
};

struct PreloadedDataCollection {

    PreloadedEnemyData goombaData;
    PreloadedTurretData piranhaData;
};

#endif // PRELOADEDDATA_H_INCLUDED
