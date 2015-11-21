#ifndef PLAYERSTATSUI_H_INCLUDED
#define PLAYERSTATSUI_H_INCLUDED

#include "TextUIComponent.h"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <string>
#include <Memory>

class Player;

//this UI component displays a given player's player number, and remaining lives
//it keeps a pointer to the target player that way if any of his stats change, the UI component can handle the change
//if the player pointer becomes invalid then the UI will keep drawing but the data from it is basically useless so it should be removed
class PlayerStatsUI {

    public:

        PlayerStatsUI(const sf::Vector2f &textPosition, const std::string &fontFilePath);
        virtual ~PlayerStatsUI() {

        }

        void handleScreenResize(const sf::Vector2f &newScreenResolution);

        void update();
        void draw(sf::RenderWindow &window);

        bool doesTargetPlayerExist();

        void setTargetPlayer(std::shared_ptr<Player> &targetPlayer);

    private:

        //store data about the player's stats from the last update, this way it can check if anything changed using cached data, instead of from the text itself
        int playerLives;

        //pointer to player
        std::weak_ptr<Player> player;

        TextUIComponent playerStatsText;
};

#endif // PLAYERSTATSUI_H_INCLUDED
