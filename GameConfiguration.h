#ifndef GAMECONFIGURATION_H_INCLUDED
#define GAMECONFIGURATION_H_INCLUDED

#include "Player.h"
#include <map>
#include <utility>
#include <fstream>

extern const std::string configurationFilename;
extern const std::string configurationFilePath;

class GameConfiguration {

    public:

        const PlayerKeys &getKeysForPlayer(const PlayerNumber &playerNumber);
        const sf::Vector2f getScreenResolution();

        bool load();
        bool save();

    private:

        void clearConfiguration();

        void savePlayerControls(std::fstream &file);
        void saveScreenResolution(std::fstream &file);

        void loadPlayerControls(std::fstream &file);
        void loadScreenResolution(std::fstream &file);

        std::map<PlayerNumber, PlayerKeys> playerControls;
        sf::Vector2f screenResolution;
};

#endif // GAMECONFIGURATION_H_INCLUDED
