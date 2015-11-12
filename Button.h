#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "UserInterfaceComponent.h"

class Button : public InteractiveUserInterfaceComponent {

    public:

        Button(const sf::Vector2f &screenResolution, const sf::Vector2f &topLeftPosition);
        virtual void handleScreenResize(const sf::Vector2f &newScreenResolution);

    private:

        virtual void drawInternal(sf::RenderTarget &target);
        virtual void handleMouseInputEvents(sf::Event &event, sf::RenderWindow &window, const sf::Vector2f &mousePos);
        virtual void handleMouseStateEvents(sf::RenderWindow &window, const sf::Vector2f &mousePos);

        sf::RectangleShape button;
};

#endif // BUTTON_H_INCLUDED
