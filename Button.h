#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "UserInterfaceComponent.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include <string>
#include <functional>

class Button : public InteractiveUserInterfaceComponent {

    public:

        Button(const sf::Vector2f &screenResolution, const std::string &buttonImagePath, const sf::Vector2f &topLeftPosition);
        virtual ~Button() {

        }

        virtual void handleScreenResize(const sf::Vector2f &newScreenResolution);

        void setOnClickFunction(const std::function<void()> &callOnClick);

    private:

        virtual void drawInternal(sf::RenderTarget &target);
        virtual void handleMouseInputEvents(sf::Event &event, sf::RenderWindow &window, const sf::Vector2f &mousePos);
        virtual void handleMouseStateEvents(sf::RenderWindow &window, const sf::Vector2f &mousePos);

        sf::Sprite sprite;
        sf::Texture texture;

        std::function<void()> onClick;
};

#endif // BUTTON_H_INCLUDED
