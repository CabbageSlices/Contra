#ifndef GUN_H_INCLUDED
#define GUN_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "ObjectHitbox.h"
#include "Direction.h"
#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "PreloadedData.h"

class Bullet;
class TileMap;

//gun object position is relative to top left corner of its owner (heirachal model)
//the gun's initial firing direction is also specified by the owner
//these traits of the gun are all in the space of the object that owns the gun
//thus when the gun is fired it requires transformations from owner object space to world space
class Gun {

    public:

        Gun(const BulletType &type = BulletType::BULLET_PLAYER, const sf::Time &gunfireDelay = sf::seconds(0.3 / 2.3f));

        //user position is position of user in the world
        //bulletOriginUserSpace is the relative origin of the bullet with respect to the user's position
        void fire(const glm::vec2 &userPosition, const glm::vec2 &bulletOriginUserSpace, const Direction &fireDirection);
        void updatePhysics(const float &delta, const sf::FloatRect &worldBounds, TileMap& map);
        void updateRendering();
        void draw(sf::RenderWindow& window);

        std::vector<std::shared_ptr<Bullet> > &getBullets();

        void setFireDelay(const sf::Time &delay);
        void setBulletType(const BulletType &type);

    private:

        bool checkCanFire() {

            return timeSinceLastFired > fireDelay;
        }

        void createBullet(const glm::vec2 &positionWorldSpace, const glm::vec2 &directionWorldSpace);

        sf::Time timeSinceLastFired;
        sf::Time fireDelay;

        BulletType bulletType;

        std::vector<std::shared_ptr<Bullet> > bullets;
};

std::shared_ptr<Gun> createGunOfType(const GunType &type);

#endif // GUN_H_INCLUDED
