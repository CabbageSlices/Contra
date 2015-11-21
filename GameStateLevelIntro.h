#ifndef GAMESTATELEVELINTRO_H_INCLUDED
#define GAMESTATELEVELINTRO_H_INCLUDED

#include "GameStateBase.h"

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

class StateManager;

//before a level starts this intro will occur
//this intro assumes the level is the state right below it, that way the level draws beneath it
class GameStateLevelIntro : public GameState {

    public:

        GameStateLevelIntro(sf::RenderWindow &window);
        virtual ~GameStateLevelIntro() {

        }

        void handleInputEvents(sf::Event &event, sf::RenderWindow &window, StateManager& stateManager);
        void handleStateEvents(sf::RenderWindow &window, StateManager& stateManager);
        void update(float deltaTime, sf::RenderWindow &window, StateManager &stateManager);
        void draw(sf::RenderWindow &window, StateManager &stateManager);

    private:

        //for now the intro effect is have a black block slide away to reveal the level
		//so this rectnagle is the black box
        sf::RectangleShape cover;
        float coverTranslationSpeed;//in pixels/s because m/s conversion is a pain in the ass
};

#endif // GAMESTATELEVELINTRO_H_INCLUDED
