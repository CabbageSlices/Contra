#ifndef STATETRANSITIONEFFECT_H_INCLUDED
#define STATETRANSITIONEFFECT_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

//class that handles transitions between different states
//all classes that inherit from this will be the actual effects
class StateTransitionEffect {

    public:

        virtual void begin() = 0;
        virtual void update(const float &delta, sf::RenderWindow &window) = 0;
        virtual void draw(sf::RenderWindow &window) = 0;

        virtual bool isFinished() = 0;
};

//has a black cover move in/out of the screen
//can inherit from this to have intro/outro effects
class BlackCoverWipe : public StateTransitionEffect {

    public:

        BlackCoverWipe(const sf::Vector2f &velocity);
        virtual ~BlackCoverWipe(){}

        virtual void update(const float &delta, sf::RenderWindow &window);
        virtual void draw(sf::RenderWindow &window);

    protected:

        //cover object
        sf::RectangleShape cover;

        //how fast the cover moves in, in pixels/sec
        sf::Vector2f coverVelocity;
};

#endif // STATETRANSITIONEFFECT_H_INCLUDED
