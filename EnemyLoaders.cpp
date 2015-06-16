#include "EnemyLoaders.h"

bool loadEnemy(TurretEnemy &enemy, const std::string &dataFileName) {

    ///for now ignore the data file
    //load the different states
    enemy.STATE_HIDING = 0;
    enemy.STATE_COMING_OUT_OF_HIDING = 1;
    enemy.STATE_GOING_INTO_HIDING = 2;
    enemy.STATE_SHOOTING = 3;

    //load the animation properties
    enemy.stateDuration = sf::seconds(2.0);
    enemy.setHealth(8);

    //load the sprite
    enemy.sprite.loadTexture("pirana.png");

    //create the texture rects
    enemy.sprite.insertTextureRect(enemy.STATE_HIDING, sf::IntRect(0, 0, 0, 0));

    enemy.sprite.insertTextureRect(enemy.STATE_COMING_OUT_OF_HIDING, sf::IntRect(91, 0, 28, 100));
    enemy.sprite.insertTextureRect(enemy.STATE_COMING_OUT_OF_HIDING, sf::IntRect(162, 0, 38, 100));
    enemy.sprite.insertTextureRect(enemy.STATE_COMING_OUT_OF_HIDING, sf::IntRect(243, 0, 35, 100));
    enemy.sprite.insertTextureRect(enemy.STATE_COMING_OUT_OF_HIDING, sf::IntRect(328, 0, 31, 100));
    enemy.sprite.insertTextureRect(enemy.STATE_COMING_OUT_OF_HIDING, sf::IntRect(426, 0, 32, 10));

    enemy.sprite.insertTextureRect(enemy.STATE_GOING_INTO_HIDING, sf::IntRect(426, 0, 32, 100));
    enemy.sprite.insertTextureRect(enemy.STATE_GOING_INTO_HIDING, sf::IntRect(328, 0, 31, 100));
    enemy.sprite.insertTextureRect(enemy.STATE_GOING_INTO_HIDING, sf::IntRect(243, 0, 35, 100));
    enemy.sprite.insertTextureRect(enemy.STATE_GOING_INTO_HIDING, sf::IntRect(162, 0, 38, 100));
    enemy.sprite.insertTextureRect(enemy.STATE_GOING_INTO_HIDING, sf::IntRect(91, 0, 28, 100));

    enemy.DOWN = enemy.sprite.insertTextureRect(enemy.STATE_SHOOTING, sf::IntRect(156, 100, 44, 100));
    enemy.DOWN_LEFT = enemy.sprite.insertTextureRect(enemy.STATE_SHOOTING, sf::IntRect(156, 100, 44, 100));
    enemy.LEFT = enemy.sprite.insertTextureRect(enemy.STATE_SHOOTING, sf::IntRect(80, 100, 33, 100));
    enemy.UP_LEFT = enemy.sprite.insertTextureRect(enemy.STATE_SHOOTING, sf::IntRect(237, 100, 40, 100));
    enemy.UP = enemy.sprite.insertTextureRect(enemy.STATE_SHOOTING, sf::IntRect(321, 100, 41, 100));
    enemy.UP_RIGHT = enemy.sprite.insertTextureRect(enemy.STATE_SHOOTING, sf::IntRect(246, 200, 40, 100));
    enemy.RIGHT= enemy.sprite.insertTextureRect(enemy.STATE_SHOOTING, sf::IntRect(71, 200, 33, 100));
    enemy.DOWN_RIGHT = enemy.sprite.insertTextureRect(enemy.STATE_SHOOTING, sf::IntRect(161, 200, 44, 100));

    enemy.setState(enemy.STATE_HIDING);

    //create hitboxes
    enemy.hitbox.insertHitbox(sf::FloatRect(91, 88, 27, 12));
    enemy.hitbox.insertHitbox()
}
