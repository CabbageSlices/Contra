#ifndef POWERUP_H_INCLUDED
#define POWERUP_H_INCLUDED

#include "DynamicObject.h"
#include "Gun.h"
#include "Bullet.h"
#include "PreloadedData.h"
#include "TileMap.h"
#include <memory>

void applyPowerUp(std::shared_ptr<Gun> &gun, const PowerUpType &powerUpType);

class PowerUp : public DynamicObject {

    public:

        PowerUp(const glm::vec2 &positionWorldSpace, const PowerUpType &powerUpType, const PreloadedPowerUpData &powerupData);

        virtual ~PowerUp() {

        }

        virtual void updatePhysics(const float& deltaTime, const sf::FloatRect& worldBounds, TileMap& map);
        virtual void updateRendering();
        virtual void draw(sf::RenderWindow &window);
        virtual bool checkCanGetHit();
        virtual void getHit(int damage = 1);

        PowerUpType getPowerUpType() const;

        void load(const PreloadedPowerUpData &powerupData);

    private:

        void setFrameForPowerUpType();

        PowerUpType powerUpType;
};


#endif // POWERUP_H_INCLUDED
