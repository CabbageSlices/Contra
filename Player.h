#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "GlobalConstants.h"
#include "PositionController.h"
#include "Tile.h"

struct PlayerKeys {

    sf::Keyboard::Key up = sf::Keyboard::Up;
    sf::Keyboard::Key down = sf::Keyboard::Down;
    sf::Keyboard::Key left = sf::Keyboard::Left;
    sf::Keyboard::Key right = sf::Keyboard::Right;

    sf::Keyboard::Key jump = sf::Keyboard::A;
    sf::Keyboard::Key shoot = sf::Keyboard::S;
};

class Player {

    public:

        Player(const PlayerKeys& keyConfiguration = PlayerKeys());

        void handleInputEvents(sf::Event& event, sf::RenderWindow& window);
        void handleKeystate(sf::RenderWindow& window);

        //time should be in seconds
        void update(const float& deltaTime, const sf::FloatRect& worldBounds, Tile& tile);

        void draw(sf::RenderWindow& window);


    private:

        bool checkCanJump() {

            //if player is falling it means he isn't standing on top of any object because if he was, his velocity would be 0
            //therefore he can't jump if his velocity isn't 0
            return canJump && positionController.getVelocitiesObjectSpace().y == 0;
        }

        bool checkIsJumping() {

            //player is jumping if he is moving upwards and he isn't able to jump
            return positionController.getVelocitiesObjectSpace().y < 0 && !canJump;
        }

        //check if player can extend his jump by holding the jump button
        bool checkExtendJump() {

            return holdingJump && extraJumpTimer.getElapsedTime() < extraJumpDuration;
        }

        void jump();

        //velocity of player when he begins to move
        //measured in meters per second
        const sf::Vector2f MOVEMENT_VELOCITY;

        bool canJump;

        //when user is holding jump he will jump higher
        //it will basically disable gravity until extra jump timer exceeds the max time allowed for user to hold jump
        bool holdingJump;
        sf::Clock extraJumpTimer;
        const sf::Time extraJumpDuration;

        PositionController positionController;

        sf::RectangleShape player;

        //the control setup for this player
        PlayerKeys controls;
};

#endif // PLAYER_H_INCLUDED
