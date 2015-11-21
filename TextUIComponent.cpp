#include "TextUIComponent.h"
#include "DefaultSettings.h"

using std::map;
using std::string;

map<string, sf::Font> TextUIComponent::fonts;

TextUIComponent::TextUIComponent(const sf::Vector2f &defaultScreenRes, const sf::Vector2f &positionTopLeftCorner, const string &textToDisplay, const string &fontFilePath) :
    UserInterfaceComponent(defaultScreenRes, sf::Vector2f(1, 1), positionTopLeftCorner),
    text()
    {
        //load the specified font
        loadFont(fontFilePath);

        text.setFont(fonts[fontFilePath]);
        text.setCharacterSize(DEFAULT_TEXT_CHARACTER_SIZE);

        setText(textToDisplay);

        text.setPosition(positionTopLeftCorner);
    }

void TextUIComponent::setText(const string &newText) {

    //set the new message to display, and update the default size
    text.setString(newText);

    initialSize.x = text.getGlobalBounds().width;
    initialSize.y = text.getGlobalBounds().height;
}

void TextUIComponent::handleScreenResize(const sf::Vector2f &newScreenResolution) {

    //calcualte the scaling factor
    sf::Vector2f scalingFactor = calculateUIScalingFactor(defaultScreenResolution, newScreenResolution);

    sf::Vector2f newPosition(initialPosition.x * scalingFactor.x, initialPosition.y * scalingFactor.y);

    text.setScale(scalingFactor);

    text.setPosition(newPosition);
}

void TextUIComponent::drawInternal(sf::RenderTarget &target) {

    target.draw(text);
}

void TextUIComponent::loadFont(const string &fontFileName) {

    //if font isn't already loaded, do so
    if(fonts.count(fontFileName) == 0) {

        fonts[fontFileName].loadFromFile(fontFileName);
    }
}
