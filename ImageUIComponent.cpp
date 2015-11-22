#include "ImageUIComponent.h"

using std::string;
using std::map;

map<string, sf::Texture> ImageUIComponent::textures;

ImageUIComponent::ImageUIComponent(const sf::Vector2f &defaultScreenRes, const string &imagePath, const sf::Vector2f &topLeftPosition) :
    UserInterfaceComponent(defaultScreenRes, sf::Vector2f(0, 0), topLeftPosition),
    sprite()
    {
        loadTexture(imagePath);
        sprite.setTexture(textures[imagePath]);
        sprite.setPosition(topLeftPosition);

        initialSize.x = sprite.getGlobalBounds().width;
        initialSize.y = sprite.getGlobalBounds().height;
    }

void ImageUIComponent::handleScreenResize(const sf::Vector2f &newScreenResolution) {

    sf::Vector2f scaleFactors = calculateUIScalingFactor(defaultScreenResolution, newScreenResolution);

    sf::Vector2f newPosition(initialPosition.x * scaleFactors.x, initialPosition.y * scaleFactors.y);

    sprite.setScale(scaleFactors);
    sprite.setPosition(newPosition);
}

void ImageUIComponent::drawInternal(sf::RenderTarget &target) {

    target.draw(sprite);
}

void ImageUIComponent::loadTexture(const string &texturePath) {

    if(textures.count(texturePath) == 0) {

        textures[texturePath].loadFromFile(texturePath);
    }
}
