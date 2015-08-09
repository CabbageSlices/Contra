#ifndef SHOOTINGENTITY_H_INCLUDED
#define SHOOTINGENTITY_H_INCLUDED

#include "EntityBase.h"
#include "Gun.h"
#include "Direction.h"

#include <memory>
#include <map>

class ShootingEntity : public EntityBase {

    public:

        ShootingEntity(const glm::vec2 &gravity, const glm::vec2 &movementVelocity, const glm::vec2 &terminalVelocity, const unsigned &initialHealth);

        virtual ~ShootingEntity() {

        }

        virtual void updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map) = 0;
        virtual void draw(sf::RenderWindow &window);

        std::shared_ptr<Gun>& getGun();

    protected:

        template<class PreloadedData>
        void loadShootingEntityData(PreloadedData &data);

        virtual void scaleComponents(const float &xFactor, const float &yFactor);

        std::shared_ptr<Gun> gun;
        Direction direction;

        //shooting entity should produce bullets at some position relative to its origin
        //keep track of the position the bullet shoudl be created at
        //in whatever state the inherited object specifies
        std::map<unsigned, glm::vec2> bulletOriginForState;
};

template<class PreloadedData>
void ShootingEntity::loadShootingEntityData(PreloadedData &data) {

    bulletOriginForState.clear();
    bulletOriginForState.insert(data.bulletOriginForState.begin(), data.bulletOriginForState.end());

    gun.reset();
    gun = createGunOfType(data.gunType);
    gun->setFireDelay(data.gunfireDelay);
    gun->setBulletType(data.bulletType);
}

#endif // SHOOTINGENTITY_H_INCLUDED
