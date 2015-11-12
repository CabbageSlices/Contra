#ifndef STATEMANAGER_H_INCLUDED
#define STATEMANAGER_H_INCLUDED

#include <vector>
#include <memory>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

/**


	THERE IS NO NEED TO WORRY ABOUT THE CURRENT ACTIVE GAMESTATE DELETING ITSELF OR REPLACING ITSELF IN THE MIDDLE OF A MEMBER FUNCTION CALL OF THE GAMESTATE
	THIS IS BECAUSE WHENEVER ANY STATE IS ACCESSED, IT IS ACCESSED THROUGH ANOTHER SHARED_PTR THAT WAY THERE WILL ALWAYS BE TWO REFERENCES TO THAT POINTER, ONE IN THE STACK, AND ONE LOCALLY IN THE STATEMANAGER FUNCTION
	SO THE CURRENT STACK OBJECT WILL BE VALID UNTIL THE END OF THE STATEMANAGER'S FUNCTION CALL, AND WIL BE VALID THROUGH OUT ALL OF THE FUNCTION CALLS TO MEMBER FUNCTIONS OF THE STATE CLASSES
**/

class GameState;

typedef std::shared_ptr<GameState> State;

//state manager class to manage game states, implemented as a stack of states, although internally it's an array managing the states
class StateManager {

    public:

        StateManager();

        //insert a new state to top, new state will become the active state
		void push(State state);

		//get rid of top most state, if any states are remaining then the top most state becomes the active state
		void pop();

		//gets rid of top most state and replaces it with the given state
		void changeState(State newState);

        //true if no states left, false otherwise
		bool isEmpty();

		//make currently active state handle event
		void handleInputEvents(sf::Event &event, sf::RenderWindow &window);
		void handleStateEvents(sf::RenderWindow &window);
		void update(const float &delta, sf::RenderWindow &window);
		void draw(sf::RenderWindow &window);

    private:

        //store the states in a vector instead of an actual stack object, that way you can actually access states aside from the topmost one
		//topmost state will be stored at the end of the vector
		std::vector<State> states;
};

#endif // STATEMANAGER_H_INCLUDED
