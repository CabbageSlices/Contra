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
        void loadBulletOriginData(PreloadedData &data);

        virtual void scaleComponents(const float &xFactor, const float &yFactor);

        std::shared_ptr<Gun> gun;
        Direction direction;

        //shooting entity should produce bullets at some position relative to its origin
        //keep track of the relative position a bullet should be created for each direction the entity is facing
        std::map<CombinedAxis::Direction, glm::vec2> bulletOriginForDirection;
};

template<class PreloadedData>
void ShootingEntity::loadBulletOriginData(PreloadedData &data) {

    bulletOriginForDirection.clear();
    bulletOriginForDirection.insert(data.bulletOriginForDirection.begin(), data.bulletOriginForDirection.end());
}

#endif // SHOOTINGENTITY_H_INCLUDED
