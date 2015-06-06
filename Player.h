#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "GlobalConstants.h"
#include "TileCollisionHandling.h"
#include "TileMap.h"
#include "ShootingEntity.h"
#include <iostream>

using std::cout;
using std::endl;

struct PlayerKeys {

    sf::Keyboard::Key up = sf::Keyboard::Up;
    sf::Keyboard::Key down = sf::Keyboard::Down;
    sf::Keyboard::Key left = sf::Keyboard::Left;
    sf::Keyboard::Key right = sf::Keyboard::Right;

    sf::Keyboard::Key jump = sf::Keyboard::A;
    sf::Keyboard::Key fire = sf::Keyboard::S;
};

//since player inherits from EntityBase his lives is equal to the health
class Player : public ShootingEntity{

    public:

        Player(const PlayerKeys& keyConfiguration = PlayerKeys());

        void handleInputEvents(sf::Event& event, sf::RenderWindow& window);
        void handleKeystate(sf::RenderWindow& window);

        //time should be in seconds
        virtual void updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map);
        void updateRendering();
        virtual bool checkIsAlive();
        virtual void draw(sf::RenderWindow& window);
        virtual bool checkCanGetHit();
        virtual void getHit(int damage = 1);//damage parameters is pretty much ignored, its only needed for inheritance purposes
        virtual void respondToCollision(const CollisionResponse &collisionResponse);

        bool checkCanRespawn();
        void respawn(const sf::FloatRect &cameraBounds);//spawns at top of camera
        void setLives(const int &newLives);

        int getLives() const;

    private:

        bool checkCanJump() const;
        bool checkCanJumpDown() const; //check if player can jump down from the current platform
        bool checkIsJumping() const;
        bool checkIsCrouching() const;

        //check if player can extend his jump by holding the jump button
        bool checkExtendJump() const;

        //calculate the position where the gun produces bullets relative to the player
        //differs whenever player is facing in different directions
        glm::vec2 calculateGunfireOrigin() const;

        //vertical and horizontal tile collisions differ only in terms of the collision handling function they call
        virtual CollisionResponse handleTileCollision(TileMap& map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object));

        void determineDirection();
        void jump();
        void jumpDown();
        void stopStandingOnPlatforms();

        void die();

        enum LifeState {

            ALIVE, //while player is dying or dead he can't move or jump
            DYING,  //but he can't respawn unless he is dead
            DEAD

        } lifeState;

        //seperate flags to keep track of player standing on blocks/ground and tiles
        bool standingOnSolid;
        bool standingOnTile;
        bool standingOnPassablePlatform;//whether player is standing on a platform from which he can jump down

        //when user is holding jump he will jump higher
        //it will basically disable gravity until extra jump timer exceeds the max time allowed for user to hold jump
        bool holdingJump;
        sf::Clock extraJumpTimer;
        const sf::Time extraJumpDuration;

        //the control setup for this player
        PlayerKeys controls;

        sf::Clock respawnInvinsibilityTimer;
        sf::Time respawnInvinsibilityDuration;
};

#endif // PLAYER_H_INCLUDED
