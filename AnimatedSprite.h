#ifndef ANIMATEDSPRITE_H_INCLUDED
#define ANIMATEDSPRITE_H_INCLUDED

#include <map>
#include <vector>
#include <string>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

//class that manages an object's animation state
class AnimatedSprite {

    public:

        AnimatedSprite(const sf::Time &nextFrameTime);

        bool loadTexture(const std::string &fileName);

        //returns id of texture rect in the texture container for the given state
        unsigned insertTextureRect(const unsigned &animationState, const sf::IntRect &textureRect);
        void resetAnimation();

        //returns true if current animation is finished, and restarts the current animation once finished
        bool animate();
        void draw(sf::RenderWindow &window);

        void setNextFrameTime(const sf::Time &nextFrameTime);
        void setAnimationState(const unsigned &animationState);
        void setFrame(const unsigned &currentFrame);

        unsigned getAnimationState() const;
        unsigned getFrame() const;
        sf::Sprite& getSprite();

    private:

        void applyActiveTextureRect();
        typedef std::vector<sf::IntRect> TextureRectContainer;

        //key represents the current animation state, textureRectconatiner gives the textureRects for current states animation and is accessed by 'frame'
        std::map<unsigned, TextureRectContainer> currentStateTextureRects;
        unsigned currentAnimationState;
        sf::Texture texture;
        sf::Sprite sprite;

        unsigned frame;
        sf::Time timeToNextFrame;
        sf::Clock animationTimer;
};

#endif // ANIMATEDSPRITE_H_INCLUDED
