#include "Button.h"

Button::Button(const sf::Vector2f &screenResolution, const sf::Vector2f &topLeftPosition) :
    InteractiveUserInterfaceComponent(screenResolution, sf::Vector2f(100, 50), topLeftPosition),
    button(sf::Vector2f(100, 50))
    {
        button.setPosition(topLeftPosition);
    }

void Button::handleScreenResize(const sf::Vector2f &newScreenResolution) {

    //scale UI position and size according to screen resolution
    float horizontalScaling = (1.f / defaultScreenResolution.x) * newScreenResolution.x;
    float verticalScaling = (1.f / defaultScreenResolution.y) * newScreenResolution.y;

    sf::Vector2f newSize = initialSize * horizontalScaling;
    sf::Vector2f newPosition = initialPosition * verticalScaling;

    button.setSize(newSize);
    button.setPosition(newPosition);
}

void Button::drawInternal(sf::RenderTarget &target) {

    target.draw(button);
}

void Button::handleMouseInputEvents(sf::Event &event, sf::RenderWindow &window, const sf::Vector2f &mousePos) {

    //nothing for now, but do something when clicked
}

void Button::handleMouseStateEvents(sf::RenderWindow &window, const sf::Vector2f &mousePos) {

    //darken button when mouse hovers over it
    if(button.getGlobalBounds().contains(mousePos)) {

        button.setFillColor(sf::Color(155, 155, 155, 255));

    } else {

        button.setFillColor(sf::Color::White);
    }
}
