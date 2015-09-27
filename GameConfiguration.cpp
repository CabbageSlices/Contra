#include "GameConfiguration.h"
#include "WorldSavedataTags.h"
#include "StringManipulator.h"
#include "FileManipulators.h"

#include <string>

using std::string;

const std::string configurationFilename = "config.txt";
const std::string configurationFilePath = "data/";

const PlayerKeys& GameConfiguration::getKeysForPlayer(const PlayerNumber &playerNumber) {

    //return controls for player
    //if controls don't exist this will create a blank control scheme
    return playerControls[playerNumber];
}

bool GameConfiguration::save() {

    std::fstream file;
    file.open(configurationFilePath + configurationFilename, std::ios_base::out | std::ios_base::trunc);

    if(!file) {

        cout << "failed to open configuration file:" + configurationFilename;
        cout << " in the path:" + configurationFilePath << endl;
        return false;
    }

    savePlayerControls(file);
    return true;
}

bool GameConfiguration::load() {

    std::fstream file;
    file.open(configurationFilePath + configurationFilename, std::ios_base::in);

    if(!file) {

        cout << "failed to open configuration file:" + configurationFilename;
        cout << " in the path:" + configurationFilePath << endl;
        return false;
    }

    clearConfiguration();

    loadPlayerControls(file);
    return true;
}

void GameConfiguration::clearConfiguration() {

    playerControls.clear();
}

void GameConfiguration::savePlayerControls(std::fstream &file) {

    file << playerControlsTag.first << endl;

    //save all data on one line
    //start with the player ID as the enum number, that way it can be easily converted to enum after being read from the file
    //then save each key, save these as the enum values as well so its easy to convert to the key after being read from the file
    //save each key as the enum number
    //that way it can easily be converted to the enum
    for(auto &keyPair : playerControls) {

        file << (int)keyPair.first << " ";

        PlayerKeys &keyConfig = keyPair.second;

        file << (int)keyConfig.up << " ";
        file << (int)keyConfig.down << " ";
        file << (int)keyConfig.left << " ";
        file << (int)keyConfig.right << " ";
        file << (int)keyConfig.jump << " ";
        file << (int)keyConfig.fire << " ";
        file << endl;
    }

    file << playerControlsTag.second << endl;
}

void GameConfiguration::loadPlayerControls(std::fstream &file) {

    if(!readAfterLine(file, playerControlsTag.first)) {

        cout << "failed to find player controls data" << endl;
        return;
    }

    string extractedData;
    getline(file, extractedData);

    while(extractedData != playerControlsTag.second && file) {

        ///save format is predetermined
        ///read order is important
        PlayerKeys keys;
        PlayerNumber playerNumber;

        playerNumber = (PlayerNumber)atoi(extractFirstWordInString(extractedData).c_str());

        keys.up = (sf::Keyboard::Key)atoi(extractFirstWordInString(extractedData).c_str());
        keys.down = (sf::Keyboard::Key)atoi(extractFirstWordInString(extractedData).c_str());
        keys.left = (sf::Keyboard::Key)atoi(extractFirstWordInString(extractedData).c_str());
        keys.right = (sf::Keyboard::Key)atoi(extractFirstWordInString(extractedData).c_str());

        keys.jump = (sf::Keyboard::Key)atoi(extractFirstWordInString(extractedData).c_str());
        keys.fire = (sf::Keyboard::Key)atoi(extractFirstWordInString(extractedData).c_str());

        playerControls[playerNumber] = keys;

        extractedData = "";
        getline(file, extractedData);
    }
}
