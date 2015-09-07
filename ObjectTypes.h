#ifndef OBJECTTYPES_H_INCLUDED
#define OBJECTTYPES_H_INCLUDED

enum EnemyType : int {

    ENEMY_GOOMBA,
    ENEMY_PIRANHA,
    ENEMY_MUSHROOM,
};

enum BulletType : int {

    BULLET_SLOW,
    BULLET_FAST,
    BULLET_MEDIUM,
};

enum DestructibleBlockType : int {

    BLOCK_BRICK = 0,
};

enum GunType {

    GUN_BASIC
};

enum PowerUpType {

    MACHINE_GUN
};

#endif // OBJECTTYPES_H_INCLUDED
