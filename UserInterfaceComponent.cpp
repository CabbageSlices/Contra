#include "UserInterfaceComponent.h"

sf::View calculateDefaultViewForCurrentSize(sf::RenderTarget &target) {

    sf::Vector2f viewSize(target.getSize().x, target.getSize().y);
    sf::Vector2f viewCenter = viewSize / 2.f;

    return sf::View(viewCenter, viewSize);
}

UserInterfaceComponent::UserInterfaceComponent(const sf::Vector2f &defaultScreenRes, const sf::Vector2f &initialSz, const sf::Vector2f &initialPos) :
    defaultScreenResolution(defaultScreenRes),
    initialSize(initialSz),
    initialPosition(initialPos)
    {

    }

void UserInterfaceComponent::draw(sf::RenderTarget &target) {

    //all UI should be drawn onto the screen regardless of views
    //so remove current view from target, and then create a default view FOR THE CURRENT TARGET'S RESOLUTION
    //that way the UI doesn't look stretched
    sf::View currentView = target.getView();

    sf::View defaultViewForSize = calculateDefaultViewForCurrentSize(target);

    target.setView(defaultViewForSize);

    this->drawInternal(target);

    target.setView(currentView);
}

InteractiveUserInterfaceComponent::InteractiveUserInterfaceComponent(const sf::Vector2f &defaultScreenRes, const sf::Vector2f &initialSz, const sf::Vector2f &initialPos) :
    UserInterfaceComponent(defaultScreenRes, initialSz, initialPos)
    {

    }

void InteractiveUserInterfaceComponent::handleInputEvents(sf::Event &event, sf::RenderWindow &window) {

    //to handle mouse related events you may need the mouse position on screen
    //since UI never moves, the screen needs to use a default view to calculate the mouse position on screen ,rather than in the world
    sf::View defaultViewForSize = calculateDefaultViewForCurrentSize(window);

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), defaultViewForSize);

    this->handleMouseInputEvents(event, window, mousePos);
}

void InteractiveUserInterfaceComponent::handleStateEvents(sf::RenderWindow &window) {

    sf::View defaultViewForSize = calculateDefaultViewForCurrentSize(window);

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), defaultViewForSize);

    this->handleMouseStateEvents(window, mousePos);
}
