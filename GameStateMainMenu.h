#ifndef GAMESTATEMAINMENU_H_INCLUDED
#define GAMESTATEMAINMENU_H_INCLUDED

#include "GameStateBase.h"
#include "Button.h"
#include "TextUIComponent.h"

class StateManager;

class GameStateMainMenu : public GameState {

    public:

        GameStateMainMenu(sf::RenderWindow &window, StateManager &stateManager);
        virtual ~GameStateMainMenu() {

        }

        virtual void handleInputEvents(sf::Event &event, sf::RenderWindow &window, StateManager& stateManager);
        virtual void handleStateEvents(sf::RenderWindow &window, StateManager& stateManager);
        virtual void update(float deltaTime, sf::RenderWindow &window, StateManager &stateManager);
        virtual void draw(sf::RenderWindow &window, StateManager &stateManager);

    private:

        void setupButtonFunctions(sf::RenderWindow &window, StateManager &stateManager);

        Button playGame;
};

#endif // GAMESTATEMAINMENU_H_INCLUDED
