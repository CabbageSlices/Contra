#ifndef SHOOTINGENTITY_H_INCLUDED
#define SHOOTINGENTITY_H_INCLUDED

#include "EntityBase.h"
#include "Gun.h"
#include "Direction.h"

#include <memory>

class ShootingEntity : public EntityBase {

    public:

        ShootingEntity(const glm::vec2 &gravity, const glm::vec2 &movementVelocity, const glm::vec2 &terminalVelocity, const unsigned &initialHealth);

        virtual void update(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map) = 0;
        virtual void draw(sf::RenderWindow &window);

        std::shared_ptr<Gun>& getGun();

    protected:

        std::shared_ptr<Gun> gun;
        Direction direction;
};

#endif // SHOOTINGENTITY_H_INCLUDED
