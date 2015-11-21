#ifndef TEXTUICOMPONENT_H_INCLUDED
#define TEXTUICOMPONENT_H_INCLUDED

#include "UserInterfaceComponent.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include <map>
#include <string>

//UI component that displays a text on screen
class TextUIComponent : public UserInterfaceComponent {

    public:

        TextUIComponent(const sf::Vector2f &defaultScreenRes, const sf::Vector2f &positionTopLeftCorner, const std::string &textToDisplay, const std::string &fontFilePath);
        virtual ~TextUIComponent() {

        }

        void setText(const std::string &newText);
        virtual void handleScreenResize(const sf::Vector2f &newScreenResolution);

    private:

        virtual void drawInternal(sf::RenderTarget &target);
        void loadFont(const std::string &fontFileName);

        static std::map<std::string, sf::Font> fonts;

        sf::Text text;
};

#endif // TEXTUICOMPONENT_H_INCLUDED
