#include "Button.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::function;

Button::Button(const sf::Vector2f &defaultScreenRes, const string &buttonImagePath,const sf::Vector2f &topLeftPosition) :
    InteractiveUserInterfaceComponent(defaultScreenRes, sf::Vector2f(100, 50), topLeftPosition),
    sprite(),
    texture(),
    onClick()
    {
        texture.loadFromFile(buttonImagePath);
        sprite.setTexture(texture);

        //use texture rect as size because i might change the button class to use a single texture and each button is linked to a specific texture rect
        initialSize.x = sprite.getTextureRect().width;
        initialSize.y = sprite.getTextureRect().height;

        sprite.setPosition(topLeftPosition);
    }

void Button::handleScreenResize(const sf::Vector2f &newScreenResolution) {

    //scale UI position and size according to screen resolution
    float horizontalScaling = (1.f / defaultScreenResolution.x) * newScreenResolution.x;
    float verticalScaling = (1.f / defaultScreenResolution.y) * newScreenResolution.y;

    sf::Vector2f newPosition(initialPosition.x * horizontalScaling, initialPosition.y * verticalScaling);

    //for now don't scale the size since changing screen aspect ratios make them look retarded
    sprite.setScale(horizontalScaling, verticalScaling);

    sprite.setPosition(newPosition);
}

void Button::setOnClickFunction(const function<void()> &onClickFunction) {

    onClick = onClickFunction;
}

void Button::drawInternal(sf::RenderTarget &target) {

    target.draw(sprite);
}

void Button::handleMouseInputEvents(sf::Event &event, sf::RenderWindow &window, const sf::Vector2f &mousePos) {

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

        if(sprite.getGlobalBounds().contains(mousePos)) {

            onClick();
        }
    }
}

void Button::handleMouseStateEvents(sf::RenderWindow &window, const sf::Vector2f &mousePos) {

    //darken button when mouse hovers over it
    if(sprite.getGlobalBounds().contains(mousePos)) {

        sprite.setColor(sf::Color(155, 155, 155, 255));

    } else {

        sprite.setColor(sf::Color::White);
    }
}
