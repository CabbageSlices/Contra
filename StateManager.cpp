#include "StateManager.h"
#include "GameStateBase.h"

using std::shared_ptr;

StateManager::StateManager():
    states()
{


}

void StateManager::push(State state) {

    states.push_back(state);
}

void StateManager::pop() {

    if(states.size() > 0) {

        states.pop_back();
    }
}

void StateManager::changeState(State newState) {

    //get rid of current state and insert this new one
    pop();
    push(newState);
}

bool StateManager::isEmpty() {

    return states.empty();
}

void StateManager::handleInputEvents(sf::Event &event, sf::RenderWindow &window) {

    //get the currently active state and make it handle the event
    if(isEmpty()) {

        return;
    }

    states.back()->handleInputEvents(event, window, *this);
}

void StateManager::handleStateEvents(sf::RenderWindow &window) {

    //get curently active state, and make it handle its events
    if(isEmpty()) {

        return;
    }

    states.back()->handleStateEvents(window, *this);
}

void StateManager::update(const float &delta, sf::RenderWindow &window) {

    //get curently active state, and update it
    if(isEmpty()) {

        return;
    }

    states.back()->update(delta, window, *this);
}

void StateManager::draw(sf::RenderWindow &window) {

    if(isEmpty()) {

        return;
    }

    //draw any state taht can be seen
    //if the topmost states allow the inner states to be drawn, draw them
    //upper states need to be drawn last so that they aren't hidden behind the other states

    //starting from the newest state, find first state that doesn't allow the states beneath it to be drawn, this will be the first state drawn
    unsigned indexFirstStateToDraw = states.size() - 1;

    //don't bother looking for older states if you are already looking at the oldest state
    while(indexFirstStateToDraw > 0 && states[indexFirstStateToDraw]->checkCanDrawPreviousStates()) {

        --indexFirstStateToDraw;
    }

    //draw all states starting from the oldest one that can be drawn
    while(indexFirstStateToDraw < states.size()) {

        states[indexFirstStateToDraw]->draw(window, *this);
        ++indexFirstStateToDraw;
    }
}
