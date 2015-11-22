#ifndef GAMESTATETRANSITION_H_INCLUDED
#define GAMESTATETRANSITION_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "GameStateBase.h"
#include "StateManager.h"
#include <memory>

class StateTransitionEffect;

//a transition state between different game states, this state allows users to specify whether the state beneath it should be drawn as well
//this state plays some kind of transition effect, and when it finishes it will pop itself off the stack
//once this transition finishes, users can specify whether it should pop the state beneath it as well, this way if this drew overtop of another state, it can get rid of it
//once it finishs, you can also tell it to pop new states onto the stack,
//the states are pushed onto the stack in the same order that they are inserted into this transition state, so first in will be the first pushed onto the stack, which will be on the bottom of the stack
class GameStateTransition : public GameState {

    public:

        GameStateTransition(sf::RenderWindow &window, bool shouldDrawPrecedingState, bool shouldPopPrecedingState);
        virtual ~GameStateTransition(){}

        //inserts a state that will follow this one
        //once again the first one inserted will be the one at the bottom of the stack, and the last one isnerted will be the state that runs first, once transition is finished
        void insertFollowingState(State state);
        void setTransitionEffect(std::shared_ptr<StateTransitionEffect> &effect);

        virtual void handleInputEvents(sf::Event &event, sf::RenderWindow &window, StateManager& stateManager);
        virtual void handleStateEvents(sf::RenderWindow &window, StateManager& stateManager);
        virtual void update(float deltaTime, sf::RenderWindow &window, StateManager &stateManager);
        virtual void draw(sf::RenderWindow &window, StateManager &stateManager);

    private:

        //transition effect to play
        std::shared_ptr<StateTransitionEffect> transitionEffect;

        //queue of states to push onto the stack once this transition is over
        std::vector<State> statesFollowingTransition;

        //if set to true, once the outro finishes it will pop itself, as well as the state that came before it in the state stack
        bool popPrecedingState;
};

#endif // GAMESTATETRANSITION_H_INCLUDED
