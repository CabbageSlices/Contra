#include "GameStateLevelIntro.h"
#include "DefaultSettings.h"
#include "StateManager.h"

GameStateLevelIntro::GameStateLevelIntro(sf::RenderWindow &window):
    GameState(true),
    cover(DEFAULT_SCREEN_RESOLUTION),
    coverTranslationSpeed(700)
    {
        //create the cover to be the same size as the default screen size, because when drawn it will use the default screen view so all screen resolution changes won't matter
        //alreayd done in initializer list
        //place left edge of cover to the left edge of the screen so that the cover can move off screen
        //technically already done for the default window resolution, so it should be fine for all of them
        cover.setFillColor(sf::Color::Black);
    }

void GameStateLevelIntro::handleInputEvents(sf::Event &event, sf::RenderWindow &window, StateManager& stateManager) {

    //maybe do something if hte screen resolution changes
}

void GameStateLevelIntro::handleStateEvents(sf::RenderWindow &window, StateManager& stateManager) {

}

void GameStateLevelIntro::update(float deltaTime, sf::RenderWindow &window, StateManager &stateManager) {

    //move the cover, if its offscreen then intro is over
    float translation = coverTranslationSpeed * deltaTime;
    cover.move(translation, 0);

    if(cover.getGlobalBounds().left > DEFAULT_SCREEN_RESOLUTION.x) {

        //cover is offscreen, intro finished
        stateManager.pop();
    }
}

void GameStateLevelIntro::draw(sf::RenderWindow &window, StateManager &stateManager) {

    //get rid of current view and restore to default
    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());

    window.draw(cover);

    window.setView(currentView);
}
