#ifndef GAMESTATEBASE_H_INCLUDED
#define GAMESTATEBASE_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class StateManager;

//base class for game states to inherit from
class GameState {

	public:

        GameState(bool drawPrevStates) :
            drawPreviousStates(drawPrevStates)
        {

        }

        virtual ~GameState() {

        }

        bool checkCanDrawPreviousStates() {

            return drawPreviousStates;
        }

        //it might be useful to have an oncreate and ondestroy, but that kind of functionality is what the constructors and destructors are for
        //and if the oncreate/ondestory are made virtual they can't be called in constructors/destructors

        virtual void handleInputEvents(sf::Event &event, sf::RenderWindow &window, StateManager& stateManager) = 0;
        virtual void handleStateEvents(sf::RenderWindow &window, StateManager& stateManager) = 0;
        virtual void update(float deltaTime, sf::RenderWindow &window, StateManager &stateManager) = 0;
        virtual void draw(sf::RenderWindow &window, StateManager &stateManager) = 0;

	private:

		//state whether or not this current state allows the states that come before it to be drawn underneath it
		//for example a pause menu might allow the gameplay to be drawn below it that way its not just the pauce menu on top of a black screen
		//the gameplay state mgiht not let previous states, like the main menu, be drawn since that 'd make no sense
		bool drawPreviousStates;
};

#endif // GAMESTATEBASE_H_INCLUDED
