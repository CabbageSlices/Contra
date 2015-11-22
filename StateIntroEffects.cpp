#include "StateIntroEffects.h"

BlackCoverWipeRightIntro::BlackCoverWipeRightIntro() :
    BlackCoverWipe(sf::Vector2f(700, 0))
    {

    }

void BlackCoverWipeRightIntro::begin() {

    //place so that its covering the screen
    cover.setPosition(0, 0);
}

bool BlackCoverWipeRightIntro::isFinished() {

    //if covers completely on screen now, then transition is over
    //since cover is the same width as the screen, you can use it's width as the screen bounds
    return cover.getPosition().x >= cover.getGlobalBounds().width;
}
