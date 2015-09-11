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

        bool load();
        bool save();

    private:

        void clearConfiguration();

        void savePlayerControls(std::fstream &file);
        void loadPlayerControls(std::fstream &file);

        std::map<PlayerNumber, PlayerKeys> playerControls;
};

#endif // GAMECONFIGURATION_H_INCLUDED
