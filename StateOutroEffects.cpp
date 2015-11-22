#include "StateOutroEffects.h"

BlackCoverWipeRightOutro::BlackCoverWipeRightOutro() :
    BlackCoverWipe(sf::Vector2f(700, 0))
    {

    }

void BlackCoverWipeRightOutro::begin() {

    //place offscreen to the left
    cover.setPosition(-cover.getGlobalBounds().width, 0);
}

bool BlackCoverWipeRightOutro::isFinished() {

    //if covers completely on screen now, then transition is over
    return cover.getPosition().x >= 0;
}
