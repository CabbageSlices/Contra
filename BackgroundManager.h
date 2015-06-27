#ifndef BACKGROUNDMANAGER_H_INCLUDED
#define BACKGROUNDMANAGER_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include <map>
#include <vector>
#include <utility>
#include <string>

//class that stores background for the level and displays them to the screen
//applies parallax scrolling to the background as well
class BackgroundManager {

    public:

        //how close the background is to the view
        static float IN_FRONT_OF_VIEW;
        static float CLOSE_TO_VIEW;
        static float NOT_CLOSE_OR_FAR_FROM_VIEW;
        static float FAR_FROM_VIEW;
        static float IN_THE_HORIZON;

        BackgroundManager();

        void insertBackground(const std::string &backgroundFileName, const float &distanceFromView, const sf::FloatRect &worldBounds);
        void clearBackgrounds();
        void clearLoadedBackgrounds();//also clears backgrounds since the textures become invalid

        void updateRendering(const sf::Vector2f &viewOffset);

        void draw(sf::RenderWindow &window);

    private:

        static std::map<std::string, sf::Texture> loadedTextures;

        //pair a background image with its distance from the view
        std::vector<std::pair<sf::RectangleShape, float> > backgrounds;
};

#endif // BACKGROUNDMANAGER_H_INCLUDED
