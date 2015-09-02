#include "BackgroundManager.h"

#include <iostream>

using std::cout;
using std::endl;
using std::map;
using std::string;

float BackgroundManager::IN_FRONT_OF_VIEW = 0;
float BackgroundManager::CLOSE_TO_VIEW = 0.25f;
float BackgroundManager::NOT_CLOSE_OR_FAR_FROM_VIEW = 0.5f;
float BackgroundManager::FAR_FROM_VIEW = 0.75f;
float BackgroundManager::IN_THE_HORIZON = 1.f;

map<string, sf::Texture> BackgroundManager::loadedTextures;

BackgroundManager::BackgroundManager() :
    backgrounds(),
    backgroundDistanceSaveData()
    {

    }

void BackgroundManager::insertBackground(const string &backgroundFileName, const float &distanceFromView, const sf::FloatRect &worldBounds) {

    //check if the given background has already been loaded
    if(!loadedTextures.count(backgroundFileName) && !loadedTextures[backgroundFileName].loadFromFile(backgroundFileName)) {

        //failed to load image so remove texturerect and exit
        loadedTextures.erase(backgroundFileName);
        return;
    }

    sf::Texture &textureToUse = loadedTextures[backgroundFileName];
    textureToUse.setRepeated(true);

    sf::RectangleShape newBackground(sf::Vector2f(worldBounds.width, worldBounds.height));
    newBackground.setTexture(&textureToUse);
    newBackground.setTextureRect(sf::IntRect(0, 0, worldBounds.width, worldBounds.height));

    auto backgroundPair = std::make_pair(newBackground, distanceFromView);

    //place it into the container in such a way that everything that comes before this object is farther from the view, that way the farther objects are covered by the closer ones
    //farther away objects have a larger distance from view, that is, their second value is larger
    auto insertionPosition = backgrounds.end();

    for(auto it = backgrounds.begin(); it != backgrounds.end(); ++it) {

        //current object is closer than new object, insert before curent object since new object is farther
        if(it->second < distanceFromView) {

            insertionPosition = it;
            break;
        }
    }

    backgrounds.insert(insertionPosition, backgroundPair);

    //store this background and its distance from the view in the savedata container so that the game can save which background is located at what distance
    auto backgroundDistancePair = std::make_pair(backgroundFileName, distanceFromView);
    backgroundDistanceSaveData.push_back(backgroundDistancePair);
}

void BackgroundManager::clearBackgrounds() {

    backgrounds.clear();
    backgroundDistanceSaveData.clear();
}

void BackgroundManager::clearLoadedBackgrounds() {

    loadedTextures.clear();
    clearBackgrounds();
}

void BackgroundManager::updateRendering(const sf::Vector2f &viewOffset) {

    //move all foreground objects by some fraction of hte views offset to simulate parallax scrolling
    for(auto &obj : backgrounds) {

        obj.first.move(viewOffset * obj.second);
    }
}

void BackgroundManager::draw(sf::RenderWindow &window) {

    for(auto &obj : backgrounds) {

        window.draw(obj.first);
    }
}

std::vector<std::pair<std::string, float> > &BackgroundManager::getDataForSaving() {

    return backgroundDistanceSaveData;
}
