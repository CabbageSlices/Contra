#include "StateTransitionEffect.h"
#include "DefaultSettings.h"

BlackCoverWipe::BlackCoverWipe(const sf::Vector2f &velocity) :
    cover(DEFAULT_SCREEN_RESOLUTION),
    coverVelocity(velocity)
    {
        cover.setFillColor(sf::Color::Black);
    }

void BlackCoverWipe::update(const float &delta, sf::RenderWindow &window) {

    //calculate how far its translated
    sf::Vector2f offset = coverVelocity * delta;
    cover.move(offset);
}

void BlackCoverWipe::draw(sf::RenderWindow &window) {

    //restore view to default so that the cover will always fill the screen, regradless of the window size
    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());

    window.draw(cover);

    window.setView(currentView);
}
