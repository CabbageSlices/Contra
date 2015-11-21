#ifndef USERINTERFACECOMPONENT_H_INCLUDED
#define USERINTERFACECOMPONENT_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

//default view just means get a view that is the same size as the current target
//where the top left corner of the view lies at the point (0, 0) in the world
//useful for UI elements that never move
sf::View calculateDefaultViewForCurrentSize(sf::RenderTarget &target);

class UserInterfaceComponent {

    public:

        UserInterfaceComponent(const sf::Vector2f &defaultScreenRes, const sf::Vector2f &initialSz, const sf::Vector2f &initialPos);
        virtual ~UserInterfaceComponent() {

        }

        //take in a render target incase UI is drawn to seperate texutre or something
        void draw(sf::RenderTarget &target);

        //scale and reposition UI according to resolution, so their relative position on screen will always be the same
        //since this requires actual components to reposition it can't be implemented here
        virtual void handleScreenResize(const sf::Vector2f &newScreenResolution) = 0;

    protected:

        virtual void drawInternal(sf::RenderTarget &target) = 0;

        //UI object's size and position is dependent on screen resolution
        //when objects are first created, they are created with a certain resolution in mind
        //and any changes to the screen resolution needs to result in a change in the object's scale and position
        //keep track of what screen resolution was used to position the UI element, that way it can be scaled up or down when screen resolution changes
        sf::Vector2f defaultScreenResolution;

        //need a default size and position for the UI in order to determine its size and position after a screen resize
        sf::Vector2f initialSize;
        sf::Vector2f initialPosition;
};

//any UI component that users can interact with should inherit from this class
class InteractiveUserInterfaceComponent : public UserInterfaceComponent {

    public:

        InteractiveUserInterfaceComponent(const sf::Vector2f &defaultScreenResolution, const sf::Vector2f &initialSz, const sf::Vector2f &initialPos);

        virtual ~InteractiveUserInterfaceComponent() {

        }

        //seperate input handling functions for keystate and inputs
        //This way if a UI element can respon when a mouse is hovering over it, instead of having to trigger an event
        void handleInputEvents(sf::Event& event, sf::RenderWindow& window);
        void handleStateEvents(sf::RenderWindow &window);

    protected:

        //if UI element need to handle mouse input events then it needs the current mouse position, which needs extra work to calculate
        //so these functions provide the mouse positions precalculated
        virtual void handleMouseInputEvents(sf::Event &event, sf::RenderWindow &window, const sf::Vector2f &mousePos) = 0;
        virtual void handleMouseStateEvents(sf::RenderWindow &window, const sf::Vector2f &mousePos) = 0;
};

#endif // USERINTERFACECOMPONENT_H_INCLUDED
