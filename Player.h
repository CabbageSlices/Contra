#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "GlobalConstants.h"
#include "TileCollisionHandling.h"
#include "TileMap.h"
#include "PreloadedData.h"
#include "ShootingEntity.h"
#include <iostream>

using std::cout;
using std::endl;

struct PlayerKeys {

    sf::Keyboard::Key up = sf::Keyboard::W;
    sf::Keyboard::Key down = sf::Keyboard::S;
    sf::Keyboard::Key left = sf::Keyboard::A;
    sf::Keyboard::Key right = sf::Keyboard::D;

    sf::Keyboard::Key jump = sf::Keyboard::F;
    sf::Keyboard::Key fire = sf::Keyboard::G;
};

enum PlayerNumber : int {

    PLAYER_1 = 1,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4
};

class GameConfiguration;

//since player inherits from EntityBase his lives is equal to the health
class Player : public ShootingEntity{

    public:

        virtual ~Player() {

        }

        friend std::shared_ptr<Player> createPlayer(const glm::vec2 &spawnPosition, const PlayerNumber &number, GameConfiguration &gameConfig);

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

        void load(const PreloadedPlayerData &data);

        bool checkCanRespawn();
        void respawn(const sf::FloatRect &cameraBounds);//spawns at top of camera
        void setLives(const int &newLives);

        int getLives() const;

    private:

        Player(const glm::vec2 &spawnPosition, const PlayerKeys& keyConfiguration = PlayerKeys());

        bool checkCanJump() const;
        bool checkCanJumpDown() const; //check if player can jump down from the current platform
        bool checkIsJumping() const;
        bool checkIsCrouching() const;
        bool checkIsInAir() const;

        //check if player can extend his jump by holding the jump button
        bool checkExtendJump() const;

        void fireGun();
        void startDeathAnimation();

        void determineRenderingState();

        //calculate the position where the gun produces bullets relative to the player
        //differs whenever player is facing in different directions
        glm::vec2 calculateGunfireOrigin();

        //vertical and horizontal tile collisions differ only in terms of the collision handling function they call
        virtual CollisionResponse handleTileCollision(TileMap& map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object));

        void determineDirection();
        void jump();
        void jumpDown();
        void stopStandingOnPlatforms();

        void die();

        unsigned jumpingHitboxState;

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
        bool wasJumpButtonPressed;//when player isn't on the ground he could either be falling, or jumping, if the jump button was pressed then he is jumping, otherwise he is just falling
        sf::Clock extraJumpTimer;
        const sf::Time extraJumpDuration;

        //the control setup for this player
        PlayerKeys controls;

        sf::Clock respawnTimer;
        sf::Time respawnDelay;

        sf::Clock respawnInvinsibilityTimer;
        sf::Time respawnInvinsibilityDuration;
};

#endif // PLAYER_H_INCLUDED
