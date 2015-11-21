#include "PlayerStatsUI.h"
#include "Player.h"
#include "DefaultSettings.h"
#include <sstream>

using std::stringstream;
using std::shared_ptr;

PlayerStatsUI::PlayerStatsUI(const sf::Vector2f &textPosition, const std::string &fontFilePath) :
    playerLives(-1),
    player(),
    playerStatsText(DEFAULT_SCREEN_RESOLUTION, textPosition, " ", fontFilePath)
    {

    }

void PlayerStatsUI::handleScreenResize(const sf::Vector2f &newScreenResolution) {

    playerStatsText.handleScreenResize(newScreenResolution);
}

void PlayerStatsUI::update() {

    //if player pointer is invalid then stop working
    if(player.expired()) {

        return;
    }

    //get players current stats, if its different from previous stats, then update the text
    if(playerLives == player.lock()->getLives()) {

        return;
    }

    //stats actually changed, so update text
    stringstream statsText;
    statsText << playerNumberToString(player.lock()->getPlayerNumber()) << "\n";
    statsText << "Lives: " << player.lock()->getLives();

    playerStatsText.setText(statsText.str());
}

void PlayerStatsUI::draw(sf::RenderWindow &window) {

    playerStatsText.draw(window);
}

bool PlayerStatsUI::doesTargetPlayerExist() {

    //target player exsists if the pointer isn't expired
    return !player.expired();
}

void PlayerStatsUI::setTargetPlayer(shared_ptr<Player> &targetPlayer) {

    player = targetPlayer;
    playerLives = -1;

    //call update to reset the text string
    update();
}
