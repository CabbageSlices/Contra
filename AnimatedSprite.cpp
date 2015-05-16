#include "AnimatedSprite.h"

#include <utility>

using std::pair;
using std::make_pair;

AnimatedSprite::AnimatedSprite(const sf::Time &nextFrameTime) :
    currentStateTextureRects(),
    currentAnimationState(0),
    texture(),
    sprite(),
    frame(0),
    timeToNextFrame(nextFrameTime),
    animationTimer()
    {
        sprite.setTexture(texture);
    }

bool AnimatedSprite::loadTexture(const std::string &fileName) {

    return texture.loadFromFile(fileName);
}

unsigned AnimatedSprite::insertTextureRect(const unsigned &animationState, const sf::IntRect &textureRect) {

    //create textureRects for the given animation state if nothing has been created yet
    if(currentStateTextureRects.count(animationState) == 0) {

        pair<unsigned, TextureRectContainer> stateTextureRect = make_pair(10, TextureRectContainer());
        currentStateTextureRects.insert(stateTextureRect);
    }

    currentStateTextureRects[animationState].push_back(textureRect);

    unsigned id = currentStateTextureRects[animationState].size() - 1;
    return id;
}

void AnimatedSprite::resetAnimation() {

    frame = 0;
    animationTimer.restart();
}

bool AnimatedSprite::animate() {

    //current animatoin state doesn't exist, return true so that maybe the user can change it
    if(currentStateTextureRects.count(currentAnimationState) == 0) {

        return true;
    }

    if(animationTimer.getElapsedTime() > timeToNextFrame) {

        animationTimer.restart();
        frame++;
    }

    if(frame >= currentStateTextureRects[currentAnimationState].size()) {

        frame = 0;
        return true;
    }

    applyActiveTextureRect();

    return false;
}

void AnimatedSprite::draw(sf::RenderWindow &window) {

    //don't draw anything if current animation state is invalid
    if(currentStateTextureRects.count(currentAnimationState) == 0 || frame >= currentStateTextureRects[currentAnimationState].size())  {

        return;
    }

    window.draw(sprite);
}

void AnimatedSprite::setAnimationState(const unsigned &animationState) {

    //only change states if the given state is valid
    if(currentStateTextureRects.count(animationState) != 0) {

        currentAnimationState = animationState;
    }
}

void AnimatedSprite::setFrame(const unsigned &currentFrame) {

    //make sure the given frame is valid and not greater than the number of textureRects that exist for current animation
    if(currentStateTextureRects.count(currentAnimationState) == 0 || currentFrame < 0) {

        return;
    }

    if(currentFrame >= currentStateTextureRects[currentAnimationState].size()) {

        return;
    }

    frame = currentFrame;
}

sf::Sprite& AnimatedSprite::getSprite() {


    return sprite;
}

void AnimatedSprite::applyActiveTextureRect() {

    //can't apply animation if current state is invalid
    if(currentStateTextureRects.count(currentAnimationState) == 0) {

        return;
    }

    if(frame >= currentStateTextureRects[currentAnimationState].size()) {

        return;
    }

    sf::IntRect rect = currentStateTextureRects[currentAnimationState][frame];
    sprite.setTextureRect(rect);
}
