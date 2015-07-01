#ifndef COLLISIONRESOLUTION_H_INCLUDED
#define COLLISIONRESOLUTION_H_INCLUDED

#include <memory>
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include <functional>

class DestructibleBlock;
class EntityBase;
class Enemy;
class Bullet;
class Player;
class PowerUp;

//calculates the minimum distance required for rectA to move in order to escape collision with rectB
glm::vec2 calculateCollisionResolutionTranslation(sf::FloatRect rectA, sf::FloatRect rectB);

//collision with destestrucitble blocks and entities that ARENt'T BULLETS
void destructibleBlockEntityCollision(std::shared_ptr<DestructibleBlock> block, std::shared_ptr<EntityBase> entity);

//collision with bullets and entities that don't deflect bullets
void bulletNonDeflectingEntityCollision(std::shared_ptr<Bullet> bullet, std::shared_ptr<EntityBase> entity);

void playerEnemyEntityCollision(std::shared_ptr<Player> player, std::shared_ptr<EntityBase> enemy);

void playerPowerUpCollision(std::shared_ptr<Player> player, std::shared_ptr<PowerUp> powerUp);

#endif // COLLISIONRESOLUTION_H_INCLUDED
