#ifndef TURRETENEMY_H_INCLUDED
#define TURRETENEMY_H_INCLUDED

#include "Gun.h"
#include "Direction.h"
#include "HitboxMovementController.h"
#include <memory>
#include <vector>

//turret enemy is an enemy that shoots at the player and doesn't move
//some turrets can also hide themselves, like pihrana plant from mario
//turret enemies don't move at all
//the reason they have a hitbox movement controller is because this will derive off the enemies class later
//and the enemy class requires one
class TurretEnemy {

    public:

        TurretEnemy(const glm::vec2 &position, const int initialHealth = 5);

        bool checkIsAlive() const;
        void createHitboxes(const std::vector<sf::FloatRect> &hitboxes);
        void update(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map, const std::vector<glm::vec2> &targetPositions);
        void draw(sf::RenderWindow &window);
        bool getHit(int damage = 1); //returns true if enemy is alive, false if dead

        const ObjectHitbox &getHitbox() const;

    private:

        //returns id of the target that is nearest to the turret
        //returns -1 if there are no valid targets
        unsigned getIdOfClosestTarget(const std::vector<glm::vec2> &targetPositions) const;

        //calculate the position where the gun produces bullets relative to the turret
        glm::vec2 calculateGunfireOrigin(const glm::vec2 &targetPosition) const;
        void determineDirection(const glm::vec2 &targetPosition);

        std::shared_ptr<Gun> gun;

        ObjectHitbox hitbox;
        HitboxMovementController hitboxMovementController;
        sf::RectangleShape enemy;
        Direction direction;
        int health;

};

#endif // TURRETENEMY_H_INCLUDED
