#include "GameStateOutro.h"
#include "DefaultSettings.h"

GameStateOutro::GameStateOutro(sf::RenderWindow &window, bool shouldPopPrecedingState, std::vector<State> &followingStates) :
    GameState(true),
    statesFollowingOutro(followingStates),
    popPrecedingState(shouldPopPrecedingState),
    cover(DEFAULT_SCREEN_RESOLUTION),
    coverTranslationSpeed(700)
    {
        //place cover off screen to the left
        cover.setPosition(-DEFAULT_SCREEN_RESOLUTION.x, 0);
        cover.setFillColor(sf::Color::Black);
    }

void GameStateOutro::handleInputEvents(sf::Event &event, sf::RenderWindow &window, StateManager &stateManager) {

    //do nothing for now
}

void GameStateOutro::handleStateEvents(sf::RenderWindow &window, StateManager& stateManager) {

}

void GameStateOutro::update(float deltaTime, sf::RenderWindow &window, StateManager &stateManager) {

    //move the cover, if its fully covered the screen, then transition is over
    float translation = coverTranslationSpeed * deltaTime;
    cover.move(translation, 0);

    if(cover.getGlobalBounds().left >= 0) {

        //cover has covered entire screen intro is finished
        stateManager.pop();

        if(popPrecedingState) {

            //the state beneath this one should be pe popped as well
            stateManager.pop();
        }

        //insert the states that follow the outro
        for(auto &state : statesFollowingOutro) {

            stateManager.push(state);
        }
    }
}

void GameStateOutro::draw(sf::RenderWindow &window, StateManager &stateManager) {

    //get rid of current view and restore to default so the box thing covers the whole screen
    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());

    window.draw(cover);

    window.setView(currentView);
}
