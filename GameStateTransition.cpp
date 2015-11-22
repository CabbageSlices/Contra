#include "GameStateTransition.h"
#include "StateTransitionEffect.h"

using std::shared_ptr;

GameStateTransition::GameStateTransition(sf::RenderWindow &window, bool shouldDrawPrecedingState, bool shouldPopPrecedingState) :
    GameState(shouldDrawPrecedingState),
    transitionEffect(),
    statesFollowingTransition(),
    popPrecedingState(shouldPopPrecedingState)
    {

    }

void GameStateTransition::insertFollowingState(State state) {

    statesFollowingTransition.push_back(state);
}

void GameStateTransition::setTransitionEffect(shared_ptr<StateTransitionEffect> &effect) {

    transitionEffect = effect;
    transitionEffect->begin();
}

void GameStateTransition::handleInputEvents(sf::Event &event, sf::RenderWindow &window, StateManager &stateManager) {

    //do nothing for now
}

void GameStateTransition::handleStateEvents(sf::RenderWindow &window, StateManager& stateManager) {

}

void GameStateTransition::update(float deltaTime, sf::RenderWindow &window, StateManager &stateManager) {

    //if there is no effect, or the effect is over, exit this state
    if(!transitionEffect || transitionEffect->isFinished()) {

        //need to exit state
        //pop itself, and any other state as specified
        stateManager.pop();

        if(popPrecedingState) {

            stateManager.pop();
        }

        //add all states that come after the transition to the state stack
        for(auto &state : statesFollowingTransition) {

            stateManager.push(state);
        }

        return;
    }

    transitionEffect->update(deltaTime, window);
}

void GameStateTransition::draw(sf::RenderWindow &window, StateManager &stateManager) {

    if(transitionEffect) {

        transitionEffect->draw(window);
    }
}
