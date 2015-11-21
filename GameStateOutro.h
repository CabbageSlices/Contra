#ifndef GAMESTATEOUTRO_H_INCLUDED
#define GAMESTATEOUTRO_H_INCLUDED

#include "GameStateBase.h"
#include "StateManager.h"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <vector>
#include <memory>

//a gamestate that transitions out of some gamestate
//this gamestate is basically an effect that draws the previous gamestate below it, and makes it look like osme kind of transition effect, maybe
//i want this to play after a state finishes, but i also want an intro to play when a state starts, so generally if you want to use this intro,
//you'll have to pop and store the stack you want to get rid of, adn then push the new state you want, then push the intro to that state, then push the originial state, and then push this,
//that way this will finish, pop itself, and if you choose to it can pop the state beneath it, that way yo uget access to the correc tstates
//but then if you don't want the original state to pop, for example the main menu, there'd be no way to start the states that come after the original state
//because of this case, i opted to allow the outro to store what states should come after its over, and allow it to pop the state thats beneath it in the stack already
//this way once this state finishes, It can push the new states into the stack, the states will be pushed first come first serve, so a queue
//it will also, if specified, pop the state that is beneath it on the statestack, that way if you are leaving al evel for example, you can get rid of the old level and start the new one
//just make sure that if the new state you're pushing requires an intro, you insert the new state into the outro queue first, and then the intro, that way when its pushed to the stack,
//the most resetly pushed state will tbe the intro, and it'll play ontop of the actual state

//for now the effect is just a black box moving onto the screen from the left
class GameStateOutro : public GameState {

    public:

        GameStateOutro(sf::RenderWindow &window, bool shouldPopPrecedingState, std::vector<State> &followingStates);
        virtual ~GameStateOutro() {

        }

        virtual void handleInputEvents(sf::Event &event, sf::RenderWindow &window, StateManager& stateManager);
        virtual void handleStateEvents(sf::RenderWindow &window, StateManager& stateManager);
        virtual void update(float deltaTime, sf::RenderWindow &window, StateManager &stateManager);
        virtual void draw(sf::RenderWindow &window, StateManager &stateManager);

    private:

        std::vector<State> statesFollowingOutro;

        //if set to true, once the outro finishes it will pop itself, as well as the state that came before it in the state stack
        bool popPrecedingState;

        //for now the intro effect is have a black block slide in from the left to cover the level
		//so this rectnagle is the black box
        sf::RectangleShape cover;
        float coverTranslationSpeed;//in pixels/s because m/s conversion is a pain in the ass
};

#endif // GAMESTATEOUTRO_H_INCLUDED
