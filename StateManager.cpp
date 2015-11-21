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

    //always get an external pointer to use the state, that way if the state pops itself off the stack, the pointer doesn't get deleted
    State state = states.back();
    state->handleInputEvents(event, window, *this);
}

void StateManager::handleStateEvents(sf::RenderWindow &window) {

    //get curently active state, and make it handle its events
    if(isEmpty()) {

        return;
    }

    //always get an external pointer to use the state, that way if the state pops itself off the stack, the pointer doesn't get deleted
    State state = states.back();
    state->handleStateEvents(window, *this);
}

void StateManager::update(const float &delta, sf::RenderWindow &window) {

    //get curently active state, and update it
    if(isEmpty()) {

        return;
    }

    //always get an external pointer to use the state, that way if the state pops itself off the stack, the pointer doesn't get deleted
    State state = states.back();
    state->update(delta, window, *this);
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

        //always get an external pointer to use the state, that way if the state pops itself off the stack, the pointer doesn't get deleted
        State state = states[indexFirstStateToDraw];
        state->draw(window, *this);
        ++indexFirstStateToDraw;
    }
}
