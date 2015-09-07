#ifndef TURRETENEMY_H_INCLUDED
#define TURRETENEMY_H_INCLUDED

#include "ShootingEntity.h"
#include "Direction.h"
#include <memory>
#include <vector>
#include <string>
#include "PreloadedData.h"
#include "glm/glm.hpp"

//turret enemy is an enemy that shoots at the player and doesn't move
//some turrets can also hide themselves, like pihrana plant from mario
//turret enemies don't move at all
//the reason they have a hitbox movement controller is because this will derive off the enemies class later
//and the enemy class requires one
class TurretEnemy : public ShootingEntity {

    public:

        TurretEnemy(const glm::vec2 &position, const int initialHealth = 5);

        virtual ~TurretEnemy() {

        }

        void createHitboxes(const std::vector<sf::FloatRect> &hitboxes);

        void updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map, const std::vector<glm::vec2> &targetPositions);
        void updateRendering();
        virtual void updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map) {

            //empty since turret enemies use the other update function
        }

        virtual bool checkCanGetHit();
        virtual void draw(sf::RenderWindow &window);

        void load(const PreloadedTurretData &data);

    private:

        //returns id of the target that is nearest to the turret
        //returns -1 if there are no valid targets
        unsigned getIdOfClosestTarget(const std::vector<glm::vec2> &targetPositions) const;

        //calculate the position where the gun produces bullets relative to the turret
        virtual glm::vec2 calculateGunfireOrigin(const glm::vec2 &targetPosition);
        void determineDirection(const glm::vec2 &targetPosition);

        virtual CollisionResponse handleTileCollision(TileMap &map, CollisionResponse(*collisionFunction)(std::shared_ptr<Tile>& tile, HitboxMovementController& object)) {

            //empty for now because this turret doesn't move so it doesn't collide with anything
            return CollisionResponse();
        }

        void restartStateDurationTimers();

        //timer to determine how long to stay in each state
        //this is only for states that aren't animated
        //animated states leave the state as soon as the animation is finished
        sf::Clock hiddenStateTimer;
        sf::Clock exposedStateTimer;

        sf::Time hiddenStateDuration;
        sf::Time exposedStateDuration;

        //drawing states
        unsigned STATE_HIDING;
        unsigned STATE_COMING_OUT_OF_HIDING;
        unsigned STATE_GOING_INTO_HIDING;
        unsigned STATE_SHOOTING;

        //texture rect index number for each direction the object faces when shooting
        unsigned DOWN;
        unsigned DOWN_LEFT;
        unsigned LEFT;
        unsigned UP_LEFT;
        unsigned UP;
        unsigned UP_RIGHT;
        unsigned RIGHT;
        unsigned DOWN_RIGHT;
};

#endif // TURRETENEMY_H_INCLUDED
